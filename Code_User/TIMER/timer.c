#include "timer.h"


// 初始化TIM3：
// 根据"timer.h"文件中的宏定义，设置TIM3的溢出时间
//--------------------------------------------------------------------------------------------------------------
void TIM3_Time_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		// 使能TIM3的时钟
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = TIM3_Reload_Num ; 		// 设置下一个更新事件后，装入自动重装载寄存器的值	
	TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Frequency_Divide;// 设置TIM3时钟预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			// 根据参数初始化TIM3的计数方式
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 					// 允许TIM3更新中断

	
	//TIM3的中断NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  			// TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	// 抢占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		// 子优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// 使能TIM3_IRQn通道
	NVIC_Init(&NVIC_InitStructure);  							// 初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  									// 使能TIM3
}
//--------------------------------------------------------------------------------------------------------------
