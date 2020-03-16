#include "adc.h"


//------------------------------------------------------------------------------
u16 ADC1_Value[C_Channel_Sample][C_ADC_Channel] = { 0 };	// DMA�������ݴ�Ŵ�

u8  C_ADC_Sample = 0 ;				// ADC��������

u32 AM_ADC_Channel1_Sample = 0 ;	// ADCͨ��1�����ܼ�
u32 AM_ADC_Channel2_Sample = 0 ;	// ADCͨ��3�����ܼ�
u32 AM_ADC_Channel3_Sample = 0 ;	// ADCͨ��1�����ܼ�
u32 AM_ADC_Channel6_Sample = 0 ;	// ADCͨ��3�����ܼ�

u16 AV_ADC_Channel1_Sample = 2047 ;	// ADC_CH1ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
u16 AV_ADC_Channel2_Sample = 2047 ;	// ADC_CH3ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
u16 AV_ADC_Channel3_Sample = 2047 ;	// ADC_CH1ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
u16 AV_ADC_Channel6_Sample = 2047 ;	// ADC_CH3ƽ��ֵ�����ó�ֵΪ�м�λ�ã�
//------------------------------------------------------------------------------


// ѭ������������ADC��ADC��ʼ��
//-----------------------------------------------------------------------------------------------
// ADC1_CH1---PA1
// ADC1_CH3---PA2
// ADC1_CH3---PA3
// ADC1_CH3---PA6
void ADC1_Rocker_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);	// ʹ��PA�˿ں�ADC1ʱ��


    // ģ�����룺PA1��PA2��PA3��PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			// ģ����������
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// ����ģʽ�������ö˿��ٶ�
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    // ����ADC1_CH1��ADC1_CH2��ADC1_CH3��ADC1_CH6
    // ADC����������ADC��ʱ��Ƶ��<=14MHz && ADC����Ƶ��<=1MHz
    // ADC����ת������(����>=1us) = ����������(1.5��239.5) + 12.5(�̶�ת������)�� / ADCʱ��Ƶ��
    // ����ADC��ʱ��Ƶ��==12MHz����������==1.5��ADCת�����ڡ�1.17us
    //---------------------------------------------------------------------------------------------------------------------
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);						// ADCʱ��6��Ƶ��72M/6=12M

    ADC_DeInit(ADC1);  										// ��λADC1

    // ����ADC1�Ĺ���ģʽ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		// ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;			// "ɨ��ת��ģʽ"ʹ�ܡ�����ADC�����ڶ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		// "����ת��ģʽ"ʧ�ܡ�����ADC�����ڵ���ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		// ת�������������Ҳ��������Ϊ�ⲿ������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	// ADC���ݶ��뷽ʽ���Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = C_ADC_Channel;		// ����˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);						// ��ʼ��ADC1


    // Ϊ���ܹ���ȷ������ÿһ�� ADC ͨ�����û��ڵ��� ADC_Init()֮��
    // �������ADC_xxxChannelConfig()������ÿ����ʹ��ͨ����ת������Ͳ���ʱ�䡣
    //--------------------------------------------------------------------
    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);	// ������ADC1,Ҫ���õ�ADCͨ�����,���������˳��,��������
    // �˺��������õ���"ADC1"��ͨ��"1"�����ڹ������еĲ���˳����"1"������������"ADC_SampleTime_239Cycles5"


    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
    // �˺��������õ���"ADC1"��ͨ��"2"�����ڹ������еĲ���˳����"2"������������"ADC_SampleTime_239Cycles5"
    //---------------------------------------------------------------------------------------------------------------------


    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);
    // �˺��������õ���"ADC1"��ͨ��"3"�����ڹ������еĲ���˳����"3"������������"ADC_SampleTime_239Cycles5"
    //---------------------------------------------------------------------------------------------------------------------

    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_6, 4, ADC_SampleTime_239Cycles5);
    // �˺��������õ���"ADC1"��ͨ��"6"�����ڹ������еĲ���˳����"4"������������"ADC_SampleTime_239Cycles5"
    //---------------------------------------------------------------------------------------------------------------------


    /*
    //ADC1���ж�NVIC����
    //---------------------------------------------------------------------------------
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;  			// ADC1�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	// ��ռ���ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		// �����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// ʹ��ADC1_2_IRQn�ж�
    NVIC_Init(&NVIC_InitStructure);  							// ��ʼ��NVIC�Ĵ���

    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE ); 						// ����ADC1��EOC�ж�
    //---------------------------------------------------------------------------------
    */


    // ����ADC��DMA֧��
    ADC_DMACmd(ADC1, ENABLE);					// Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ���


    ADC_Cmd(ADC1, ENABLE);						// ʹ��ADC1


    ADC_ResetCalibration(ADC1);					// ʹ��ADC1��λУ׼

    while(ADC_GetResetCalibrationStatus(ADC1));	// �ȴ�ADC1��λУ׼����

    ADC_StartCalibration(ADC1);	 				// ����ADC1У׼

    while(ADC_GetCalibrationStatus(ADC1));	 	// �ȴ�ADC1У׼����

}
//-----------------------------------------------------------------------------------------------

// ��ȡADCͨ��1/3��ƽ��ֵ
//-----------------------------------------------------------------------------------------------
void ADC1_Value_average(void)
{
    // ҡ�˼��
    //------------------------------------------------------------------------
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);					// ���ʹ��ADC1ת��

    // ת����ɺ�DMA�Զ������ݰ��˵�ָ��λ��
    //while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));		// �ȴ�ת������

    AM_ADC_Channel1_Sample += ADC1_Value[C_ADC_Sample][0];	// �õ�

    AM_ADC_Channel2_Sample += ADC1_Value[C_ADC_Sample][1];	// �õ�

    AM_ADC_Channel3_Sample += ADC1_Value[C_ADC_Sample][2];

    AM_ADC_Channel6_Sample += ADC1_Value[C_ADC_Sample][3];

    C_ADC_Sample++ ;	// ADCʵ�ʲ�������

    // �ж��Ƿ��Ѿ�����10��
    if(C_ADC_Sample >= C_Channel_Sample)
    {
        C_ADC_Sample = 0;

        AV_ADC_Channel1_Sample = AM_ADC_Channel1_Sample / C_Channel_Sample;		// ��ͨ��1��ƽ��ֵ

        AM_ADC_Channel1_Sample = 0 ;


        AV_ADC_Channel3_Sample = AM_ADC_Channel3_Sample / C_Channel_Sample;		// ��ͨ��3��ƽ��ֵ

        AM_ADC_Channel3_Sample = 0 ;
			

        AV_ADC_Channel2_Sample = AM_ADC_Channel2_Sample / C_Channel_Sample;		// ��ͨ��2��ƽ��ֵ

        AM_ADC_Channel2_Sample = 0 ;


        AV_ADC_Channel6_Sample = AM_ADC_Channel6_Sample / C_Channel_Sample;		// ��ͨ��6��ƽ��ֵ

        AM_ADC_Channel6_Sample = 0 ;
    }

}
//-----------------------------------------------------------------------------------------------
