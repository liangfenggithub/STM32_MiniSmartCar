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
#include "string.h"

// �궨��
//------------------------------------------------------------------
#define DEFAULT_MODE 0
#define PHONE_MODE 1
#define AUTO_MODE 2



//�ṹ�嶨��
//------------------------------------------------------------------
//�˵���ʾ�ṹ�壬��ֹ��Ļ�ظ�ˢ��
typedef struct
{
    uint8_t lock1;//λ��1����ͬ
    uint8_t lock2;//λ��1����ͬ
    uint8_t lock3;//λ��1����ͬ
    uint8_t lock4;//λ��1����ͬ
    uint8_t lock5;//λ��1����ͬ

} MENU_STRUCT;


// ȫ�ֱ���
//------------------------------------------------------------------
u16 C_TIM2_IT_Update = 0 ;	// TIM3�Ķ�ʱ�ƴ�


u8 mode  = 0;// С��ģʽ��1ң��  2.ѭ������

u8  One_Ms_Timing = 0 ;		// 1msȫ��ʱ��

u8 F_100MS = 0 ;			// 100ms��־λ

u8 F_500MS = 0;// 500ms��־λ

u8 speed_duty = 80;    //pwm��ռ�ձ�

u8 servo_deg = 0;//����Ƕ�

char inputStringCopy[20] = {0}; //�����ַ�������


char dirString[10] = {0}; //�����ַ���



MENU_STRUCT menu_default;
MENU_STRUCT menu_phone;
MENU_STRUCT menu_auto;

//------------------------------------------------------------------

// ȫ�ֺ���
//------------------------------------------------------------------

void serial_data_parse(void);//���ڽ�������
void setMenuReDisplay(MENU_STRUCT *menu);
void display_task(void);//��ʾ��������
unsigned int str_to_num(char* dig);

//��̨����
void back_task(void)
{
    // 1ms����
    //------------------------------------------------------------------
    if( One_Ms_Timing == 1 )
    {
        One_Ms_Timing = 0 ;

        // �������
        //-------------------
        KEY_Scan();

        //
        //--------------------------------------
        ADC1_Value_average();	// ��ȡADCƽ��ֵ


        //���п���
        BEEP_Loop();

    }
    //------------------------------------------------------------------
}

//------------------------------------------------------------------

//��ʾ��������
void display_task(void)
{
    switch(mode) {
    case DEFAULT_MODE:
        /***********************************����ģʽ��ʼ********************************************/
        //��Ļ��ʾ
        if(menu_default.lock1 == 0) {
            menu_default.lock1 = 1;
            OLED_ShowString(0,0,"            ",16);
            OLED_ShowString(0,0,"DEFAULT_MODE",16);
        }
        /***********************************����ģʽ����********************************************/
        break;
    case PHONE_MODE:
        /***********************************ң��ģʽ��ʼ********************************************/
        //ģʽ��ʾ
        if(menu_phone.lock1 == 0) {
            menu_phone.lock1 = 1;
            OLED_ShowString(0,0,"             ",16);
            OLED_ShowString(0,0,"PHONE_MODE",16);
        }
        //�����յ�����Ч������ʾ
        if(menu_phone.lock2 == 0) {
            menu_phone.lock2 = 1;
            OLED_ShowString(0,2,"                ",16);
            OLED_ShowString(0,2,inputStringCopy,8);
        }

        //�������ָ��Ϊ
        //�����յ�����Ч������ʾ
        if(menu_phone.lock3 == 0) {
            menu_phone.lock3 = 1;
            OLED_ShowString(0,4,"                ",8);
            OLED_ShowString(0,4,dirString,8);
            memset(dirString,0,sizeof(dirString));
        }


        /***********************************ң��ģʽ����********************************************/
        break;
    case AUTO_MODE:
        /***********************************�Զ�ģʽ��ʼ********************************************/
        //��Ļ��ʾ
        if(menu_auto.lock1 == 0) {
            menu_auto.lock1 = 1;
            OLED_ShowString(0,0,"             ",16);
            OLED_ShowString(0,0,"AUTO_MODE",16);
        }
        /***********************************�Զ�ģʽ����********************************************/
        break;
    default:
        break;
    }




//    //��
}


