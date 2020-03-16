/*
###################################################
---------------------------------------------------

---------------------------------------------------

---------------------------------------------------
工程硬件平台：WiFi智能遥控小车
工程程序版本1.0.0.0
---------------------------------------------------

---------------------------------------------------
硬件连接：请参考《底盘原理理图》
---------------------------------------------------

---------------------------------------------------
程序功能：
1. 自动模式：循迹避障 语音报警
2. 遥控模式：wifi远程视频遥控
---------------------------------------------------
###################################################
*/

#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "oled.h"
#include "adc.h"
#include "spi.h"
#include "dma.h"
#include "beep.h"
#include "usart.h"
#include "string.h"
#include "pwm.h"
#include "MotorDrive.h"
#include "yuyin.h"
#include "ultra.h"
#include "servo.h"

// 全局变量
//------------------------------------------------------------------
u16 C_TIM2_IT_Update = 0 ;	// TIM3的定时计次


u8 mode  = 0;// 小车模式：遥控还是循迹避障

u8  One_Ms_Timing = 0 ;		// 1ms全局时基

u8 F_100MS = 0 ;			// 100ms标志位

u8 F_500MS = 0;// 500ms标志位

u8 speed_duty = 80;    //pwm的占空比

u8 servo_deg = 0;//舵机角度
//------------------------------------------------------------------



int main(void)
{
    // 变量定义
    //------------------------------------------------------------


    //------------------------------------------------------------


    // 注：程序中使用中断时，NVIC分组设置应尽量位于程序起始处，并且在设置后尽量不要再更改NVIC分组
    //------------------------------------------------------------------------------------------
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //NVIC分组2：2位抢占优先级，2位响应优先级

    delay_Init();				// 延时初始化（注：调用延时函数之前，必须先调用delay_Init()将SysTick初始化）

    YUYIN_Init();            //语音模块初始化
    YUYIN_ON(1);

    uart_init(9600);       //串口初始化

    LED_Init() ;				// 初始化LED硬件接口

    TIM3_Time_Init();			// 定时器3初始化

    TIM4_PWM_Init();         //PWM初始化

    KEY_Init() ;				// 初始化按键硬件接口

    DMA_Init_JX();				// DMA初始化（搬运ADC数据）

    ADC1_Rocker_Init();			// ADC初始化（摇杆）

    BEEP_Init();            //蜂鸣器初始化

    UlTRA_Init();          //超声波初始化
		
		SERVO_Init();          //舵机初始化

    // 初始化OLED
    //-----------------------------------------
    OLED_Init();				// 初始化OLED
    OLED_Clear(); 				// 清屏


    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    YUYIN_OFF();
//    OLED_ShowString(0, 0, "Smart Car");
//    OLED_ShowString(0, 2, "   by TangHao");
    //-----------------------------------------

    while(1)
    {
        // 1ms调度
        //------------------------------------------------------------------
        if( One_Ms_Timing == 1 )
        {
            One_Ms_Timing = 0 ;

            // 按键检测
            //-------------------
            RF2G4_KEY_Scan();

            //
            //--------------------------------------
            ADC1_Value_average();	// 获取ADC平均值


            //鸣叫控制
            BEEP_Loop();

        }
        //------------------------------------------------------------------

        //上
        if(F_KEY_Down[0] == 1) {
            F_KEY_Down[0] = 0;
            LED = !LED;
						//舵机测试
						if(servo_deg<180){
							servo_deg+=5;
						}else{
							servo_deg = 180;
						}
						printf("舵机角度 %d -- 高电平时间 %d\r\n",servo_deg,servo_duty);
						SERVO_Set(servo_deg);
					
            //Car_forward(speed_duty);

        }

        //中
        if(F_KEY_Down[2] == 1) {
            F_KEY_Down[2] = 0;
            Car_Turn_Left(50);
            YUYIN_ON(2);
        }

        //下
        if(F_KEY_Down[1] == 1) {
            F_KEY_Down[1] = 0;
            
						if(servo_deg>5){
							servo_deg-=5;
						}else{
							servo_deg = 5;
						}
						printf("舵机角度 %d -- 高电平时间 %d\r\n",servo_deg,servo_duty);
						SERVO_Set(servo_deg);
					
						//Car_backward(80);
            //YUYIN_OFF();
        }



        // 串口回显，测试摄像头命令解码
        //--------------------------------

        if(USART_RX_STA&0x8000) {
            printf("收到的有效命令为：%s\r\n",USART_RX_BUF);
            memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
            USART_RX_STA = 0;


        }

        //RF2G4_Send_Data[12] = (u8)(AV_ADC_Channel1_Sample / 16);


        // 100调度
        //--------------------------------
        if( F_100MS == 1 )
        {
            F_100MS = 0;

            // 	oled显示
            //-----------------------------------------------------
            OLED_ShowNum(0,0,LEFT_TRA_ADC,3,16);
            OLED_ShowNum(0,2,CENTER_TRA_ADC,3,16);
            OLED_ShowNum(0,4,RIGHT_TRA_ADC,3,16);
            OLED_ShowNum(0,6,BAT_VAL,4,16);


            //串口输出
            //-----------------------------------------------------
            //printf("track adc value is : %d -- %d -- %d\r\n",LEFT_TRA_ADC,CENTER_TRA_ADC,RIGHT_TRA_ADC);
        }

        // 500ms调度
        //--------------------------------
        if(F_500MS == 1) {
            int l= 0;
						F_500MS = 0;
						l = get_distance();
					  printf("超声波探测距离为：%d cm\r\n",l);
	
        }

    }
}

