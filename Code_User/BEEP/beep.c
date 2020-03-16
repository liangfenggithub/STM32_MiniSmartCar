#include "beep.h"


unsigned int on_time_ms = 0;//����������ʱ��

// ��ʼ��PB15Ϊ�������
//-----------------------------------------------------------------------------------------
void BEEP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;					// ����GPIO_InitTypeDef�ͽṹ��


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// ʹ��GPIOB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	// PB15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// IO���ٶ�Ϊ10MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//
    GPIO_ResetBits(GPIOB,GPIO_Pin_15);				// PB15���0
}
//-----------------------------------------------------------------------------------------

// ��������
// time: ʱ�� ms
void BEEP_ON(unsigned int  time)
{
    on_time_ms = time;
}


//���������
void BEEP_Loop() {
    if(on_time_ms >0) {
        on_time_ms--;
        GPIO_SetBits(GPIOB,GPIO_Pin_15);				// PB15���1
    } else {
        GPIO_ResetBits(GPIOB,GPIO_Pin_15);				// PB15���0
    }
}

