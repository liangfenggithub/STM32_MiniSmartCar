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
#include "string.h"

// 宏定义
//------------------------------------------------------------------
#define DEFAULT_MODE 0
#define PHONE_MODE 1
#define AUTO_MODE 2



//结构体定义
//------------------------------------------------------------------
//菜单显示结构体，防止屏幕重复刷新
typedef struct
{
    uint8_t lock1;//位置1，下同
    uint8_t lock2;//位置1，下同
    uint8_t lock3;//位置1，下同
    uint8_t lock4;//位置1，下同
    uint8_t lock5;//位置1，下同

} MENU_STRUCT;


// 全局变量
//------------------------------------------------------------------
u16 C_TIM2_IT_Update = 0 ;	// TIM3的定时计次


u8 mode  = 0;// 小车模式：1遥控  2.循迹避障

u8  One_Ms_Timing = 0 ;		// 1ms全局时基

u8 F_100MS = 0 ;			// 100ms标志位

u8 F_110MS = 0;// 500ms标志位

u16 F_1S = 0;// 1s标志位

u8 speed_duty = 80;    //pwm的占空比

u8 servo_deg = 0;//舵机角度

char inputStringCopy[20] = {0}; //接受字符串备份

char dirString[10] = {0}; //方向字符串

u8 left_thr = 0; //左传感器阈值

u8 middle_thr = 0;//中间传感器值

u8 right_thr = 0;//右传感器阈值

u8 auto_mode_run_flag = 0;//自动模式下启动标志位

char run_str[16] = {0};//运行指示字符串

u16 left_real_speed = 0;//左侧实际速度，厘米每秒

u16 right_real_speed = 0;//右侧实际速度，厘米每秒

u16 all_distance = 0;//总里程 厘米

MENU_STRUCT menu_default;
MENU_STRUCT menu_phone;
MENU_STRUCT menu_auto;

//------------------------------------------------------------------

// 全局函数
//------------------------------------------------------------------

void serial_data_parse(void);//串口解析函数
void setMenuReDisplay(MENU_STRUCT *menu);
void display_task(void);//显示处理任务
unsigned int str_to_num(char* dig);

//后台任务
void back_task(void)
{
    // 1ms调度
    //------------------------------------------------------------------
    if( One_Ms_Timing == 1 )
    {
        One_Ms_Timing = 0 ;

        // 按键检测
        //-------------------
        KEY_Scan();

        //
        //--------------------------------------
        ADC1_Value_average();	// 获取ADC平均值


        //鸣叫控制
        BEEP_Loop();

    }
    //------------------------------------------------------------------
}

//------------------------------------------------------------------

//显示处理任务
int all_length = 0;
void display_task(void)
{
		char str[20] = {0};
    switch(mode) {
    case DEFAULT_MODE:
        /***********************************待机模式开始********************************************/
        //屏幕显示
        if(menu_default.lock1 == 0) {
            menu_default.lock1 = 1;
            OLED_ShowString(0,0,"            ",16);
            OLED_ShowString(0,0,"DEFAULT_MODE",16);
        }
        /***********************************待机模式结束********************************************/
        break;
    case PHONE_MODE:
        /***********************************遥控模式开始********************************************/
        //模式显示
        if(menu_phone.lock1 == 0) {
            menu_phone.lock1 = 1;
            OLED_ShowString(0,0,"             ",16);
            OLED_ShowString(0,0,"PHONE_MODE",16);
        }
        //串口收到的有效数据显示
        if(menu_phone.lock2 == 0) {
            menu_phone.lock2 = 1;
            OLED_ShowString(0,2,"                ",16);
            OLED_ShowString(0,2,inputStringCopy,8);
        }

        //解析后的指令为
        //串口收到的有效数据显示
        if(menu_phone.lock3 == 0) {
            menu_phone.lock3 = 1;
            OLED_ShowString(0,4,"                ",8);
            OLED_ShowString(0,4,dirString,8);
            memset(dirString,0,sizeof(dirString));
        }

				
				if( F_1S == 1 )
        {
            F_1S = 0;
						//计算总里程
						all_length = all_length+ ((left_real_speed+right_real_speed)/2);//总距离以左右侧速度平均值为基准
					
						printf("左侧速度为 %d 每秒\r\n",left_real_speed);
						printf("右侧速度为 %d 每秒\r\n",right_real_speed);	
						printf("总里程为 %d \r\n",all_length);
						
						//屏幕显示
						memset(str,0,sizeof(str));
						sprintf(str,"total: %d cm",all_length);
						OLED_ShowString(0,6,"                ",8);
            OLED_ShowString(0,6,str,8);
					
					
						//清零时速
						left_real_speed = 0;
						right_real_speed = 0;
						
					
				}
				
        /***********************************遥控模式结束********************************************/
        break;
    case AUTO_MODE:
        /***********************************自动模式开始********************************************/
        //屏幕显示
        if(menu_auto.lock1 == 0) {
            menu_auto.lock1 = 1;
            OLED_ShowString(0,0,"             ",16);
            OLED_ShowString(0,0,"AUTO_MODE",16);
        }

        //状态显示
        if(menu_auto.lock4 == 0) {
            menu_auto.lock4 = 1;
            OLED_ShowString(0,6,"             ",16);
            OLED_ShowString(0,6,run_str,16);
        }

        // 100ms调度
        //--------------------------------
        if( F_100MS == 1 )
        {
            F_100MS = 0;

            // 	oled显示
            //-----------------------------------------------------
            OLED_ShowNum(0,2,LEFT_TRA_ADC,3,16);//左传感器值
            OLED_ShowNum(40,2,CENTER_TRA_ADC,3,16);//中传感器值
            OLED_ShowNum(80,2,RIGHT_TRA_ADC,3,16);//右传感器值

            OLED_ShowNum(0,4,left_thr,3,16);//左传感器阈值
            OLED_ShowNum(80,4,right_thr,3,16);//右传感器阈值
            //OLED_ShowNum(0,6,BAT_VAL,4,16);
            //串口输出
            //-----------------------------------------------------
            printf("track adc value is : %d -- %d -- %d\r\n",LEFT_TRA_ADC,CENTER_TRA_ADC,RIGHT_TRA_ADC);
        }





        /***********************************自动模式结束********************************************/
        break;
    default:
        break;
    }




//    //上
}


