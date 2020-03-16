#ifndef __MOTORDRIVE_H
#define __MOTORDRIVE_H


#include "stm32f10x.h"
#include "bit_band.h"


// С����Ϩ��/ɲ���궨��
//-----------------------------------
#define CAR_FLAMEOUT	0	// Ϩ��

#define CAR_BREAK		1	// ɲ��
//-----------------------------------


// ������PB6~PB9�ܽŵ����״̬


// PB6ģʽ����
//----------------------------------------------------------------------------
// PB6��ͨ���������
#define PB6_Out_PP  { GPIOB->CRL&=0XF0FFFFFF; GPIOB->CRL|=(u32)0x01<<(6*4); }

// PB6�������������
#define PB6_AF_PP   { GPIOB->CRL&=0XF0FFFFFF; GPIOB->CRL|=(u32)0x09<<(6*4); }
//----------------------------------------------------------------------------



// PB7ģʽ����
//----------------------------------------------------------------------------
// PB7��ͨ���������
#define PB7_Out_PP  { GPIOB->CRL&=0X0FFFFFFF; GPIOB->CRL|=(u32)0x01<<(7*4); }

// PB7�������������
#define PB7_AF_PP   { GPIOB->CRL&=0X0FFFFFFF; GPIOB->CRL|=(u32)0x09<<(7*4); }
//----------------------------------------------------------------------------



// PB8ģʽ����
//----------------------------------------------------------------------------
// PB8��ͨ���������
#define PB8_Out_PP  { GPIOB->CRH&=0XFFFFFFF0; GPIOB->CRH|=(u32)0x01<<(0*4); }

// PB8�������������
#define PB8_AF_PP   { GPIOB->CRH&=0XFFFFFFF0; GPIOB->CRH|=(u32)0x09<<(0*4); }
//----------------------------------------------------------------------------


// PB9ģʽ����
//----------------------------------------------------------------------------
// PB9��ͨ���������
#define PB9_Out_PP  { GPIOB->CRH&=0XFFFFFF0F; GPIOB->CRH|=(u32)0x01<<(1*4); }

// PB9�������������
#define PB9_AF_PP   { GPIOB->CRH&=0XFFFFFF0F; GPIOB->CRH|=(u32)0x09<<(1*4); }
//----------------------------------------------------------------------------
//------------------------------------------------------------------------------------



//----------------------------------------------------------------------------

void Motor_L_forward(u8 speed);		// �����=ǰת(�ٶ� = speed%)
void Motor_L_backward(u8 speed);		// �����=��ת(�ٶ� = -speed%)
void Motor_L_Stop(u8 Wheel_STOP);		// �����=ֹͣ(Ϩ��/ɲ��)

void Motor_R_forward(u8 speed);		// �Һ���=ǰת(�ٶ� = speed%)
void Motor_R_backward(u8 speed);		// �Һ���=��ת(�ٶ� = -speed%)
void Motor_R_Stop(u8 Wheel_STOP);		// ��ǰ��=ֹͣ(Ϩ��/ɲ��)


void Car_Stop(u8 Wheel_STOP);			// С��=ֹͣ(Ϩ��/ɲ��)
void Car_forward(u8 speed);				// С����ǰ(�ٶ� = speed%)
void Car_backward(u8 speed);			// С�����(�ٶ� = -speed%)

void Car_Turn_Left(u8 speed);			// С����ת(�ٶ� = speed%)
void Car_Turn_Right(u8 speed);			// С����ת(�ٶ� = speed%)
//----------------------------------------------------------------------------


#endif /* __MOTORDRIVE_H */
