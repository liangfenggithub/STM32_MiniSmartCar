#include "adc.h"


//------------------------------------------------------------------------------
u16 ADC1_Value[C_Channel_Sample][C_ADC_Channel] = { 0 };	// DMA搬运数据存放处

u8  C_ADC_Sample = 0 ;				// ADC采样次数

u32 AM_ADC_Channel1_Sample = 0 ;	// ADC通道1采样总计
u32 AM_ADC_Channel2_Sample = 0 ;	// ADC通道3采样总计
u32 AM_ADC_Channel3_Sample = 0 ;	// ADC通道1采样总计
u32 AM_ADC_Channel6_Sample = 0 ;	// ADC通道3采样总计

u16 AV_ADC_Channel1_Sample = 2047 ;	// ADC_CH1平均值（设置初值为中间位置）
u16 AV_ADC_Channel2_Sample = 2047 ;	// ADC_CH3平均值（设置初值为中间位置）
u16 AV_ADC_Channel3_Sample = 2047 ;	// ADC_CH1平均值（设置初值为中间位置）
u16 AV_ADC_Channel6_Sample = 2047 ;	// ADC_CH3平均值（设置初值为中间位置）
//------------------------------------------------------------------------------


// 循迹传感器与电池ADC的ADC初始化
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

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);	// 使能PA端口和ADC1时钟


    // 模拟输入：PA1、PA2、PA3、PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			// 模拟输入引脚
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// 输入模式无需设置端口速度
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    // 配置ADC1_CH1、ADC1_CH2、ADC1_CH3、ADC1_CH6
    // ADC工作条件：ADC的时钟频率<=14MHz && ADC采样频率<=1MHz
    // ADC的总转换周期(必须>=1us) = （采样周期(1.5～239.5) + 12.5(固定转换周期)） / ADC时钟频率
    // 当：ADC的时钟频率==12MHz、采样周期==1.5，ADC转换周期≈1.17us
    //---------------------------------------------------------------------------------------------------------------------
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);						// ADC时钟6分频：72M/6=12M

    ADC_DeInit(ADC1);  										// 复位ADC1

    // 设置ADC1的工作模式
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		// ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;			// "扫描转换模式"使能。即：ADC工作在多通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		// "连续转换模式"失能。即：ADC工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		// 转换由软件启动（也可以设置为外部触发）
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	// ADC数据对齐方式：右对齐
    ADC_InitStructure.ADC_NbrOfChannel = C_ADC_Channel;		// 设置顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);						// 初始化ADC1


    // 为了能够正确地配置每一个 ADC 通道，用户在调用 ADC_Init()之后，
    // 必须调用ADC_xxxChannelConfig()来配置每个所使用通道的转换次序和采样时间。
    //--------------------------------------------------------------------
    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);	// 参数：ADC1,要设置的ADC通道序号,规则组采样顺序,采样周期
    // 此函数：配置的是"ADC1"的通道"1"，它在规则组中的采样顺序是"1"，采样周期是"ADC_SampleTime_239Cycles5"


    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
    // 此函数：配置的是"ADC1"的通道"2"，它在规则组中的采样顺序是"2"，采样周期是"ADC_SampleTime_239Cycles5"
    //---------------------------------------------------------------------------------------------------------------------


    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);
    // 此函数：配置的是"ADC1"的通道"3"，它在规则组中的采样顺序是"3"，采样周期是"ADC_SampleTime_239Cycles5"
    //---------------------------------------------------------------------------------------------------------------------

    ADC_RegularChannelConfig(ADC1, N_ADC_Channel_6, 4, ADC_SampleTime_239Cycles5);
    // 此函数：配置的是"ADC1"的通道"6"，它在规则组中的采样顺序是"4"，采样周期是"ADC_SampleTime_239Cycles5"
    //---------------------------------------------------------------------------------------------------------------------


    /*
    //ADC1的中断NVIC设置
    //---------------------------------------------------------------------------------
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;  			// ADC1中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	// 抢占优先级3级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		// 子优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// 使能ADC1_2_IRQn中断
    NVIC_Init(&NVIC_InitStructure);  							// 初始化NVIC寄存器

    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE ); 						// 允许ADC1的EOC中断
    //---------------------------------------------------------------------------------
    */


    // 开启ADC的DMA支持
    ADC_DMACmd(ADC1, ENABLE);					// 要实现DMA功能，还需独立配置DMA通道等参数


    ADC_Cmd(ADC1, ENABLE);						// 使能ADC1


    ADC_ResetCalibration(ADC1);					// 使能ADC1复位校准

    while(ADC_GetResetCalibrationStatus(ADC1));	// 等待ADC1复位校准结束

    ADC_StartCalibration(ADC1);	 				// 开启ADC1校准

    while(ADC_GetCalibrationStatus(ADC1));	 	// 等待ADC1校准结束

}
//-----------------------------------------------------------------------------------------------

// 获取ADC通道1/3的平均值
//-----------------------------------------------------------------------------------------------
void ADC1_Value_average(void)
{
    // 摇杆检测
    //------------------------------------------------------------------------
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);					// 软件使能ADC1转换

    // 转换完成后，DMA自动将数据搬运到指定位置
    //while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));		// 等待转换结束

    AM_ADC_Channel1_Sample += ADC1_Value[C_ADC_Sample][0];	// 得到

    AM_ADC_Channel2_Sample += ADC1_Value[C_ADC_Sample][1];	// 得到

    AM_ADC_Channel3_Sample += ADC1_Value[C_ADC_Sample][2];

    AM_ADC_Channel6_Sample += ADC1_Value[C_ADC_Sample][3];

    C_ADC_Sample++ ;	// ADC实际采样次数

    // 判断是否已经采样10次
    if(C_ADC_Sample >= C_Channel_Sample)
    {
        C_ADC_Sample = 0;

        AV_ADC_Channel1_Sample = AM_ADC_Channel1_Sample / C_Channel_Sample;		// 求通道1的平均值

        AM_ADC_Channel1_Sample = 0 ;


        AV_ADC_Channel3_Sample = AM_ADC_Channel3_Sample / C_Channel_Sample;		// 求通道3的平均值

        AM_ADC_Channel3_Sample = 0 ;
			

        AV_ADC_Channel2_Sample = AM_ADC_Channel2_Sample / C_Channel_Sample;		// 求通道2的平均值

        AM_ADC_Channel2_Sample = 0 ;


        AV_ADC_Channel6_Sample = AM_ADC_Channel6_Sample / C_Channel_Sample;		// 求通道6的平均值

        AM_ADC_Channel6_Sample = 0 ;
    }

}
//-----------------------------------------------------------------------------------------------
