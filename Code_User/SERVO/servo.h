#ifndef __SERVO_H
#define __SERVO_H
#include "stm32f10x.h"
// 宏定义
//--------------------------------------------------
#define SERVO	PA_out(15)	// PA15

extern u16 servo_duty;//舵机占空比
// 初始化
//--------------------------------------------------
void SERVO_Init(void);		// PA15设为推挽输出高

void SERVO_Set(u8 deg);//角度设置
#endif
