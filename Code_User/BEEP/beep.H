#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"


// �궨��
//--------------------------------------------------
#define BEEP	PB_out(15)	//


// ��������
//--------------------------------------------------
extern unsigned int on_time_ms;//����������ʱ��


// ��ʼ��pb15
//--------------------------------------------------
void BEEP_Init(void);		// �PB15��Ϊ�������0

extern void BEEP_Loop(void);


// ָ��ms������
//--------------------------------------------------
extern void BEEP_ON(unsigned int time);

#endif	/* __BEEP_H */

