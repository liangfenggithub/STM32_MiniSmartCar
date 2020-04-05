#include "servo.h"
u16 servo_duty = 15;//���ռ�ձ�,Ĭ��Ϊ90��

void SERVO_Init(void)		// PA15��Ϊ�������
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

//�Ƕ����ã����deg��0-180�ĽǶ�
void SERVO_Set(u8 deg)//�Ƕ�����
{
	if(deg>= 180)deg = 180;
	if(deg<= 0)deg = 0;
	//ʹ��100us��ʱ����Ҳ�����趨��ռ�ձ�ֵΪ 5-25 ��Ӧ0-180 ��ϵʽΪ 0.1x+5 ����xΪ0-180
	servo_duty = (u8)((0.1* (float)deg)+5);
}

