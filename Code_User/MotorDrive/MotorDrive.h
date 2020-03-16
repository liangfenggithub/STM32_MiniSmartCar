#ifndef __MOTORDRIVE_H
#define __MOTORDRIVE_H


#include "stm32f10x.h"
#include "bit_band.h"


// 小车的熄火/刹车宏定义
//-----------------------------------
#define CAR_FLAMEOUT	0	// 熄火

#define CAR_BREAK		1	// 刹车
//-----------------------------------


// 宏设置PB6~PB9管脚的输出状态


// PB6模式配置
//----------------------------------------------------------------------------
// PB6：通用推挽输出
#define PB6_Out_PP  { GPIOB->CRL&=0XF0FFFFFF; GPIOB->CRL|=(u32)0x01<<(6*4); }

// PB6：复用推挽输出
#define PB6_AF_PP   { GPIOB->CRL&=0XF0FFFFFF; GPIOB->CRL|=(u32)0x09<<(6*4); }
//----------------------------------------------------------------------------



// PB7模式配置
//----------------------------------------------------------------------------
// PB7：通用推挽输出
#define PB7_Out_PP  { GPIOB->CRL&=0X0FFFFFFF; GPIOB->CRL|=(u32)0x01<<(7*4); }

// PB7：复用推挽输出
#define PB7_AF_PP   { GPIOB->CRL&=0X0FFFFFFF; GPIOB->CRL|=(u32)0x09<<(7*4); }
//----------------------------------------------------------------------------



// PB8模式配置
//----------------------------------------------------------------------------
// PB8：通用推挽输出
#define PB8_Out_PP  { GPIOB->CRH&=0XFFFFFFF0; GPIOB->CRH|=(u32)0x01<<(0*4); }

// PB8：复用推挽输出
#define PB8_AF_PP   { GPIOB->CRH&=0XFFFFFFF0; GPIOB->CRH|=(u32)0x09<<(0*4); }
//----------------------------------------------------------------------------


// PB9模式配置
//----------------------------------------------------------------------------
// PB9：通用推挽输出
#define PB9_Out_PP  { GPIOB->CRH&=0XFFFFFF0F; GPIOB->CRH|=(u32)0x01<<(1*4); }

// PB9：复用推挽输出
#define PB9_AF_PP   { GPIOB->CRH&=0XFFFFFF0F; GPIOB->CRH|=(u32)0x09<<(1*4); }
//----------------------------------------------------------------------------
//------------------------------------------------------------------------------------



//----------------------------------------------------------------------------

void Motor_L_forward(u8 speed);		// 左后轮=前转(速度 = speed%)
void Motor_L_backward(u8 speed);		// 左后轮=后转(速度 = -speed%)
void Motor_L_Stop(u8 Wheel_STOP);		// 左后轮=停止(熄火/刹车)

void Motor_R_forward(u8 speed);		// 右后轮=前转(速度 = speed%)
void Motor_R_backward(u8 speed);		// 右后轮=后转(速度 = -speed%)
void Motor_R_Stop(u8 Wheel_STOP);		// 右前轮=停止(熄火/刹车)


void Car_Stop(u8 Wheel_STOP);			// 小车=停止(熄火/刹车)
void Car_forward(u8 speed);				// 小车向前(速度 = speed%)
void Car_backward(u8 speed);			// 小车向后(速度 = -speed%)

void Car_Turn_Left(u8 speed);			// 小车左转(速度 = speed%)
void Car_Turn_Right(u8 speed);			// 小车右转(速度 = speed%)
//----------------------------------------------------------------------------


#endif /* __MOTORDRIVE_H */
