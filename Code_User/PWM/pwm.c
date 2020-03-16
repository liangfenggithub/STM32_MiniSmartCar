#include "pwm.h"



// TIM4_PWM输出初始化 
// TIM4_CH1 = PB6
// TIM4_CH2 = PB7
// TIM4_CH3 = PB8
// TIM4_CH4 = PB9

// TIM4_Reload_Num			= TIM1自动重装值
// TIM4_Frequency_Divide	= 时钟预分频数
//-----------------------------------------------------------------------------------------------------------------
void TIM4_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			// 使能TIM4时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  			// 使能GPIOB时钟
	
	// 配置IO模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  				// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							
 
	
	//初始化TIM4的计数模式、分频值、重装载值等
	TIM_TimeBaseStructure.TIM_Period = TIM4_Reload_Num; 			// 设置下一个更新事件后，装入自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler =TIM4_Frequency_Divide; 	// 设置TIM4时钟预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	// 向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				// 根据参数初始化TIM4的计数方式
	
	
	//初始化TIM4_CH1-4的PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				// 选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// 输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  						
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  				// 使能TIM4在OC1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);				// 使能TIM4在OC2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);				// 使能TIM4在OC3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);				// 使能TIM4在OC4上的预装载寄存器
 
	TIM_Cmd(TIM4, ENABLE);											// 使能TIM4
}
//-----------------------------------------------------------------------------------------------------------------


