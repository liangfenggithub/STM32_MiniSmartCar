#include "MotorDrive.h"



// 右轮
//**************************************
// 右轮=前转(速度 = speed%)
//----------------------------------
void Motor_R_forward(u8 speed)
{
	PB7_Out_PP;
	PB_out(7) = 0;
	
	PB6_AF_PP;
	TIM_SetCompare1(TIM4, speed);
	
}
//----------------------------------

// 右轮=后转(速度 = -speed%)
//----------------------------------
void Motor_R_backward(u8 speed)
{
	PB6_Out_PP;
	PB_out(6) = 0;
	
	PB7_AF_PP;
	TIM_SetCompare2(TIM4, speed);
	
}
//----------------------------------


// 右轮=停止(熄火/刹车)
// Wheel_STOP = 0：熄火	1：刹车
//----------------------------------
void Motor_R_Stop(u8 Wheel_STOP)
{
	if( Wheel_STOP == 0 )	// 熄火
	{
		PB6_Out_PP;
		PB_out(6) = 0;
	
		PB7_Out_PP;
		PB_out(7) = 0;
	}
	
	else//if(Wheel_STOP==1)	// 刹车
	{
		PB6_Out_PP;
		PB_out(6) = 1;
	
		PB7_Out_PP;
		PB_out(7) = 1;
	}
}
//----------------------------------
//**************************************



// 左轮
//**************************************
// 左轮=前转(速度 = speed%)
//----------------------------------
void Motor_L_forward(u8 speed)
{
	PB9_Out_PP;
	PB_out(9) = 0;
	
	PB8_AF_PP;
	TIM_SetCompare3(TIM4, speed);
	
}
//----------------------------------

// 左轮轮=后转(速度 = -speed%)
//----------------------------------
void Motor_L_backward(u8 speed)
{
	PB8_Out_PP;
	PB_out(8) = 0;
	
	PB9_AF_PP;
	TIM_SetCompare4(TIM4, speed);
	
//		PB9_Out_PP;
//	PB_out(9) = 1;
}
//----------------------------------


// 左轮=停止(熄火/刹车)
// Wheel_STOP = 0：熄火	1：刹车
//----------------------------------
void Motor_L_Stop(u8 Wheel_STOP)
{
	if( Wheel_STOP == 0 )	// 熄火
	{
		PB8_Out_PP;
		PB_out(8) = 0;
	
		PB9_Out_PP;
		PB_out(9) = 0;
	}
	
	else//if(Wheel_STOP==1)	// 刹车
	{
		PB8_Out_PP;
		PB_out(8) = 1;
	
		PB9_Out_PP;
		PB_out(9) = 1;
	}
}
//----------------------------------
//**************************************




// 小车运动情况
//**************************************
// 小车=停止(熄火/刹车)
// Wheel_STOP = 0：熄火	1：刹车
//---------------------------------
void Car_Stop(u8 Wheel_STOP)
{
	if( Wheel_STOP == 0 )	// 熄火
	{
		Motor_L_Stop(0);
		Motor_R_Stop(0);
	}
	
	else//if(Wheel_STOP==1)	// 刹车
	{
		Motor_L_Stop(1);
		Motor_R_Stop(1);
	}
}
//---------------------------------


// 小车向前(速度 = speed%)
//---------------------------------
void Car_forward(u8 speed)
{
	Motor_L_forward(speed);
	Motor_R_forward(speed);

}
//---------------------------------


// 小车向后(速度 = -speed%)
//---------------------------------
void Car_backward(u8 speed)
{
	Motor_L_backward(speed);				
	Motor_R_backward(speed);
}
//---------------------------------



// 小车左转
// 左侧车轮向后转(速度 = -speed%)
// 右侧车轮向前转(速度 = speed%)
//---------------------------------
void Car_Turn_Left(u8 speed)
{
	Motor_L_backward(speed);
		
	Motor_R_forward(speed);

}
//---------------------------------


// 小车右转
// 右侧车轮向后转(速度 = -speed%)
// 左侧车轮向前转(速度 = speed%)
//---------------------------------
void Car_Turn_Right(u8 speed)
{
	Motor_L_forward(speed);
	Motor_R_backward(speed);
}
//---------------------------------

