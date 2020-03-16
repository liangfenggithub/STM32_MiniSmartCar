#include "yuyin.h"

// 初始化PB4、PB5为推挽输出
//-----------------------------------------------------------------------------------------
void YUYIN_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;					// 定义GPIO_InitTypeDef型结构体

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);		// PB4的JTRST失能

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 使能GPIOB端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	// Green--PB4、Blue--PB5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// IO口速度为10MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//
    GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);				// PB4、PB5输出高
}
//-----------------------------------------------------------------------------------------

// 播放指定音乐（1-4）
// number: 1-4 播放00001-00004.MP3; 0 or 其他 停止播放
//-----------------------------------------------------------------------------------------
void YUYIN_ON(char number)
{
    switch(number) {
    case 1:
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);
				GPIO_SetBits(GPIOB,GPIO_Pin_5);				// PB4、PB5输出高
        break;
    case 2:
				GPIO_SetBits(GPIOB,GPIO_Pin_4);				// PB4、PB5输出高
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
        break;
    case 3:
				GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);
        break;
    case 4:
				GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);				// PB4、PB5输出高
        break;
    default:

        break;
    }
}
//关闭语音
void YUYIN_OFF(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);				// PB4、PB5输出高
}
