#ifndef __YUYIN_H
#define __YUYIN_H

#include "stm32f10x.h"



// 初始化PB4、PB5
//--------------------------------------------------
void YUYIN_Init(void);		// PC13设为推挽输出高

void YUYIN_ON(char number);

void YUYIN_OFF(void);

#endif	/* __YUYIN_H */
