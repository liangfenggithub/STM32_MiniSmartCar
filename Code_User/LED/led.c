#include "led.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
// 初始化PB4、PB5为推挽输出
//-----------------------------------------------------------------------------------------
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		
	
	//注意，画原理图时没有多余引脚了，把LED连接到PC13了，由于PC13引脚特殊，其作为IO与单片机RTC功能不能同时存在
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//开启GPIOC和复用时钟


    PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器

    RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能后，PC13/14/15才可以当普通IO口使用

    BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是PC13,也可以当普通IO使用

    BKP_ITConfig(DISABLE);



    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//注意，PC13只能输出2mhz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_ResetBits(GPIOC,GPIO_Pin_13);				// PC13输出0
}
//-----------------------------------------------------------------------------------------
