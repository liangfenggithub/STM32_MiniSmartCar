#ifndef __SERVO_H
#define __SERVO_H
#include "stm32f10x.h"
// �궨��
//--------------------------------------------------
#define SERVO	PA_out(15)	// PA15

extern u16 servo_duty;//���ռ�ձ�
// ��ʼ��
//--------------------------------------------------
void SERVO_Init(void);		// PA15��Ϊ���������

void SERVO_Set(u8 deg);//�Ƕ�����
#endif
