#include "led.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
// ��ʼ��PB4��PB5Ϊ�������
//-----------------------------------------------------------------------------------------
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		
	
	//ע�⣬��ԭ��ͼʱû�ж��������ˣ���LED���ӵ�PC13�ˣ�����PC13�������⣬����ΪIO�뵥Ƭ��RTC���ܲ���ͬʱ����
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//����GPIOC�͸���ʱ��


    PWR_BackupAccessCmd(ENABLE);//�����޸�RTC �ͺ󱸼Ĵ���

    RCC_LSEConfig(RCC_LSE_OFF);//�ر��ⲿ�����ⲿʱ���źŹ��ܺ�PC13/14/15�ſ��Ե���ͨIO��ʹ��

    BKP_TamperPinCmd(DISABLE);//�ر����ּ�⹦�ܣ�Ҳ����PC13,Ҳ���Ե���ͨIOʹ��

    BKP_ITConfig(DISABLE);



    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//ע�⣬PC13ֻ�����2mhz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_ResetBits(GPIOC,GPIO_Pin_13);				// PC13���0
}
//-----------------------------------------------------------------------------------------
