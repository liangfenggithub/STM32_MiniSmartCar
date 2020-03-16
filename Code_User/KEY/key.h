#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"

// 3��������״̬������Ϊ1���ɿ�Ϊ0
//-------------------------------
extern u8 F_KEY_Down[3];
//-------------------------------


// ��������궨��
//------------------------------------------------------------------------
#define   LIMIT_TIME        110       // ������ʱ���ʱ��
#define 	KEY_UP_in					PB_in(12)	// KEY_UP--PB12
#define 	KEY_DOWN_in				PB_in(13)	// KEY_DOWN--PB13
#define 	KEY_OK_in				  PB_in(14)	// KEY_DOWN--PB14

//------------------------------------------------------------------------

// �������
//--------------------------------------------------------------
#define 	KEY_UP				0				// ��
#define 	KEY_DOWN			1				// ��
#define 	KEY_OK			  2				// OK



// ������ʼ��
//----------------------------
void KEY_Init(void);

// ����ɨ�躯��
//----------------------------
void RF2G4_KEY_Scan(void);


#endif	/* __KEY_H */
