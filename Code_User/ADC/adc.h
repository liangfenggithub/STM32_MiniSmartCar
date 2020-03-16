#ifndef __ADC_H
#define __ADC_H


#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"
#include "dma.h"


//------------------------------------------------------------
#define		C_ADC_Channel			4		// ADC通道数
#define		N_ADC_Channel_1			1		// ADC通道1
#define		N_ADC_Channel_2			2		// ADC通道2
#define		N_ADC_Channel_3			3		// ADC通道3
#define		N_ADC_Channel_6			6		// ADC通道6
#define 	C_Channel_Sample		10		// ADC采样次数
//------------------------------------------------------------

//--------------------------------------------------------------------------
extern u8   C_ADC_Sample ;					// ADC采样次数
	
extern u32  AM_ADC_Channel1_Sample ;		// ADC通道1采样总计
extern u32  AM_ADC_Channel2_Sample ;		// ADC通道2采样总计
extern u32  AM_ADC_Channel3_Sample ;		// ADC通道3采样总计
extern u32  AM_ADC_Channel6_Sample ;		// ADC通道3采样总计
	
extern u16  AV_ADC_Channel1_Sample ;		// ADC_CH1平均值
extern u16  AV_ADC_Channel2_Sample ;		// ADC_CH1平均值
extern u16  AV_ADC_Channel3_Sample ;		// ADC_CH3平均值
extern u16  AV_ADC_Channel6_Sample ;		// ADC_CH6平均值

#define LEFT_TRA_ADC (u8)(AV_ADC_Channel1_Sample/16) //8位平均值
#define CENTER_TRA_ADC (u8)(AV_ADC_Channel2_Sample/16) //8位平均值
#define RIGHT_TRA_ADC (u8)(AV_ADC_Channel3_Sample/16) //8位平均值
#define BAT_VAL (AV_ADC_Channel6_Sample*100/4095*3.3*2) //8位平均值


extern u16 ADC1_Value[C_Channel_Sample][C_ADC_Channel];	// DMA搬运数据存放处
//--------------------------------------------------------------------------

// ADC初始化
void ADC1_Rocker_Init(void);

// 获取ADC通道1/3的平均值
void ADC1_Value_average(void);


#endif	/* __ADC_H */

