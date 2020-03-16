#include "spi.h"

// SPI2初始化
//---------------------------------------------------------------------------------------------------------------
void SPI2_Init_JX(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );				// GPIOB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );				// SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  					// PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);								// 初始化GPIOB
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  			// PB13/14/15上拉

	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						// SPI主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					// 发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							// 时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						// 数据捕获于第1个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							// NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	// 定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					// 数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;							// CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);
 
	SPI_Cmd(SPI2, ENABLE); 		// 使能SPI外设
	
	SPI2_ReadWriteByte(0xFF);	// 启动传输		 
}   
//---------------------------------------------------------------------------------------------------------------


// 设置SPI速度
//---------------------------------------------------
// SPI_DivideFrequency_2	2分频： 18MHz
// SPI_DivideFrequency_4	4分频： 9MHz
// SPI_DivideFrequency_8	8分频： 4.5MHz
// SPI_DivideFrequency_16	16分频：2.25MHz
void SPI2_SetSpeed(u8 SPI_DivideFrequency)
{
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_DivideFrequency;	//设置SPI2速度 
	//SPI_Cmd(SPI2,ENABLE); 
} 
//---------------------------------------------------


// SPI读写一个字节
// TxData:要写入的字节
// 返回值:读取到的字节
//-----------------------------------------------------------------------
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 TxWait = 0;
	u8 RxWait = 0;
	
	// 等待发送缓存为空
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		TxWait++;
		if(TxWait>250)	// 等待时间过长则放弃本次读写
		return 0;
	}	

	SPI_I2S_SendData(SPI2, TxData); // SPI2写一个字节
	
	// 等待接收缓存为空
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		RxWait++;
		if(RxWait>250)	// 等待时间过长则放弃本次读写
		return 0;
	}	  	
	
	return SPI_I2S_ReceiveData(SPI2); // 将读到的字节返回					    
}
//-----------------------------------------------------------------------
