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
extern u16 C_TIM2_IT_Update ;	// TIM3�Ķ�ʱ�ƴ�

extern u8  One_Ms_Timing ;		// 1msȫ��ʱ��

extern u8 F_100MS;			// 

extern u8 F_110MS;

extern u8 F_1S;

extern int left_real_speed;
extern int right_real_speed;

// ��ʱ��3�жϷ������	 
// 1ms�ж�һ��
//---------------------------------------------------------------------------------------
void TIM3_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 	// �ж��Ƿ�ΪTIM3�ĸ����ж�
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  	// ���TIM3�����жϱ�־ 
		
		// ����ɨ��ʱ��
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
			//1��ʱ�䵽����ʼ����������Ƶ��
			 left_real_speed = left_pulse_cnt * 44 / 120;//mm ������̥תһȦ��12�����壬��̥�ܳ�Ϊ44���ף�����1�����������õ�ʵ��ת��
			 right_real_speed = right_pulse_cnt * 44 / 120;
				
			left_pulse_cnt = 0;//�������������
			right_pulse_cnt = 0;
			
			F_1S = 1;
		}
		
		//---------------------------------------------------
	}
}
//---------------------------------------------------------------------------------------

//��ʱ��2�жϺ�����100us��(�����ʱ�����ø�������ģ��Ͷ��)
extern u32 us100HcCount;
extern u16 servo_duty;
u8 servo_cnt = 0;
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//�жϼ��
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //����ж�
		
		
		//���������
		us100HcCount++;
		
		
		//���
		/*�������Ƶ��Ϊ50hz������Ϊ20ms����ͬռ�ձȶ�Ӧ�Ƕ�
			0.5ms--------------0;
			1.0ms------------45;
			1.5ms------------90;
			2.0ms-----------135;
			2.5ms-----------180;
			ʹ��100us��ʱ����Ҳ�����趨��ռ�ձ�ֵΪ 5-25 ��Ӧ0-180 ��ϵʽΪ 0.1x+5 ����xΪ0-180
		*/
		if(++servo_cnt<servo_duty){
			 SERVO= 1;
		}else if(servo_cnt<199){//20ms��ӦΪ����ֵΪ200
			 SERVO= 0;
		}else{
			servo_cnt = 0;
		}
		
		
		
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
