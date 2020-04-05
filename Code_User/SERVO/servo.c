#include "servo.h"
u16 servo_duty = 15;//舵机占空比,默认为90度

void SERVO_Init(void)		// PA15设为推挽输出
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    
    /* Disable JLink, enable SW */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    /* Push-pill output, it can be other output types */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//角度设置，入参deg是0-180的角度
void SERVO_Set(u8 deg)//角度设置
{
	if(deg>= 180)deg = 180;
	if(deg<= 0)deg = 0;
	//使用100us定时器，也就是设定的占空比值为 5-25 对应0-180 关系式为 0.1x+5 其中x为0-180
	servo_duty = (u8)((0.1* (float)deg)+5);
}

