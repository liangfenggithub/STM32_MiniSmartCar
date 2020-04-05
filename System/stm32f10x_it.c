/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


#include "bit_band.h"
#include "led.h"
#include "beep.h"
#include "servo.h"
#include "exti.h"
extern u16 C_TIM2_IT_Update ;	// TIM3的定时计次

extern u8  One_Ms_Timing ;		// 1ms全局时基

extern u8 F_100MS;			// 

extern u8 F_110MS;

extern u8 F_1S;

extern int left_real_speed;
extern int right_real_speed;

// 定时器3中断服务程序	 
// 1ms中断一次
//---------------------------------------------------------------------------------------
void TIM3_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 	// 判断是否为TIM3的更新中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  	// 清除TIM3更新中断标志 
		
		// 按键扫描时基
		//---------------------------------------------------
		One_Ms_Timing = 1;			// 1ms
		//---------------------------------------------------
		
		// (100ms)
		//---------------------------------------------------
		C_TIM2_IT_Update ++ ;
		
		if( C_TIM2_IT_Update % 100 == 0 )		
		{

			F_100MS = 1;		// 
			
		}
		// (500ms)
		//---------------------------------------------------
		if(C_TIM2_IT_Update % 110 == 0)
		{
			F_110MS = 1;
		}
		
		
	  if(C_TIM2_IT_Update >= 1000)
		{
			C_TIM2_IT_Update = 0 ;
			//1秒时间到，开始计算左右轮频率
			 left_real_speed = left_pulse_cnt * 44 / 120;//mm 单个轮胎转一圈有12个脉冲，轮胎周长为44毫米，利用1秒钟脉冲计算得到实际转速
			 right_real_speed = right_pulse_cnt * 44 / 120;
				
			left_pulse_cnt = 0;//清零脉冲计数器
			right_pulse_cnt = 0;
			
			F_1S = 1;
		}
		
		//---------------------------------------------------
	}
}
//---------------------------------------------------------------------------------------

//定时器2中断函数（100us）(这个定时器公用给超声波模块和舵机)
extern u32 us100HcCount;
extern u16 servo_duty;
u8 servo_cnt = 0;
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//中断检测
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断
		
		
		//超声波测距
		us100HcCount++;
		
		
		//舵机
		/*舵机控制频率为50hz，周期为20ms，不同占空比对应角度
			0.5ms--------------0;
			1.0ms------------45;
			1.5ms------------90;
			2.0ms-----------135;
			2.5ms-----------180;
			使用100us定时器，也就是设定的占空比值为 5-25 对应0-180 关系式为 0.1x+5 其中x为0-180
		*/
		if(++servo_cnt<servo_duty){
			 SERVO= 1;
		}else if(servo_cnt<199){//20ms对应为计数值为200
			 SERVO= 0;
		}else{
			servo_cnt = 0;
		}
		
		
		
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
