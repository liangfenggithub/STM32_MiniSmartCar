#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


// �궨��
//--------------------------------------------------
#define LED	PC_out(13)	// Green -- PC13



// ��ʼ��PB4��PB5
//--------------------------------------------------
void LED_Init(void);		// PC13��Ϊ���������



#endif	/* __LED_H */
