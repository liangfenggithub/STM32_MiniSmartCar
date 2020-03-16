#include "yuyin.h"

// ��ʼ��PB4��PB5Ϊ�������
//-----------------------------------------------------------------------------------------
void YUYIN_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;					// ����GPIO_InitTypeDef�ͽṹ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);		// PB4��JTRSTʧ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// ʹ��GPIOB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	// Green--PB4��Blue--PB5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// IO���ٶ�Ϊ10MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//
    GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);				// PB4��PB5�����
}
//-----------------------------------------------------------------------------------------

// ����ָ�����֣�1-4��
// number: 1-4 ����00001-00004.MP3; 0 or ���� ֹͣ����
//-----------------------------------------------------------------------------------------
void YUYIN_ON(char number)
{
    switch(number) {
    case 1:
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);
				GPIO_SetBits(GPIOB,GPIO_Pin_5);				// PB4��PB5�����
        break;
    case 2:
				GPIO_SetBits(GPIOB,GPIO_Pin_4);				// PB4��PB5�����
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
        break;
    case 3:
				GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);
        break;
    case 4:
				GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);				// PB4��PB5�����
        break;
    default:

        break;
    }
}
//�ر�����
void YUYIN_OFF(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);				// PB4��PB5�����
}
