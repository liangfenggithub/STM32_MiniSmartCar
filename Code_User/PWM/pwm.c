#include "pwm.h"



// TIM4_PWM�����ʼ�� 
// TIM4_CH1 = PB6
// TIM4_CH2 = PB7
// TIM4_CH3 = PB8
// TIM4_CH4 = PB9

// TIM4_Reload_Num			= TIM1�Զ���װֵ
// TIM4_Frequency_Divide	= ʱ��Ԥ��Ƶ��
//-----------------------------------------------------------------------------------------------------------------
void TIM4_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			// ʹ��TIM4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  			// ʹ��GPIOBʱ��
	
	// ����IOģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  				// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							
 
	
	//��ʼ��TIM4�ļ���ģʽ����Ƶֵ����װ��ֵ��
	TIM_TimeBaseStructure.TIM_Period = TIM4_Reload_Num; 			// ������һ�������¼���װ���Զ���װ�ؼĴ�����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =TIM4_Frequency_Divide; 	// ����TIM4ʱ��Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	// ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				// ���ݲ�����ʼ��TIM4�ļ�����ʽ
	
	
	//��ʼ��TIM4_CH1-4��PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				// ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// �������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  						
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  				// ʹ��TIM4��OC1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);				// ʹ��TIM4��OC2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);				// ʹ��TIM4��OC3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);				// ʹ��TIM4��OC4�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM4, ENABLE);											// ʹ��TIM4
}
//-----------------------------------------------------------------------------------------------------------------


