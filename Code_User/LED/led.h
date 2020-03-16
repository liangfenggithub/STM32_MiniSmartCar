#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


// 宏定义
//--------------------------------------------------
#define LED	PC_out(13)	// Green -- PC13



// 初始化PB4、PB5
//--------------------------------------------------
void LED_Init(void);		// PC13设为推挽输出高



#endif	/* __LED_H */
