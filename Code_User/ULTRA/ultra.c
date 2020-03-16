#include "ULTRA.h"
#include "delay.h"
#include "usart.h"
 u32 us100HcCount= 0;
//³¬Éù²¨³õÊ¼»¯º¯Êı
void UlTRA_Init(void)
{

//	A11:HCSR04_TRIG; A8:ECHO
    GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

//IO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//A11:HCSR04_TRIG;       //??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_8;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//ÊäÈë¸¡¿Õ
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);


    
    //¶¨Ê±Æ÷
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    

		//³õÊ¼»¯¶¨Ê±Æ÷(Õâ¸ö¶¨Ê±Æ÷¹«ÓÃ¸ø³¬Éù²¨Ä£¿éºÍ¶æ»ú)
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period = (100-1);
   //×Ô¶¯ÖØÔØÖµ£¬1000£¬ÖÜÆÚÎª1/1Mhz/100 = 100us
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1);
    //·ÖÆµÏµÊı72£¬ÆµÂÊÎª1Mhz
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		//¿ªÆôÖĞ¶Ï
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
   

		//ÖĞ¶ÏÓÅÏÈ¼¶ÉèÖÃ
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2,ENABLE);
}

//¿ªÆô¶¨Ê±Æ÷2
static void OpenTimerForHc(void)
{
    //TIM_SetCounter(TIM2,0);
    us100HcCount = 0;
    //TIM_Cmd(TIM2, ENABLE);
}
//¹Ø±Õ¶¨Ê±Æ÷2
static void CloseTimerForHc(void)
{
    //TIM_Cmd(TIM2, DISABLE);

}


//»ñÈ¡³¬Éù²¨Ä£¿é»Ø´«¸ßµçÆ½Ê±¼ä
u32 GetEchoTimer(void)
{
    u32 t = 0;
    t = us100HcCount*100;//100us×ªus
    t += TIM_GetCounter(TIM2);//µÃµ½×Üus
    TIM2->CNT = 0;//Çå¿Õ¶¨Ê±Æ÷6µÄ¼ÆÊıÆ÷
    return t;
}

//»ñÈ¡¾àÀëº¯Êı¡¢×èÈûµ÷ÓÃ£¬ĞèÒªÓÅ»¯
//µ±·µ»ØÖµÎª-1Ê±±íÊ¾¿ÉÌ½²â¾àÀë³¬ÏŞ,µ±²»ÊÇ-1Ê±´ú±í¾ßÌå·µ»ØµÄ¾àÀë
//×¢Òâ£º´ËĞÍºÅ×îÉÙ¾àÀë¼ì²âÎª4cm£¬µÍÓÚ4cm¾Í²»×¼È·ÁË
int get_distance(void)
{
    u32 t = 0;
    int length = 0;
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
    delay_us(25);
    GPIO_ResetBits(GPIOA,GPIO_Pin_11);
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0);
    OpenTimerForHc();
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 1);
    CloseTimerForHc();
    t = GetEchoTimer(); //»ñÈ¡usÊı

    //¾àÀë³¬ÏŞÅĞ¶Ï
    if(t>66000) { //¹ºÂòÄ£¿é·µ»ØÊ±¼ä³¬¹ı66msËµÃ÷ÒÑ¾­³¬¹ıÌ½²â¾àÀëÁË£¬ËùÒÔ²»ÓÃ¼ì²âÖ±½Ó·µ»Ø
        return -1;
    }

    length = ((float)t*17/1000.0);//cm


    return length;
}
