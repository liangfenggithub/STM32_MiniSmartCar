#include "delay.h"

static u16 SysTick_each_us_CNT ;	// SysTick计时1us所需的计数次数
static u16 SysTick_each_ms_CNT ;	// SysTick计时1ms所需的计数次数

// 初始化SysTick定时器
void delay_Init(void)
{
	// RCC通过AHB时钟(HCLK)8分频后作为Cortex系统定时器(SysTick)的可选外部时钟
	// 默认：SystemCoreClock = SYSCLK_FREQ_72MHz ;
	//**********************************************************************************************************
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	// SysTick的时钟选为外部时钟的8分频（HCLK/8）
	
	SysTick_each_us_CNT = SystemCoreClock / 8 / 1000000;	// 得到1us的计数次数

	SysTick_each_ms_CNT = SysTick_each_us_CNT * 1000;		// 得到1ms的计数次数

}


// 延时delay_us_Num微秒	
// 参数delay_us_Num的值应 < 1,864,135(十进制)
//*********************************************
u8 delay_us(u32 delay_us_Num)
{		
	u8  NOP_CNT = 4 ;
	
	u32 T_SysTick_CTR ;
	
	
	// 如果没有此判断，当参数为0时，会陷入死循环
	//********************************************
	if( delay_us_Num == 0 )		// 参数不允许为0
	{ return 0 ; }				// 跳出函数
	
	
	// 当参数是1us时，不使用SysTick定时器，直接使用几个NOP实现1us延时
	//****************************************************************
	if( delay_us_Num == 1 )
	{
		while( --NOP_CNT )		// NOP_CNT = 4 ;
		{
			__NOP();			// NOP：一个指令周期的空操作
		}

		return 0 ;				// 跳出函数
	}
	
	
	// SysTick定时器真正计时前，程序多耗时1us左右，在此负补偿SysTick计数器
	//*****************************************************************
	if( delay_us_Num>1 && delay_us_Num<1000 )
	{ delay_us_Num -- ; }
	
	
	// SysTick->LOAD：SysTick重装载寄存器(SysTick->LOAD为24位计数器)
	// 当计数器递减到0时（或强制清0时），将SysTick->LOAD中的值重新载入计数器
	//**********************************************************************************************************
	SysTick->LOAD = (u32)SysTick_each_us_CNT * delay_us_Num ;			// 将此次函数的计数次数写入重装载寄存器中
	
	
	// SysTick->VAL：SysTick的递减计数器
	// 向SysTick->VAL中写入数据，其就会被清0。
	// 当其递减到0时，COUNTFLAG位置1。
	//**********************************************************************************************************
	SysTick->VAL = 0x00; 						// 将SysTick->VAL清0，从而使得SysTick->LOAD中的值装入SysTick->VAL
		

	// SysTick->CTRL的最低位为SysTick定时器的使能位：1使能 0失能
	//*****************************************************************************************
	SysTick->CTRL |= (u32)0x01 ;				// 使能SysTick定时器，计数器开始递减

	
	// 一直等待计数器递减至0
	//**********************************************************************************************************************
	do{
		T_SysTick_CTR = SysTick->CTRL ;			// 将SysTick->CTRL中的值缓存到T_SysTick_CTR变量中
		
	}while( (T_SysTick_CTR & 0x01) && !(T_SysTick_CTR & (1<<16) ) );	// 当SysTick->CTRL的最低位为1，且第16位为0时，继续循环
	
	
	SysTick->CTRL &= ~ ( (u32)0x01 ) ;			// 失能SysTick定时器

	SysTick->VAL = 0X00;      					// 清0计数器
	
	return 0 ;	// 跳出函数
}



// 延时delay_ms_Num毫秒
// 参数delay_ms_Num的值应 < 1,864(十进制)
//****************************************
u8 delay_ms(u16 delay_ms_Num)
{	 		  	  
	u32 T_SysTick_CTR ;
	
	// 如果没有此判断，当参数为0时，会陷入死循环
	//********************************************
	if( delay_ms_Num == 0 )		// 参数不允许为0
	{ return 0 ; }				// 跳出函数

	// SysTick->LOAD：SysTick重装载寄存器(SysTick->LOAD为24位计数器)
	// 当计数器递减到0时（或强制清0时），将SysTick->LOAD中的值重新载入计数器
	//**********************************************************************************************************
	SysTick->LOAD = (u32)SysTick_each_ms_CNT * delay_ms_Num ;			// 将此次函数的计数次数写入重装载寄存器中
	
	
	// SysTick->VAL：SysTick的递减计数器
	// 向SysTick->VAL中写入数据，其就会被清0。
	// 当其递减到0时，COUNTFLAG位置1。
	//**********************************************************************************************************
	SysTick->VAL = 0x00; 						// 将SysTick->VAL清0，从而使得SysTick->LOAD中的值装入SysTick->VAL
	
	
	// SysTick->CTRL的最低位为SysTick定时器的使能位：1使能 0失能
	//*****************************************************************************************
	SysTick->CTRL |= (u32)0x01 ;				// 使能SysTick定时器，计数器开始递减

		
	// 一直等待计数器递减至0
	//**********************************************************************************************************************
	do{
		T_SysTick_CTR = SysTick->CTRL ;			// 将SysTick->CTRL中的值缓存到T_SysTick_CTR变量中
		
	}while( (T_SysTick_CTR & 0x01) && !(T_SysTick_CTR & (1<<16) ) );	// 当SysTick->CTRL的最低位为1，且第16位为0时，继续循环
	
	
	SysTick->CTRL &= ~ ( (u32)0x01 ) ;			// 失能SysTick定时器
	
	SysTick->VAL = 0X00;      					// 清空计数器
	
	return 0 ;	// 跳出函数
}
