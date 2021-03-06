#include "beep.h"


unsigned int on_time_ms = 0;//蜂鸣器鸣叫时间

// 初始化PB15为推挽输出
//-----------------------------------------------------------------------------------------
void BEEP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;					// 定义GPIO_InitTypeDef型结构体


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 使能GPIOB端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	// PB15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// IO口速度为10MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//
    GPIO_ResetBits(GPIOB,GPIO_Pin_15);				// PB15输出0
}
//-----------------------------------------------------------------------------------------

// 开启鸣叫
// time: 时长 ms
void BEEP_ON(unsigned int  time)
{
    on_time_ms = time;
}


//蜂鸣器检测
void BEEP_Loop() {
    if(on_time_ms >0) {
        on_time_ms--;
        GPIO_SetBits(GPIOB,GPIO_Pin_15);				// PB15输出1
    } else {
        GPIO_ResetBits(GPIOB,GPIO_Pin_15);				// PB15输出0
    }
}


