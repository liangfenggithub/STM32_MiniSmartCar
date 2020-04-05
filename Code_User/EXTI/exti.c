#include "exti.h"
#include "usart.h"
#include "stm32f10x.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//外部中断0服务程序

int left_pulse_cnt = 0;//左侧轮胎计数
int right_pulse_cnt = 0;//左侧轮胎计数

void EXTIX_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //	按键端口初始化 PA0 PA12

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_12;//KEY0-KEY2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOE2,3,4



    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //GPIOA0 中断线以及中断初始化配置   下降沿触发
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

    EXTI_InitStructure.EXTI_Line=EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA12	  中断线以及中断初始化配置 下降沿触发 //KEY1
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
    printf("A0 下降沿中断\r\n");
		left_pulse_cnt++;
    EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位
}

//外部中断12服务程序
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
    {
        printf("A12 下降沿中断\r\n");
				right_pulse_cnt++;
        if(Bit_SET == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))
        {
            //上升沿处理
        }
        else if(Bit_RESET ==GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))
        {
            //下降沿处理
					

        }
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}

