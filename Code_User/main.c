/*
###################################################
---------------------------------------------------

---------------------------------------------------

---------------------------------------------------
¹¤³ÌÓ²¼þÆ½Ì¨£ºWiFiÖÇÄÜÒ£¿ØÐ¡³µ
¹¤³Ì³ÌÐò°æ±¾£V1.0.0.0
---------------------------------------------------

---------------------------------------------------
Ó²¼þÁ¬½Ó£ºÇë²Î¿¼¡¶µ×ÅÌÔ­ÀíÀíÍ¼¡·
---------------------------------------------------

---------------------------------------------------
³ÌÐò¹¦ÄÜ£º
1. ×Ô¶¯Ä£Ê½£ºÑ­¼£±ÜÕÏ ÓïÒô±¨¾¯
2. Ò£¿ØÄ£Ê½£ºwifiÔ¶³ÌÊÓÆµÒ£¿Ø
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

// ºê¶¨Òå
//------------------------------------------------------------------
#define DEFAULT_MODE 0
#define PHONE_MODE 1
#define AUTO_MODE 2



//½á¹¹Ìå¶¨Òå
//------------------------------------------------------------------
//²Ëµ¥ÏÔÊ¾½á¹¹Ìå£¬·ÀÖ¹ÆÁÄ»ÖØ¸´Ë¢ÐÂ
typedef struct
{
    uint8_t lock1;//Î»ÖÃ1£¬ÏÂÍ¬
    uint8_t lock2;//Î»ÖÃ1£¬ÏÂÍ¬
    uint8_t lock3;//Î»ÖÃ1£¬ÏÂÍ¬
    uint8_t lock4;//Î»ÖÃ1£¬ÏÂÍ¬
    uint8_t lock5;//Î»ÖÃ1£¬ÏÂÍ¬

} MENU_STRUCT;


// È«¾Ö±äÁ¿
//------------------------------------------------------------------
u16 C_TIM2_IT_Update = 0 ;	// TIM3µÄ¶¨Ê±¼Æ´Î


u8 mode  = 0;// Ð¡³µÄ£Ê½£º1Ò£¿Ø  2.Ñ­¼£±ÜÕÏ

u8  One_Ms_Timing = 0 ;		// 1msÈ«¾ÖÊ±»ù

u8 F_100MS = 0 ;			// 100ms±êÖ¾Î»

u8 F_500MS = 0;// 500ms±êÖ¾Î»

u8 speed_duty = 80;    //pwmµÄÕ¼¿Õ±È

u8 servo_deg = 0;//¶æ»ú½Ç¶È

char inputStringCopy[20] = {0}; //½ÓÊÜ×Ö·û´®±¸·Ý


char dirString[10] = {0}; //·½Ïò×Ö·û´®



MENU_STRUCT menu_default;
MENU_STRUCT menu_phone;
MENU_STRUCT menu_auto;

//------------------------------------------------------------------

// È«¾Öº¯Êý
//------------------------------------------------------------------

void serial_data_parse(void);//´®¿Ú½âÎöº¯Êý
void setMenuReDisplay(MENU_STRUCT *menu);
void display_task(void);//ÏÔÊ¾´¦ÀíÈÎÎñ
unsigned int str_to_num(char* dig);

//ºóÌ¨ÈÎÎñ
void back_task(void)
{
    // 1msµ÷¶È
    //------------------------------------------------------------------
    if( One_Ms_Timing == 1 )
    {
        One_Ms_Timing = 0 ;

        // °´¼ü¼ì²â
        //-------------------
        KEY_Scan();

        //
        //--------------------------------------
        ADC1_Value_average();	// »ñÈ¡ADCÆ½¾ùÖµ


        //Ãù½Ð¿ØÖÆ
        BEEP_Loop();

    }
    //------------------------------------------------------------------
}

//------------------------------------------------------------------

//ÏÔÊ¾´¦ÀíÈÎÎñ
void display_task(void)
{
    switch(mode) {
    case DEFAULT_MODE:
        /***********************************´ý»úÄ£Ê½¿ªÊ¼********************************************/
        //ÆÁÄ»ÏÔÊ¾
        if(menu_default.lock1 == 0) {
            menu_default.lock1 = 1;
            OLED_ShowString(0,0,"            ",16);
            OLED_ShowString(0,0,"DEFAULT_MODE",16);
        }
        /***********************************´ý»úÄ£Ê½½áÊø********************************************/
        break;
    case PHONE_MODE:
        /***********************************Ò£¿ØÄ£Ê½¿ªÊ¼********************************************/
        //Ä£Ê½ÏÔÊ¾
        if(menu_phone.lock1 == 0) {
            menu_phone.lock1 = 1;
            OLED_ShowString(0,0,"             ",16);
            OLED_ShowString(0,0,"PHONE_MODE",16);
        }
        //´®¿ÚÊÕµ½µÄÓÐÐ§Êý¾ÝÏÔÊ¾
        if(menu_phone.lock2 == 0) {
            menu_phone.lock2 = 1;
            OLED_ShowString(0,2,"                ",16);
            OLED_ShowString(0,2,inputStringCopy,8);
        }

        //½âÎöºóµÄÖ¸ÁîÎª
        //´®¿ÚÊÕµ½µÄÓÐÐ§Êý¾ÝÏÔÊ¾
        if(menu_phone.lock3 == 0) {
            menu_phone.lock3 = 1;
            OLED_ShowString(0,4,"                ",8);
            OLED_ShowString(0,4,dirString,8);
            memset(dirString,0,sizeof(dirString));
        }


        /***********************************Ò£¿ØÄ£Ê½½áÊø********************************************/
        break;
    case AUTO_MODE:
        /***********************************×Ô¶¯Ä£Ê½¿ªÊ¼********************************************/
        //ÆÁÄ»ÏÔÊ¾
        if(menu_auto.lock1 == 0) {
            menu_auto.lock1 = 1;
            OLED_ShowString(0,0,"             ",16);
            OLED_ShowString(0,0,"AUTO_MODE",16);
        }
        /***********************************×Ô¶¯Ä£Ê½½áÊø********************************************/
        break;
    default:
        break;
    }




