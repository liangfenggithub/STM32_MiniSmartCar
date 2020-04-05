#include "key.h"
#include "beep.h"




//-------------------------------

//结构体定义
typedef struct
{
    uint8_t lock;//按键自锁标志/
    uint16_t timeCnt;//按键去抖动延时计数器//key_up.timeCnt
    uint8_t timeCntDelay;//摁住不松手连续步进计数器
    uint8_t reTimeCntThreshold;//摁住不松手连续步进计数阈值，用于匀加速触发按键
    uint8_t shortTouchFlag;//服务于长嗯作为一个键值
} KEY_STRUCT;

KEY_STRUCT key_up;
KEY_STRUCT key_ok;
KEY_STRUCT key_down;


//变量声明
uint8_t keyNum = 0;



//函数声明


void KEY_Scan(void)//按键扫描函数 放在定时中断里
{

    if(KEY_UP_IN==1)//IO是高电平，说明按键没有被按下，
    {
        key_up.lock=0; //按键自锁标志清零
        key_up.timeCnt=0;//按键去抖动延时计数器清零，

        if(key_up.shortTouchFlag==1)  //短按触发标志
        {
            key_up.shortTouchFlag=0;
            keyNum = 1;//触发短按键值1
            BEEP_ON(30);
            printf("上键按下\r\n");

        }
    }
    else if(key_up.lock==0)//有按键按下，且是第一次被按下
    {
        key_up.timeCnt++; //累加定时中断次数
        if(key_up.timeCnt>KEYSHORTTIME)
        {
            key_up.shortTouchFlag=1;   //激活按键短按的有效标志
        }

        if(key_up.timeCnt>KEYLONGTIME)
        {
            key_up.shortTouchFlag=0;  //清除按键短按的有效标志

            key_up.timeCnt=0;
            key_up.lock=1;  //自锁按键置位,避免一直触发

            keyNum=4;    //触发1号键的长按

        }
    }



    if(KEY_OK_IN==1)//IO是高电平，说明按键没有被按下，
    {
        key_ok.lock=0; //按键自锁标志清零
        key_ok.timeCnt=0;//按键去抖动延时计数器清零，

        if(key_ok.shortTouchFlag==1)  //短按触发标志
        {
            key_ok.shortTouchFlag=0;
            keyNum = 2;//触发短按键值1
            BEEP_ON(30);
            printf("中键按下\r\n");

        }
    }
    else if(key_ok.lock==0)//有按键按下，且是第一次被按下
    {
        key_ok.timeCnt++; //累加定时中断次数
        if(key_ok.timeCnt>KEYSHORTTIME)
        {
            key_ok.shortTouchFlag=1;   //激活按键短按的有效标志
        }

        if(key_ok.timeCnt>KEYLONGTIME)
        {
            key_ok.shortTouchFlag=0;  //清除按键短按的有效标志

            key_ok.timeCnt=0;
            key_ok.lock=1;  //自锁按键置位,避免一直触发

            keyNum=5;    //触发1号键的长按
					BEEP_ON(60);
					printf("中键长按下\r\n");
        }
    }


    if(KEY_DOWN_IN==1)//IO是高电平，说明按键没有被按下，
    {
        key_down.lock=0; //按键自锁标志清零
        key_down.timeCnt=0;//按键去抖动延时计数器清零，

        if(key_down.shortTouchFlag==1)  //短按触发标志
        {
            key_down.shortTouchFlag=0;
            keyNum = 3;//触发短按键值1
            BEEP_ON(30);
            printf("下键按下\r\n");

        }
    }
    else if(key_down.lock==0)//有按键按下，且是第一次被按下
    {
        key_down.timeCnt++; //累加定时中断次数
        if(key_down.timeCnt>KEYSHORTTIME)
        {
            key_down.shortTouchFlag=1;   //激活按键短按的有效标志
        }

        if(key_down.timeCnt>KEYLONGTIME)
        {
            key_down.shortTouchFlag=0;  //清除按键短按的有效标志

            key_down.timeCnt=0;
            key_down.lock=1;  //自锁按键置位,避免一直触发

            keyNum=6;    //触发1号键的长按
        }
    }




}





// 按键初始化
//---------------------
void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;						// 定义GPIO_InitTypeDef型结构体


    // 使能GPIOB端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 使能GPIO时钟


    // 按键：UP DOWN OK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				//b12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			// 上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);					// 根据设定参数初始化


    //按键结构体初始化
    key_up.lock = 0;
    key_up.timeCnt = 0;
    key_up.shortTouchFlag = 0;

    key_ok.lock = 0;
    key_ok.timeCnt = 0;
    key_ok.shortTouchFlag = 0;

    key_down.lock = 0;
    key_down.timeCnt = 0;
    key_down.shortTouchFlag = 0;
}



//------------------------------------------