//按键处理任务
void key_task(void)
{
    switch(mode) {
    case DEFAULT_MODE:
        /***********************************待机模式开始********************************************/

        switch(keyNum) {
        case 1://上
            mode = PHONE_MODE;
            printf("进入 遥控模式\r\n");
            setMenuReDisplay(&menu_phone);
            OLED_Clear(); 				// 清屏
            keyNum = 0;
            break;
        case 2://中



            keyNum = 0;
            break;
        case 3://下
            mode = AUTO_MODE;
            printf("进入 自动模式\r\n");
            OLED_Clear(); 				// 清屏
            setMenuReDisplay(&menu_auto);
            auto_mode_run_flag = 0;//默认为等待设置
            memcpy(run_str,"wait set...",sizeof("wait set..."));
            keyNum = 0;
            break;
        case 5://中键长按
            mode = DEFAULT_MODE;
            printf("进入 待机模式\r\n");
            setMenuReDisplay(&menu_default);
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************待机模式结束********************************************/
        break;
    case PHONE_MODE:
        /***********************************遥控模式开始********************************************/

        switch(keyNum) {
        case 1://上
            LED = !LED;

            keyNum = 0;
            break;
        case 2://中


            keyNum = 0;
            break;
        case 3://下



            keyNum = 0;
            break;
        case 5://中键长按
            mode = DEFAULT_MODE;
            printf("进入 待机模式\r\n");
            setMenuReDisplay(&menu_default);
            OLED_Clear(); 				// 清屏
						YUYIN_OFF;
                Car_Stop(0);
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************遥控模式结束********************************************/
        break;
    case AUTO_MODE:
        /***********************************自动模式开始********************************************/
        switch(keyNum) {
        case 1://上
            LED = !LED;

            keyNum = 0;
            break;
        case 2://中
            if(auto_mode_run_flag == 0) {
                //如果是等待运行模式下，按下中键就开始运行
                printf("开始循迹\r\n");
                auto_mode_run_flag = 1;
                memcpy(run_str,"running...",sizeof("running..."));

            } else {
                //如果是运行模式下，按下中键就停止运行
                auto_mode_run_flag = 0;
                printf("停止循迹\r\n");
                memcpy(run_str,"wait set...",sizeof("wait set..."));
            }

            menu_auto.lock4 = 0;
            keyNum = 0;
            break;
        case 3://下
            left_thr = CENTER_TRA_ADC;
            right_thr = CENTER_TRA_ADC;
            printf("设置左右阈值为 %d\r\n",CENTER_TRA_ADC);
            keyNum = 0;
            break;
        case 5://中键长按
            mode = DEFAULT_MODE;
            printf("进入 待机模式\r\n");
            setMenuReDisplay(&menu_default);
            OLED_Clear(); 				// 清屏
				YUYIN_OFF;
                Car_Stop(0);
            keyNum = 0;
            break;
        default:
            break;
        }
        /***********************************自动模式结束********************************************/
        break;
    default:
        break;
    }


}
//设置菜单重刷屏幕标志
void setMenuReDisplay(MENU_STRUCT *menu) {
    menu->lock1 = 0;
    menu->lock2 = 0;
    menu->lock3 = 0;
    menu->lock4 = 0;
    menu->lock5 = 0;
}

//手机遥控模式任务
void mode_remote_control_task(void)
{
    if(mode != PHONE_MODE) return;
    if(newLineReceived == 1) {
        printf("收到的有效命令为：%s\r\n",inputString);
        memcpy(inputStringCopy,inputString,20);//暂时只显示前20个字符
        menu_phone.lock2 = 0;//通知刷新
        memset(inputString,0,sizeof(inputString));
        serial_data_parse();
        newLineReceived = 0;
    }
}
//循迹避障模式任务
int l= 0;
void mode_track_obstacle_task(void)
{
    if(mode != AUTO_MODE) return;
    // 500ms调度 超声波检测避障
    //--------------------------------
    if(F_110MS == 1) {

        F_110MS = 0;
        l = get_distance();
        if(l<5) l = 5;//超声波测量最小距离
        if(l>60) l= 60;//超声波测量最远距离
        printf("超声波探测距离为：%d cm\r\n",l);
    }

    //循迹
    if(auto_mode_run_flag == 1)//是否开始循迹？
    {
        //压住左线则右转
        //压住右线则左转
        //都不压则前进，如果障碍物小于设定距离则停止报警
        if(LEFT_TRA_ADC>= left_thr) {
            Car_Turn_Left(35);
            printf("向左转\r\n");
        } else if(RIGHT_TRA_ADC>= right_thr) {
            Car_Turn_Right(35);
            printf("向右转\r\n");
        } else if((LEFT_TRA_ADC <= left_thr)&& (RIGHT_TRA_ADC<= right_thr)) {
            printf("在正中心\r\n");
            if(l >= 7)//
            {
                Car_forward(speed_duty);
                YUYIN_OFF();
                printf("向前走\r\n");
            } else {
                printf("小于7厘米，开始报警\r\n");
                YUYIN_ON(2);
                Car_Stop(0);
                printf("停止\r\n");

            }
        } else if((LEFT_TRA_ADC <= left_thr)&& (RIGHT_TRA_ADC<= right_thr)) {
            //Car_forward(speed_duty);
            Car_Stop(0);
            printf("停止\r\n");
        }

    } else {
        Car_Stop(0);
        //printf("停止\r\n");
    }


}



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

    uart_init(9600);       //串口初始化

    uart3_init(115200);            //串口3初始化

    YUYIN_Init();            //语音模块初始化
    YUYIN_ON(1);



    LED_Init() ;				// 初始化LED硬件接口

    TIM3_Time_Init();			// 定时器3初始化

    TIM4_PWM_Init();         //PWM初始化

    KEY_Init() ;				// 初始化按键硬件接口

    DMA_Init_JX();				// DMA初始化（搬运ADC数据）

    ADC1_Rocker_Init();			// ADC初始化（摇杆）

    BEEP_Init();            //蜂鸣器初始化

    UlTRA_Init();          //超声波初始化

    SERVO_Init();          //舵机初始化

		EXTIX_Init();         //外部中断初始化 用于计算里程



    // 初始化OLED
    //-----------------------------------------
    OLED_Init();				// 初始化OLED
    OLED_Clear(); 				// 清屏
    OLED_ShowString(0, 0, "Smart Car",16);
    OLED_ShowString(0, 2, "   by TangHao",16);

    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1500);
    YUYIN_OFF();
    OLED_Clear(); 				// 清屏
    //-----------------------------------------
    //Car_forward(80);
    while(1)
    {
        //后台任务
        back_task();

        //按键处理任�
        key_task();

        //显示处理任务
        display_task();

        //手机遥控模式任务
        mode_remote_control_task();

        //循迹避障模式任务
        mode_track_obstacle_task();






    }
}



