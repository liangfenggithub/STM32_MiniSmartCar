#ifndef __YUYIN_H
#define __YUYIN_H

#include "stm32f10x.h"



// ��ʼ��PB4��PB5
//--------------------------------------------------
void YUYIN_Init(void);		// PC13��Ϊ���������

void YUYIN_ON(char number);

void YUYIN_OFF(void);

#endif	/* __YUYIN_H */
