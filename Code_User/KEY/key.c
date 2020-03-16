#include "key.h"
#include "beep.h"

// �������¼�ʱ
// -------------------------
u8 KEY_UP_Cnt = 0;
u8 KEY_DOWN_Cnt = 0;
u8 KEY_OK_Cnt = 0;


// -------------------------

// 3��������״̬������Ϊ1���ɿ�Ϊ0
//-------------------------------
u8 F_KEY_Down[3] = { 0 };
//-------------------------------



// ������ʼ��
//---------------------
void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;						// ����GPIO_InitTypeDef�ͽṹ��


    // ʹ��GPIOB�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// ʹ��GPIOʱ��


    // ������UP DOWN OK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				//b12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			// ��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);					// �����趨������ʼ��


}




// ����ɨ�躯��
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

