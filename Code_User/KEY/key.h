#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"

//����оƬ����
#define KEYSHORTTIME 80//MS����̫�̣�����ᵼ��������������ʱͬʱ����ĳһ������
#define KEYLONGTIME 500//MS

// 3��������״̬������Ϊ1���ɿ�Ϊ0,�Ͱ���״̬������Ӧ�ö�ѡһ
//-------------------------------

extern u8 keyNum;
//-------------------------------


// ��������궨��
//------------------------------------------------------------------------
#define   LIMIT_TIME        200       // ������ʱ���ʱ��
#define 	KEY_UP_IN					PB_in(12)	// KEY_UP--PB12
#define 	KEY_DOWN_IN				PB_in(13)	// KEY_DOWN--PB13
#define 	KEY_OK_IN				  PB_in(14)	// KEY_DOWN--PB14

//------------------------------------------------------------------------

// �������
//--------------------------------------------------------------
#define 	KEY_UP				1				// ��
#define 	KEY_DOWN			3				// ��
#define 	KEY_OK			  2				// OK



// ������ʼ��
//----------------------------
void KEY_Init(void);

// ����ɨ�躯��
//----------------------------
void KEY_Scan(void);


#endif	/* __KEY_H */
