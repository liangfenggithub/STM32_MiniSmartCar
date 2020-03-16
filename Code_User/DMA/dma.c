#include "dma.h"

// DMA初始化
//-------------------------------------------------------------------------------------------------------------------------------
void DMA_Init_JX(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 			// 使能DMA时钟
	
	/*
	// DMA_CH1(ADC1)的中断NVIC设置
	//----------------------------------------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  	// DMA_CH1(ADC1)	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	// 抢占优先级3级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  		// 子优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// 使能DMA_CH1(ADC1)
	NVIC_Init(&NVIC_InitStructure);  							// 初始化NVIC寄存器
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE ); 				// 允许DMA_CH1(ADC1)的DMA_IT_TC中断
	//----------------------------------------------------------------------------------------------
	*/
	
	// DMA_CH1(ADC1)参数设置
	//--------------------------------------------------------------------------------------------------------------------------
	DMA_DeInit(DMA1_Channel1); 													// 将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; 					// DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Value; 					// DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 							// 内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = C_ADC_Channel * C_Channel_Sample; 		// DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 			// 外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 					// 内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 		// 数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 							// 工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 						// DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 								// DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 								// 根据DMA_InitStruct中指定的参数初始化DMA的通道
	//--------------------------------------------------------------------------------------------------------------------------
	
	DMA_Cmd(DMA1_Channel1, ENABLE);		// 启动DMA通道
	
}
//-------------------------------------------------------------------------------------------------------------------------------
