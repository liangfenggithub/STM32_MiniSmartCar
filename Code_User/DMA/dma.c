#include "dma.h"

// DMA��ʼ��
//-------------------------------------------------------------------------------------------------------------------------------
void DMA_Init_JX(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 			// ʹ��DMAʱ��
	
	/*
	// DMA_CH1(ADC1)���ж�NVIC����
	//----------------------------------------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  	// DMA_CH1(ADC1)	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	// ��ռ���ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  		// �����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// ʹ��DMA_CH1(ADC1)
	NVIC_Init(&NVIC_InitStructure);  							// ��ʼ��NVIC�Ĵ���
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE ); 				// ����DMA_CH1(ADC1)��DMA_IT_TC�ж�
	//----------------------------------------------------------------------------------------------
	*/
	
	// DMA_CH1(ADC1)��������
	//--------------------------------------------------------------------------------------------------------------------------
	DMA_DeInit(DMA1_Channel1); 													// ��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; 					// DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Value; 					// DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 							// �ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = C_ADC_Channel * C_Channel_Sample; 		// DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 			// �����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 					// �ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 		// ���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 							// ������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 						// DMAͨ�� xӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 								// DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 								// ����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
	//--------------------------------------------------------------------------------------------------------------------------
	
	DMA_Cmd(DMA1_Channel1, ENABLE);		// ����DMAͨ��
	
}
//-------------------------------------------------------------------------------------------------------------------------------
