#include "key.h"
#include "beep.h"

// 按键按下计时
// -------------------------
u8 KEY_UP_Cnt = 0;
u8 KEY_DOWN_Cnt = 0;
u8 KEY_OK_Cnt = 0;


// -------------------------

// 3个按键的状态：按下为1，松开为0
//-------------------------------
u8 F_KEY_Down[3] = { 0 };
//-------------------------------



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


}




// 按键扫描函数
//------------------------------------------
void RF2G4_KEY_Scan(void)
{


    // KEY_UP
    //------------------------------------
    if(KEY_UP_in)
    {
        KEY_UP_Cnt = 0;
        F_KEY_Down[KEY_UP] = 0;
    }
    else
    {   KEY_UP_Cnt ++ ;
        if( KEY_UP_Cnt>LIMIT_TIME )
        {
            F_KEY_Down[KEY_UP] = 1;
            BEEP_ON(10);
        }
    }
    //------------------------------------

    // KEY_DOWN
    //------------------------------------
    if(KEY_DOWN_in)
    {
        KEY_DOWN_Cnt = 0;
        F_KEY_Down[KEY_DOWN] = 0;
    }
    else
    {   KEY_DOWN_Cnt ++ ;
        if( KEY_DOWN_Cnt>LIMIT_TIME )
        {
            F_KEY_Down[KEY_DOWN] = 1;
            BEEP_ON(10);
        }
    }
    //------------------------------------

    // KEY_OK
    //------------------------------------
    if(KEY_OK_in)
    {
        KEY_OK_Cnt = 0;
        F_KEY_Down[KEY_OK] = 0;
    }
    else
    {   KEY_OK_Cnt ++ ;
        if( KEY_OK_Cnt>LIMIT_TIME )
        {
            F_KEY_Down[KEY_OK] = 1;
            BEEP_ON(10);
        }
    }
    //------------------------------------


}
//------------------------------------------

