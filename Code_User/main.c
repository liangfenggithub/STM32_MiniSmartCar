/*
###################################################
---------------------------------------------------

---------------------------------------------------

---------------------------------------------------
����Ӳ��ƽ̨��WiFi����ң��С��
���̳���汾�V1.0.0.0
---------------------------------------------------

---------------------------------------------------
Ӳ�����ӣ���ο�������ԭ����ͼ��
---------------------------------------------------

---------------------------------------------------
�����ܣ�
1. �Զ�ģʽ��ѭ������ ��������
2. ң��ģʽ��wifiԶ����Ƶң��
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

// ȫ�ֱ���
//------------------------------------------------------------------
u16 C_TIM2_IT_Update = 0 ;	// TIM3�Ķ�ʱ�ƴ�


u8 mode  = 0;// С��ģʽ��ң�ػ���ѭ������

u8  One_Ms_Timing = 0 ;		// 1msȫ��ʱ��

u8 F_100MS = 0 ;			// 100ms��־λ

u8 F_500MS = 0;// 500ms��־λ

u8 speed_duty = 80;    //pwm��ռ�ձ�

u8 servo_deg = 0;//����Ƕ�
//------------------------------------------------------------------



int main(void)
{
    // ��������
    //------------------------------------------------------------


    //------------------------------------------------------------


    // ע��������ʹ���ж�ʱ��NVIC��������Ӧ����λ�ڳ�����ʼ�������������ú�����Ҫ�ٸ���NVIC����
    //------------------------------------------------------------------------------------------
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //NVIC����2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�

    delay_Init();				// ��ʱ��ʼ����ע��������ʱ����֮ǰ�������ȵ���delay_Init()��SysTick��ʼ����

    YUYIN_Init();            //����ģ���ʼ��
    YUYIN_ON(1);

    uart_init(9600);       //���ڳ�ʼ��

    LED_Init() ;				// ��ʼ��LEDӲ���ӿ�

    TIM3_Time_Init();			// ��ʱ��3��ʼ��

    TIM4_PWM_Init();         //PWM��ʼ��

    KEY_Init() ;				// ��ʼ������Ӳ���ӿ�

    DMA_Init_JX();				// DMA��ʼ��������ADC���ݣ�

    ADC1_Rocker_Init();			// ADC��ʼ����ҡ�ˣ�

    BEEP_Init();            //��������ʼ��

    UlTRA_Init();          //��������ʼ��
		
		SERVO_Init();          //�����ʼ��

    // ��ʼ��OLED
    //-----------------------------------------
    OLED_Init();				// ��ʼ��OLED
    OLED_Clear(); 				// ����


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
        // 1ms����
        //------------------------------------------------------------------
        if( One_Ms_Timing == 1 )
        {
            One_Ms_Timing = 0 ;

            // �������
            //-------------------
            RF2G4_KEY_Scan();

            //
            //--------------------------------------
            ADC1_Value_average();	// ��ȡADCƽ��ֵ


            //���п���
            BEEP_Loop();

        }
        //------------------------------------------------------------------

        //��
        if(F_KEY_Down[0] == 1) {
            F_KEY_Down[0] = 0;
            LED = !LED;
						//�������
						if(servo_deg<180){
							servo_deg+=5;
						}else{
							servo_deg = 180;
						}
						printf("����Ƕ� %d -- �ߵ�ƽʱ�� %d\r\n",servo_deg,servo_duty);
						SERVO_Set(servo_deg);
					
            //Car_forward(speed_duty);

        }

        //��
        if(F_KEY_Down[2] == 1) {
            F_KEY_Down[2] = 0;
            Car_Turn_Left(50);
            YUYIN_ON(2);
        }

        //��
        if(F_KEY_Down[1] == 1) {
            F_KEY_Down[1] = 0;
            
						if(servo_deg>5){
							servo_deg-=5;
						}else{
							servo_deg = 5;
						}
						printf("����Ƕ� %d -- �ߵ�ƽʱ�� %d\r\n",servo_deg,servo_duty);
						SERVO_Set(servo_deg);
					
						//Car_backward(80);
            //YUYIN_OFF();
        }



        // ���ڻ��ԣ���������ͷ�������
        //--------------------------------

        if(USART_RX_STA&0x8000) {
            printf("�յ�����Ч����Ϊ��%s\r\n",USART_RX_BUF);
            memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
            USART_RX_STA = 0;


        }

        //RF2G4_Send_Data[12] = (u8)(AV_ADC_Channel1_Sample / 16);


        // 100����
        //--------------------------------
        if( F_100MS == 1 )
        {
            F_100MS = 0;

            // 	oled��ʾ
            //-----------------------------------------------------
            OLED_ShowNum(0,0,LEFT_TRA_ADC,3,16);
            OLED_ShowNum(0,2,CENTER_TRA_ADC,3,16);
            OLED_ShowNum(0,4,RIGHT_TRA_ADC,3,16);
            OLED_ShowNum(0,6,BAT_VAL,4,16);


            //�������
            //-----------------------------------------------------
            //printf("track adc value is : %d -- %d -- %d\r\n",LEFT_TRA_ADC,CENTER_TRA_ADC,RIGHT_TRA_ADC);
        }

        // 500ms����
        //--------------------------------
        if(F_500MS == 1) {
            int l= 0;
						F_500MS = 0;
						l = get_distance();
					  printf("������̽�����Ϊ��%d cm\r\n",l);
	
        }

    }
}

