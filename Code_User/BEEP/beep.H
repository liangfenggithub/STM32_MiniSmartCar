#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"


// 宏定义
//--------------------------------------------------
#define BEEP	PB_out(15)	//


// 变量定义
//--------------------------------------------------
extern unsigned int on_time_ms;//蜂鸣器鸣叫时间


// 初始化pb15
//--------------------------------------------------
void BEEP_Init(void);		// B15设为推挽输出0

extern void BEEP_Loop(void);


// 指定ms数鸣叫
//--------------------------------------------------
extern void BEEP_ON(unsigned int time);

#endif	/* __BEEP_H */