//������������
void key_task(void)
{
    switch(mode) {
    case DEFAULT_MODE:
        /***********************************����ģʽ��ʼ********************************************/

        switch(keyNum) {
        case 1://��
            mode = PHONE_MODE;
            printf("���� ң��ģʽ\r\n");
            setMenuReDisplay(&menu_phone);
            OLED_Clear(); 				// ����
            keyNum = 0;
            break;
        case 2://��



            keyNum = 0;
            break;
        case 3://��
            mode = AUTO_MODE;
            printf("���� �Զ�ģʽ\r\n");
            OLED_Clear(); 				// ����
            setMenuReDisplay(&menu_auto);
            keyNum = 0;
            break;
        case 5://�м�����
            mode = DEFAULT_MODE;
            printf("���� ����ģʽ\r\n");
            setMenuReDisplay(&menu_default);
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************����ģʽ����********************************************/
        break;
    case PHONE_MODE:
        /***********************************ң��ģʽ��ʼ********************************************/

        switch(keyNum) {
        case 1://��
            LED = !LED;

            keyNum = 0;
            break;
        case 2://��


            keyNum = 0;
            break;
        case 3://��



            keyNum = 0;
            break;
        case 5://�м�����
            mode = DEFAULT_MODE;
            printf("���� ����ģʽ\r\n");
            setMenuReDisplay(&menu_default);
            OLED_Clear(); 				// ����
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************ң��ģʽ����********************************************/
        break;
    case AUTO_MODE:
        /***********************************�Զ�ģʽ��ʼ********************************************/
        switch(keyNum) {
        case 1://��
            LED = !LED;

            keyNum = 0;
            break;
        case 2://��


            keyNum = 0;
            break;
        case 3://��


            keyNum = 0;
            break;
        case 5://�м�����
            mode = DEFAULT_MODE;
            printf("���� ����ģʽ\r\n");
            setMenuReDisplay(&menu_default);
            OLED_Clear(); 				// ����
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************�Զ�ģʽ����********************************************/
        break;
    default:
        break;
    }




//    //��
//    switch(keyNum) {
//    case 1://��
//        LED = !LED;
//        printf("�ϼ�����\r\n");

////        //�������
////        if(servo_deg<180) {
////            servo_deg+=5;
////        } else {
////            servo_deg = 180;
////        }
////        printf("����Ƕ� %d -- �ߵ�ƽʱ�� %d\r\n",servo_deg,servo_duty);
////        SERVO_Set(servo_deg);
//        keyNum = 0;
//        break;
//    case 2://��
//        printf("�м�����\r\n");
//        switch(mode) {
//        case DEFAULT_MODE:

//            break;
//        case PHONE_MODE:
//											/***********************************ң��ģʽ��ʼ********************************************/
//						mode = PHONE_MODE;
//            printf("���� ң��ģʽ\r\n");
//				/***********************************ң��ģʽ����********************************************/
//						/***********************************ң��ģʽ��ʼ********************************************/
//				/***********************************ң��ģʽ����********************************************/
//            break;
//        case AUTO_MODE:
//            break;
//        default:
//            break;
//        }
////        F_KEY_Down[2] = 0;
////        Car_Turn_Left(50);
////        YUYIN_ON(2);
//        keyNum = 0;
//        break;
//    case 3://��
//        printf("�¼�����\r\n");
//        switch(mode) {
//        case DEFAULT_MODE:
//            mode = PHONE_MODE;
//            printf("ѭ������ ģʽ\r\n");
//            break;
//        case PHONE_MODE:

//            break;
//        case AUTO_MODE:
//            break;
//        default:
//            break;
//        }
////        if(servo_deg>5) {
////            servo_deg-=5;
////        } else {
////            servo_deg = 5;
////        }
////        printf("����Ƕ� %d -- �ߵ�ƽʱ�� %d\r\n",servo_deg,servo_duty);
////        SERVO_Set(servo_deg);
//        keyNum = 0;
//        break;
//    default:
//        break;
//    }

}
//���ò˵���ˢ��Ļ��־
void setMenuReDisplay(MENU_STRUCT *menu) {
    menu->lock1 = 0;
    menu->lock2 = 0;
    menu->lock3 = 0;
    menu->lock4 = 0;
    menu->lock5 = 0;
}

//�ֻ�ң��ģʽ����
void mode_remote_control_task(void)
{
    if(newLineReceived == 1) {
        printf("�յ�����Ч����Ϊ��%s\r\n",inputString);
        memcpy(inputStringCopy,inputString,20);//��ʱֻ��ʾǰ20���ַ�
        menu_phone.lock2 = 0;//֪ͨˢ��
        memset(inputString,0,sizeof(inputString));
        serial_data_parse();
        newLineReceived = 0;
    }
}
//ѭ������ģʽ����
void mode_track_obstacle_task(void)
{
    // 500ms���� ���������
    //--------------------------------
    if(F_500MS == 1) {
        int l= 0;
        F_500MS = 0;
        l = get_distance();
        //printf("������̽�����Ϊ��%d cm\r\n",l);

    }
}



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

    uart_init(9600);       //���ڳ�ʼ��

    uart3_init(115200);            //����3��ʼ��

    YUYIN_Init();            //����ģ���ʼ��
    YUYIN_ON(1);



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


//    delay_ms(1000);
//    delay_ms(1000);
//    delay_ms(1000);
//    delay_ms(1000);
    YUYIN_OFF();
//    OLED_ShowString(0, 0, "Smart Car");
//    OLED_ShowString(0, 2, "   by TangHao");
    //-----------------------------------------
		 //Car_forward(80);
    while(1)
    {
        //��̨����
        back_task();

        //�����������
        key_task();

        //��ʾ��������
        display_task();

        //�ֻ�ң��ģʽ����
        mode_remote_control_task();

        //ѭ������ģʽ����
        mode_track_obstacle_task();


        // 100����
        //--------------------------------
        if( F_100MS == 1 )
        {
            F_100MS = 0;

            // 	oled��ʾ
            //-----------------------------------------------------
//            OLED_ShowNum(0,0,LEFT_TRA_ADC,3,16);
//            OLED_ShowNum(0,2,CENTER_TRA_ADC,3,16);
//            OLED_ShowNum(0,4,RIGHT_TRA_ADC,3,16);
//            OLED_ShowNum(0,6,BAT_VAL,4,16);


            //�������
            //-----------------------------------------------------
            //printf("track adc value is : %d -- %d -- %d\r\n",LEFT_TRA_ADC,CENTER_TRA_ADC,RIGHT_TRA_ADC);
        }



    }
}



// ���ڻ��ԣ���������ͷ�������
//--------------------------------

//        if(USART_RX_STA&0x8000) {
//            printf("�յ�����Ч����Ϊ��%s\r\n",USART_RX_BUF);
//            memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
//            USART_RX_STA = 0;
//        }
//
//RF2G4_Send_Data[12] = (u8)(AV_ADC_Channel1_Sample / 16);

//�������ݽ���
void serial_data_parse(void)
{

    //�����ֻ�������ͨ��Э��ָ���ִ����Ӧ�Ķ���
    //$4WD,PTZ180#

    //if (inputStringCopy.indexOf("4WD") > 0)
    if(strstr(inputStringCopy,"4WD"))
    {
        //�����ֻ������Ķ����̨�Ŀ���ָ�ִ�ж����ת
        //��$4WD,PTZ180#���ת��180��
        //if (inputStringCopy.indexOf("PTZ") > 0)
        if(strstr(inputStringCopy,"PTZ")> 0)
        {
            //$4WD,PTZ69#
            char val[10] = {0};


            char * start;
            char * end;
            char aa = 0;
            start= strchr(inputStringCopy,'Z');
            start++;//����Ч���ֿ�ʼ��
            end = strchr(inputStringCopy,'#');
            if(end>start) {
                aa = end-start;
                memcpy(val,start,aa);
                printf("����PTZָ��\r\n");
                printf("��Ч����ַ��ǣ�%s\r\n",val);
                memcpy(dirString,val,aa);
                menu_phone.lock3 = 0;
                servo_deg =  str_to_num(val);
                SERVO_Set(servo_deg);
                newLineReceived = 0;
            }

            return;

        }
    }
    else if(strchr(inputStringCopy,'#')-strchr(inputStringCopy,'$')>12)
    {
        //$1,0,0,0,0,0,0,0,0,0#    С��ǰ��

        //�����ж�

        switch(inputStringCopy[1]) {
        case '0': //ֹͣ
            memcpy(dirString,"stop",sizeof("stop"));
            menu_phone.lock3 = 0;
            Car_Stop(0);
            break;
        case '1'://ǰ��
            memcpy(dirString,"go",sizeof("go"));
            menu_phone.lock3 = 0;
            Car_forward(speed_duty);
            break;
        case '2'://����
            memcpy(dirString,"back",sizeof("back"));
            menu_phone.lock3 = 0;
            Car_backward(speed_duty);
            break;
        case '3'://��ת
            memcpy(dirString,"left",sizeof("left"));
            menu_phone.lock3 = 0;
            Car_Turn_Left(speed_duty);
            break;
        case '4'://��ת
            memcpy(dirString,"right",sizeof("right"));
            menu_phone.lock3 = 0;
            Car_Turn_Right(speed_duty);
            break;
        }


    }
}

unsigned int str_to_num(char* dig)
{
    unsigned int num = 0;
    while(*dig != '\0')
    {
        if(*dig >= '0'&& *dig <= '9')
        {
            num = num * 10 + (*dig - '0');//(*dig-48)??
            dig++;
        }
        else
        {
            num = 0;
            break;
        }
    }
    return(num);
}