// 串口回显，测试摄像头命令解码
//--------------------------------

//        if(USART_RX_STA&0x8000) {
//            printf("收到的有效命令为：%s\r\n",USART_RX_BUF);
//            memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
//            USART_RX_STA = 0;
//        }
//
//RF2G4_Send_Data[12] = (u8)(AV_ADC_Channel1_Sample / 16);

//串口数据解析
void serial_data_parse(void)
{

    //解析手机发来的通用协议指令，并执行相应的动作
    //$4WD,PTZ180#

    //if (inputStringCopy.indexOf("4WD") > 0)
    if(strstr(inputStringCopy,"4WD"))
    {
        //解析手机发来的舵机云台的控制指令并执行舵机旋转
        //如$4WD,PTZ180#舵机转动180度
        //if (inputStringCopy.indexOf("PTZ") > 0)
        if(strstr(inputStringCopy,"PTZ")> 0)
        {
            //$4WD,PTZ69#
            char val[10] = {0};


            char * start;
            char * end;
            char aa = 0;
            start= strchr(inputStringCopy,'Z');
            start++;//从有效数字开始算
            end = strchr(inputStringCopy,'#');
            if(end>start) {
                aa = end-start;
                memcpy(val,start,aa);
                printf("具有PTZ指令\r\n");
                printf("有效舵机字符是：%s\r\n",val);
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
        //$1,0,0,0,0,0,0,0,0,0#    小车前进

        //动作判断

        switch(inputStringCopy[1]) {
        case '0': //停止
            memcpy(dirString,"stop",sizeof("stop"));
            menu_phone.lock3 = 0;
            Car_Stop(0);
            break;
        case '1'://前进
            memcpy(dirString,"go",sizeof("go"));
            menu_phone.lock3 = 0;
            Car_forward(speed_duty);
            break;
        case '2'://后退
            memcpy(dirString,"back",sizeof("back"));
            menu_phone.lock3 = 0;
            Car_backward(speed_duty);
            break;
        case '3'://左转
            memcpy(dirString,"left",sizeof("left"));
            menu_phone.lock3 = 0;
            Car_Turn_Left(speed_duty);
            break;
        case '4'://右转
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


