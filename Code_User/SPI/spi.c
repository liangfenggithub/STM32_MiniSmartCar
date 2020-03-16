#include "spi.h"

// SPI2��ʼ��
//---------------------------------------------------------------------------------------------------------------
void SPI2_Init_JX(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );				// GPIOBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );				// SPI2ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  					// PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);								// ��ʼ��GPIOB
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  			// PB13/14/15����

	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						// SPI����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					// ���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							// ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						// ���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							// NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					// ���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							// CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);
 
	SPI_Cmd(SPI2, ENABLE); 		// ʹ��SPI����
	
	SPI2_ReadWriteByte(0xFF);	// ��������		 
}   
//---------------------------------------------------------------------------------------------------------------


// ����SPI�ٶ�
//---------------------------------------------------
// SPI_DivideFrequency_2	2��Ƶ�� 18MHz
// SPI_DivideFrequency_4	4��Ƶ�� 9MHz
// SPI_DivideFrequency_8	8��Ƶ�� 4.5MHz
// SPI_DivideFrequency_16	16��Ƶ��2.25MHz
void SPI2_SetSpeed(u8 SPI_DivideFrequency)
{
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_DivideFrequency;	//����SPI2�ٶ� 
	//SPI_Cmd(SPI2,ENABLE); 
} 
//---------------------------------------------------


// SPI��дһ���ֽ�
// TxData:Ҫд����ֽ�
// ����ֵ:��ȡ�����ֽ�
//-----------------------------------------------------------------------
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 TxWait = 0;
	u8 RxWait = 0;
	
	// �ȴ����ͻ���Ϊ��
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		TxWait++;
		if(TxWait>250)	// �ȴ�ʱ�������������ζ�д
		return 0;
	}	

	SPI_I2S_SendData(SPI2, TxData); // SPI2дһ���ֽ�
	
	// �ȴ����ջ���Ϊ��
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		RxWait++;
		if(RxWait>250)	// �ȴ�ʱ�������������ζ�д
		return 0;
	}	  	
	
	return SPI_I2S_ReceiveData(SPI2); // ���������ֽڷ���					    
}
//-----------------------------------------------------------------------
