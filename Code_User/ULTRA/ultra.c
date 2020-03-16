#include "ULTRA.h"
#include "delay.h"
#include "usart.h"
 u32 us100HcCount= 0;
//��������ʼ������
void UlTRA_Init(void)
{

//	A11:HCSR04_TRIG; A8:ECHO
    GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

//IO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//A11:HCSR04_TRIG; � � � //??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//�������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_8;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���븡��
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);


    
    //��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    

		//��ʼ����ʱ��(�����ʱ�����ø�������ģ��Ͷ��)
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period = (100-1);
   //�Զ�����ֵ��1000������Ϊ1/1Mhz/100 = 100us
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1);
    //��Ƶϵ��72��Ƶ��Ϊ1Mhz
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		//�����ж�
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
   

		//�ж����ȼ�����
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2,ENABLE);
}

//������ʱ��2
static void OpenTimerForHc(void)
{
    //TIM_SetCounter(TIM2,0);
    us100HcCount = 0;
    //TIM_Cmd(TIM2, ENABLE);
}
//�رն�ʱ��2
static void CloseTimerForHc(void)
{
    //TIM_Cmd(TIM2, DISABLE);

}


//��ȡ������ģ��ش��ߵ�ƽʱ��
u32 GetEchoTimer(void)
{
    u32 t = 0;
    t = us100HcCount*100;//100usתus
    t += TIM_GetCounter(TIM2);//�õ���us
    TIM2->CNT = 0;//��ն�ʱ��6�ļ�����
    return t;
}

//��ȡ���뺯�����������ã���Ҫ�Ż�
//������ֵΪ-1ʱ��ʾ��̽����볬��,������-1ʱ������巵�صľ���
//ע�⣺���ͺ����پ�����Ϊ4cm������4cm�Ͳ�׼ȷ��
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
    t = GetEchoTimer(); //��ȡus��

    //���볬���ж�
    if(t>66000) { //����ģ�鷵��ʱ�䳬��66ms˵���Ѿ�����̽������ˣ����Բ��ü��ֱ�ӷ���
        return -1;
    }

    length = ((float)t*17/1000.0);//cm


    return length;
}
