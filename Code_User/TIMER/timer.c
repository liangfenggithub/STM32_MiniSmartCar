#include "timer.h"


// ��ʼ��TIM3��
// ����"timer.h"�ļ��еĺ궨�壬����TIM3�����ʱ��
//--------------------------------------------------------------------------------------------------------------
void TIM3_Time_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		// ʹ��TIM3��ʱ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = TIM3_Reload_Num ; 		// ������һ�������¼���װ���Զ���װ�ؼĴ�����ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Frequency_Divide;// ����TIM3ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			// ���ݲ�����ʼ��TIM3�ļ�����ʽ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 					// ����TIM3�����ж�

	
	//TIM3���ж�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  			// TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	// ��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		// �����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// ʹ��TIM3_IRQnͨ��
	NVIC_Init(&NVIC_InitStructure);  							// ��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  									// ʹ��TIM3
}
//--------------------------------------------------------------------------------------------------------------
