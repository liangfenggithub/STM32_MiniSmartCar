#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"

//触摸芯片配置
#define KEYSHORTTIME 80//MS不能太短，否则会导致两个按键按下时同时触发某一个按键
#define KEYLONGTIME 500//MS

// 3个按键的状态：按下为1，松开为0,和按键状态，根据应用二选一
//-------------------------------

extern u8 keyNum;
//-------------------------------


// 按键输入宏定义
//------------------------------------------------------------------------
#define   LIMIT_TIME        200       // 按键延时检测时间
#define 	KEY_UP_IN					PB_in(12)	// KEY_UP--PB12
#define 	KEY_DOWN_IN				PB_in(13)	// KEY_DOWN--PB13
#define 	KEY_OK_IN				  PB_in(14)	// KEY_DOWN--PB14

//------------------------------------------------------------------------

// 按键序号
//--------------------------------------------------------------
#define 	KEY_UP				1				// 上
#define 	KEY_DOWN			3				// 下
#define 	KEY_OK			  2				// OK



// 按键初始化
//----------------------------
void KEY_Init(void);

// 按键扫描函数
//----------------------------
void KEY_Scan(void);


#endif	/* __KEY_H */
