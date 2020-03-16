#include "MotorDrive.h"



// ����
//**************************************
// ����=ǰת(�ٶ� = speed%)
//----------------------------------
void Motor_R_forward(u8 speed)
{
	PB7_Out_PP;
	PB_out(7) = 0;
	
	PB6_AF_PP;
	TIM_SetCompare1(TIM4, speed);
	
}
//----------------------------------

// ����=��ת(�ٶ� = -speed%)
//----------------------------------
void Motor_R_backward(u8 speed)
{
	PB6_Out_PP;
	PB_out(6) = 0;
	
	PB7_AF_PP;
	TIM_SetCompare2(TIM4, speed);
	
}
//----------------------------------


// ����=ֹͣ(Ϩ��/ɲ��)
// Wheel_STOP = 0��Ϩ��	1��ɲ��
//----------------------------------
void Motor_R_Stop(u8 Wheel_STOP)
{
	if( Wheel_STOP == 0 )	// Ϩ��
	{
		PB6_Out_PP;
		PB_out(6) = 0;
	
		PB7_Out_PP;
		PB_out(7) = 0;
	}
	
	else//if(Wheel_STOP==1)	// ɲ��
	{
		PB6_Out_PP;
		PB_out(6) = 1;
	
		PB7_Out_PP;
		PB_out(7) = 1;
	}
}
//----------------------------------
//**************************************



// ����
//**************************************
// ����=ǰת(�ٶ� = speed%)
//----------------------------------
void Motor_L_forward(u8 speed)
{
	PB9_Out_PP;
	PB_out(9) = 0;
	
	PB8_AF_PP;
	TIM_SetCompare3(TIM4, speed);
	
}
//----------------------------------

// ������=��ת(�ٶ� = -speed%)
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


// ����=ֹͣ(Ϩ��/ɲ��)
// Wheel_STOP = 0��Ϩ��	1��ɲ��
//----------------------------------
void Motor_L_Stop(u8 Wheel_STOP)
{
	if( Wheel_STOP == 0 )	// Ϩ��
	{
		PB8_Out_PP;
		PB_out(8) = 0;
	
		PB9_Out_PP;
		PB_out(9) = 0;
	}
	
	else//if(Wheel_STOP==1)	// ɲ��
	{
		PB8_Out_PP;
		PB_out(8) = 1;
	
		PB9_Out_PP;
		PB_out(9) = 1;
	}
}
//----------------------------------
//**************************************




// С���˶����
//**************************************
// С��=ֹͣ(Ϩ��/ɲ��)
// Wheel_STOP = 0��Ϩ��	1��ɲ��
//---------------------------------
void Car_Stop(u8 Wheel_STOP)
{
	if( Wheel_STOP == 0 )	// Ϩ��
	{
		Motor_L_Stop(0);
		Motor_R_Stop(0);
	}
	
	else//if(Wheel_STOP==1)	// ɲ��
	{
		Motor_L_Stop(1);
		Motor_R_Stop(1);
	}
}
//---------------------------------


// С����ǰ(�ٶ� = speed%)
//---------------------------------
void Car_forward(u8 speed)
{
	Motor_L_forward(speed);
	Motor_R_forward(speed);

}
//---------------------------------


// С�����(�ٶ� = -speed%)
//---------------------------------
void Car_backward(u8 speed)
{
	Motor_L_backward(speed);				
	Motor_R_backward(speed);
}
//---------------------------------



// С����ת
// ��೵�����ת(�ٶ� = -speed%)
// �Ҳ೵����ǰת(�ٶ� = speed%)
//---------------------------------
void Car_Turn_Left(u8 speed)
{
	Motor_L_backward(speed);
		
	Motor_R_forward(speed);

}
//---------------------------------


// С����ת
// �Ҳ೵�����ת(�ٶ� = -speed%)
// ��೵����ǰת(�ٶ� = speed%)
//---------------------------------
void Car_Turn_Right(u8 speed)
{
	Motor_L_forward(speed);
	Motor_R_backward(speed);
}
//---------------------------------

