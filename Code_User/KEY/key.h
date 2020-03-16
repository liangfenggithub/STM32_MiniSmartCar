#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"

// 3个按键的状态：按下为1，松开为0
//-------------------------------
extern u8 F_KEY_Down[3];
//-------------------------------


// 按键输入宏定义
//------------------------------------------------------------------------
#define   LIMIT_TIME        110       // 按键延时检测时间
#define 	KEY_UP_in					PB_in(12)	// KEY_UP--PB12
#define 	KEY_DOWN_in				PB_in(13)	// KEY_DOWN--PB13
#define 	KEY_OK_in				  PB_in(14)	// KEY_DOWN--PB14

//------------------------------------------------------------------------

// 按键序号
//--------------------------------------------------------------
#define 	KEY_UP				0				// 上
#define 	KEY_DOWN			1				// 下
#define 	KEY_OK			  2				// OK



// 按键初始化
//----------------------------
void KEY_Init(void);

// 按键扫描函数
//----------------------------
void RF2G4_KEY_Scan(void);


#endif	/* __KEY_H */