//    //ÉÏ
}


//°´¼ü´¦ÀíÈÎÎñ
void key_task(void)
{
    switch(mode) {
    case DEFAULT_MODE:
        /***********************************´ý»úÄ£Ê½¿ªÊ¼********************************************/

        switch(keyNum) {
        case 1://ÉÏ
            mode = PHONE_MODE;
            printf("½øÈë Ò£¿ØÄ£Ê½\r\n");
            setMenuReDisplay(&menu_phone);
            OLED_Clear(); 				// ÇåÆÁ
            keyNum = 0;
            break;
        case 2://ÖÐ



            keyNum = 0;
            break;
        case 3://ÏÂ
            mode = AUTO_MODE;
            printf("½øÈë ×Ô¶¯Ä£Ê½\r\n");
            OLED_Clear(); 				// ÇåÆÁ
            setMenuReDisplay(&menu_auto);
            keyNum = 0;
            break;
        case 5://ÖÐ¼ü³¤°´
            mode = DEFAULT_MODE;
            printf("½øÈë ´ý»úÄ£Ê½\r\n");
            setMenuReDisplay(&menu_default);
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************´ý»úÄ£Ê½½áÊø********************************************/
        break;
    case PHONE_MODE:
        /***********************************Ò£¿ØÄ£Ê½¿ªÊ¼********************************************/

        switch(keyNum) {
        case 1://ÉÏ
            LED = !LED;

            keyNum = 0;
            break;
        case 2://ÖÐ


            keyNum = 0;
            break;
        case 3://ÏÂ



            keyNum = 0;
            break;
        case 5://ÖÐ¼ü³¤°´
            mode = DEFAULT_MODE;
            printf("½øÈë ´ý»úÄ£Ê½\r\n");
            setMenuReDisplay(&menu_default);
            OLED_Clear(); 				// ÇåÆÁ
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************Ò£¿ØÄ£Ê½½áÊø********************************************/
        break;
    case AUTO_MODE:
        /***********************************×Ô¶¯Ä£Ê½¿ªÊ¼********************************************/
        switch(keyNum) {
        case 1://ÉÏ
            LED = !LED;

            keyNum = 0;
            break;
        case 2://ÖÐ


            keyNum = 0;
            break;
        case 3://ÏÂ


            keyNum = 0;
            break;
        case 5://ÖÐ¼ü³¤°´
            mode = DEFAULT_MODE;
            printf("½øÈë ´ý»úÄ£Ê½\r\n");
            setMenuReDisplay(&menu_default);
            OLED_Clear(); 				// ÇåÆÁ
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************×Ô¶¯Ä£Ê½½áÊø********************************************/
        break;
    default:
        break;
    }




//    //ÉÏ
//    switch(keyNum) {
//    case 1://ÉÏ
//        LED = !LED;
//        printf("ÉÏ¼ü°´ÏÂ\r\n");

////        //¶æ»ú²âÊÔ
////        if(servo_deg<180) {
////            servo_deg+=5;
////        } else {
////            servo_deg = 180;
////        }
////        printf("¶æ»ú½Ç¶È %d -- ¸ßµçÆ½Ê±¼ä %d\r\n",servo_deg,servo_duty);
////        SERVO_Set(servo_deg);
//        keyNum = 0;
//        break;
//    case 2://ÖÐ
//        printf("ÖÐ¼ü°´ÏÂ\r\n");
//        switch(mode) {
//        case DEFAULT_MODE:

//            break;
//        case PHONE_MODE:
//											/***********************************Ò£¿ØÄ£Ê½¿ªÊ¼********************************************/
//						mode = PHONE_MODE;
//            printf("½øÈë Ò£¿ØÄ£Ê½\r\n");
//				/***********************************Ò£¿ØÄ£Ê½½áÊø********************************************/
//						/***********************************Ò£¿ØÄ£Ê½¿ªÊ¼********************************************/
//				/***********************************Ò£¿ØÄ£Ê½½áÊø********************************************/
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
//    case 3://ÏÂ
//        printf("ÏÂ¼ü°´ÏÂ\r\n");
//        switch(mode) {
//        case DEFAULT_MODE:
//            mode = PHONE_MODE;
//            printf("Ñ­¼£±ÜÕÏ Ä£Ê½\r\n");
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
////        printf("¶æ»ú½Ç¶È %d -- ¸ßµçÆ½Ê±¼ä %d\r\n",servo_deg,servo_duty);
////        SERVO_Set(servo_deg);
//        keyNum = 0;
//        break;
//    default:
//        break;
//    }

}
//ÉèÖÃ²Ëµ¥ÖØË¢ÆÁÄ»±êÖ¾
void setMenuReDisplay(MENU_STRUCT *menu) {
    menu->lock1 = 0;
    menu->lock2 = 0;
    menu->lock3 = 0;
    menu->lock4 = 0;
    menu->lock5 = 0;
}

//ÊÖ»úÒ£¿ØÄ£Ê½ÈÎÎñ
void mode_remote_control_task(void)
{
    if(newLineReceived == 1) {
        printf("ÊÕµ½µÄÓÐÐ§ÃüÁîÎª£º%s\r\n",inputString);
        memcpy(inputStringCopy,inputString,20);//ÔÝÊ±Ö»ÏÔÊ¾Ç°20¸ö×Ö·û
        menu_phone.lock2 = 0;//Í¨ÖªË¢ÐÂ
        memset(inputString,0,sizeof(inputString));
        serial_data_parse();
        newLineReceived = 0;
    }
}
//Ñ­¼£±ÜÕÏÄ£Ê½ÈÎÎñ
void mode_track_obstacle_task(void)
{
    // 500msµ÷¶È ³¬Éù²¨¼ì²â
    //--------------------------------
    if(F_500MS == 1) {
        int l= 0;
        F_500MS = 0;
        l = get_distance();
        //printf("³¬Éù²¨Ì½²â¾àÀëÎª£º%d cm\r\n",l);

    }
}



//------------------------------------------------------------------
int main(void)
{
    // ±äÁ¿¶¨Òå
    //------------------------------------------------------------


    //------------------------------------------------------------


    // ×¢£º³ÌÐòÖÐÊ¹ÓÃÖÐ¶ÏÊ±£¬NVIC·Ö×éÉèÖÃÓ¦¾¡Á¿Î»ÓÚ³ÌÐòÆðÊ¼´¦£¬²¢ÇÒÔÚÉèÖÃºó¾¡Á¿²»ÒªÔÙ¸ü¸ÄNVIC·Ö×é
    //------------------------------------------------------------------------------------------
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //NVIC·Ö×é2£º2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶

    delay_Init();				// ÑÓÊ±³õÊ¼»¯£¨×¢£ºµ÷ÓÃÑÓÊ±º¯ÊýÖ®Ç°£¬±ØÐëÏÈµ÷ÓÃdelay_Init()½«SysTick³õÊ¼»¯£©

    uart_init(9600);       //´®¿Ú³õÊ¼»¯

    uart3_init(115200);            //´®¿Ú3³õÊ¼»¯

    YUYIN_Init();            //ÓïÒôÄ£¿é³õÊ¼»¯
    YUYIN_ON(1);



    LED_Init() ;				// ³õÊ¼»¯LEDÓ²¼þ½Ó¿Ú

    TIM3_Time_Init();			// ¶¨Ê±Æ÷3³õÊ¼»¯

    TIM4_PWM_Init();         //PWM³õÊ¼»¯

    KEY_Init() ;				// ³õÊ¼»¯°´¼üÓ²¼þ½Ó¿Ú

    DMA_Init_JX();				// DMA³õÊ¼»¯£¨°áÔËADCÊý¾Ý£©

    ADC1_Rocker_Init();			// ADC³õÊ¼»¯£¨Ò¡¸Ë£©

    BEEP_Init();            //·äÃùÆ÷³õÊ¼»¯

    UlTRA_Init();          //³¬Éù²¨³õÊ¼»¯

    SERVO_Init();          //¶æ»ú³õÊ¼»¯




    // ³õÊ¼»¯OLED
    //-----------------------------------------
    OLED_Init();				// ³õÊ¼»¯OLED
    OLED_Clear(); 				// ÇåÆÁ


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
        //ºóÌ¨ÈÎÎñ
        back_task();

        //°´¼ü´¦ÀíÈÎÎ
        key_task();

        //ÏÔÊ¾´¦ÀíÈÎÎñ
        display_task();

        //ÊÖ»úÒ£¿ØÄ£Ê½ÈÎÎñ
        mode_remote_control_task();

        //Ñ­¼£±ÜÕÏÄ£Ê½ÈÎÎñ
        mode_track_obstacle_task();


        // 100µ÷¶È
        //--------------------------------
        if( F_100MS == 1 )
        {
            F_100MS = 0;

            // 	oledÏÔÊ¾
            //-----------------------------------------------------
//            OLED_ShowNum(0,0,LEFT_TRA_ADC,3,16);
//            OLED_ShowNum(0,2,CENTER_TRA_ADC,3,16);
//            OLED_ShowNum(0,4,RIGHT_TRA_ADC,3,16);
//            OLED_ShowNum(0,6,BAT_VAL,4,16);


            //´®¿ÚÊä³ö
            //-----------------------------------------------------
            //printf("track adc value is : %d -- %d -- %d\r\n",LEFT_TRA_ADC,CENTER_TRA_ADC,RIGHT_TRA_ADC);
        }



    }
}



// ´®¿Ú»ØÏÔ£¬²âÊÔÉãÏñÍ·ÃüÁî½âÂë
//--------------------------------

//        if(USART_RX_STA&0x8000) {
//            printf("ÊÕµ½µÄÓÐÐ§ÃüÁîÎª£º%s\r\n",USART_RX_BUF);
//            memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
//            USART_RX_STA = 0;
//        }
//
//RF2G4_Send_Data[12] = (u8)(AV_ADC_Channel1_Sample / 16);

//´®¿ÚÊý¾Ý½âÎö
void serial_data_parse(void)
{

    //½âÎöÊÖ»ú·¢À´µÄÍ¨ÓÃÐ­ÒéÖ¸Áî£¬²¢Ö´ÐÐÏàÓ¦µÄ¶¯×÷
    //$4WD,PTZ180#

    //if (inputStringCopy.indexOf("4WD") > 0)
    if(strstr(inputStringCopy,"4WD"))
    {
        //½âÎöÊÖ»ú·¢À´µÄ¶æ»úÔÆÌ¨µÄ¿ØÖÆÖ¸Áî²¢Ö´ÐÐ¶æ»úÐý×ª
        //Èç$4WD,PTZ180#¶æ»ú×ª¶¯180¶È
        //if (inputStringCopy.indexOf("PTZ") > 0)
        if(strstr(inputStringCopy,"PTZ")> 0)
        {
            //$4WD,PTZ69#
            char val[10] = {0};


            char * start;
            char * end;
            char aa = 0;
            start= strchr(inputStringCopy,'Z');
            start++;//´ÓÓÐÐ§Êý×Ö¿ªÊ¼Ëã
            end = strchr(inputStringCopy,'#');
            if(end>start) {
                aa = end-start;
                memcpy(val,start,aa);
                printf("¾ßÓÐPTZÖ¸Áî\r\n");
                printf("ÓÐÐ§¶æ»ú×Ö·ûÊÇ£º%s\r\n",val);
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
        //$1,0,0,0,0,0,0,0,0,0#    Ð¡³µÇ°½ø

        //¶¯×÷ÅÐ¶Ï

        switch(inputStringCopy[1]) {
        case '0': //Í£Ö¹
            memcpy(dirString,"stop",sizeof("stop"));
            menu_phone.lock3 = 0;
            Car_Stop(0);
            break;
        case '1'://Ç°½ø
            memcpy(dirString,"go",sizeof("go"));
            menu_phone.lock3 = 0;
            Car_forward(speed_duty);
            break;
        case '2'://ºóÍË
            memcpy(dirString,"back",sizeof("back"));
            menu_phone.lock3 = 0;
            Car_backward(speed_duty);
            break;
        case '3'://×ó×ª
            memcpy(dirString,"left",sizeof("left"));
            menu_phone.lock3 = 0;
            Car_Turn_Left(speed_duty);
            break;
        case '4'://ÓÒ×ª
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


