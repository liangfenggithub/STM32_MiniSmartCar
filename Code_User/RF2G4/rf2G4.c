#include "rf2G4.h"


// 2G4ͨ�ŵ�ַ
//------------------------------------------------------------------------------------------		
const u8 RF2G4_ADDR_TX[5] = { 'j', 'i', 'x', 'i', 'n' } ;		// 0X6A,0X69,0X78,0X69,0X6E
const u8 RF2G4_ADDR_RX[5] = { 'j', 'i', 'x', 'i', 'n' } ;		// ASCII��jixin
//------------------------------------------------------------------------------------------

u8 RF2G4_Send_Data[14] = { 0 };		// 2.4G�������ݻ�������
			 
u8 RF2G4_Receive_Data[14] = { 0 };	// 2.4G�������ݻ�������


// 	���߷���(RF2G4_Send_Data[14])�������14�����ݣ��ֱ�Ϊ
//--------------------------------------------------------------
// 	0  == WKUP					// 1:����	0���ɿ�
//	1  == KEY1					// 1:����	0���ɿ�
//	2  == KEY_UP				// 1:����	0���ɿ�
//	3  == KEY_DOWN				// 1:����	0���ɿ�
//	4  == KEY_LEFT				// 1:����	0���ɿ�
//	5  == KEY_RIGHT				// 1:����	0���ɿ�
//	6  == KEY_A					// 1:����	0���ɿ�
//	7  == KEY_AA				// 1:����	0���ɿ�
//	8  == KEY_B					// 1:����	0���ɿ�
//	9  == KEY_BB				// 1:����	0���ɿ�

//	10 == KEY_LeftRocker		// 1:����	0���ɿ�
//	11 == KEY_RightRocker		// 1:����	0���ɿ�

//  12 == Rocker_FORWARD_x		// ǰ��/���ˣ���λ��0��255��
//  13 == Rocker_BACK_x			// ��ת/��ת����λ��0��255��
//--------------------------------------------------------------


// RF2G4��ʼ��
//------------------------------------------------------------------------------------------
void RF2G4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	// ʹ��PB�˿�ʱ��
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12;		// PB10��PB12���� 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	// �������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						// ��ʼ��ָ��IO
	GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_12);				// PB10��PB12�����
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 				// PB11����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
						 

	SPI2_Init_JX();    		// ��ʼ��SPI	 
 
	RF2G4_CE=0; 			// ʹ��SI24R1
	
	RF2G4_CSN=1;			// SPIƬѡȡ��  
	
}
//------------------------------------------------------------------------------------------	 	 


// 2G4��SPI��д
//==========================================================================================

// SPI��SI24R1�Ĵ�����д����
// Reg 			= �Ĵ���
// Write_Value 	= д���ֵ
// return		= ״ֵ̬
//--------------------------------------------------------
u8 RF2G4_Write_Reg(u8 Reg,u8 Write_Value)
{
	u8 V_Reg;	
	
   	RF2G4_CSN = 0;                 		// ʹ��SPI����
	
  	V_Reg =SPI2_ReadWriteByte(Reg);		// ���ͼĴ����� 
	
  	SPI2_ReadWriteByte(Write_Value);	// д��Ĵ�����ֵ
	
  	RF2G4_CSN = 1;                 		// ��ֹSPI����	 
	
  	return(V_Reg);       				// ����״ֵ̬
}
//--------------------------------------------------------

// ��Ĵ�����д��ָ�����ȵ�����
// Reg		= �Ĵ���
// P_Data 	= ����ָ��
// N_Data 	= ���ݸ���
// return 	= �˴ζ�����״̬�Ĵ���ֵ
//--------------------------------------------------------
u8 RF2G4_Write_Cont(u8 Reg, u8* P_Data, u8 N_Data)
{
	u8 V_Reg = 0;
	u8 C_Write = 0;
	
 	RF2G4_CSN = 0;          			// ʹ��SPI����
	
  	V_Reg = SPI2_ReadWriteByte(Reg);	// ѡ���Ĵ���,����ȡ״ֵ̬
	
  	for(; C_Write<N_Data; C_Write++)
	{
		SPI2_ReadWriteByte(*P_Data++); 	// д������
	}
		 
  	RF2G4_CSN = 1;       				// �ر�SPI����
	
  	return V_Reg;          				// ���ض�����״ֵ̬
}
//--------------------------------------------------------



// SPI��SI24R1�Ĵ����Ķ�����
// Reg 		= �Ĵ���
// return 	= д���ֵ
//--------------------------------------------------------
u8 RF2G4_Read_Reg(u8 Reg)
{
	u8 V_Reg;	    
	
 	RF2G4_CSN = 0;          			// ʹ��SPI����		
	
  	SPI2_ReadWriteByte(Reg);   			// ���ͼĴ�����
	
  	V_Reg=SPI2_ReadWriteByte(0xFF);		// ��ȡ�Ĵ�������
	
  	RF2G4_CSN = 1;          			// ��ֹSPI����		    
	
  	return(V_Reg);           			// ����״ֵ̬
}	
//--------------------------------------------------------



// �ӼĴ����ж���ָ�����ȵ�����
// Reg = �Ĵ���(λ��)
// P_Data = ����ָ��
// N_Data = ���ݸ���
// return = �˴ζ�����״̬�Ĵ���ֵ
//----------------------------------------------------------------------------
u8 RF2G4_Read_Cont(u8 Reg, u8* P_Data, u8 N_Data)
{
	u8 V_Reg;
	u8 C_Read = 0;	
	
  	RF2G4_CSN = 0;           			// ʹ��SPI����
	
  	V_Reg=SPI2_ReadWriteByte(Reg);		// ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	
 	for(; C_Read<N_Data;C_Read++)
	
	P_Data[C_Read] = SPI2_ReadWriteByte(0xFF);	// ��������

  	RF2G4_CSN = 1;       				// �ر�SPI����
	
  	return V_Reg;        				// ���ض�����״ֵ̬
}
//----------------------------------------------------------------------------
//==========================================================================================			   


// ���SI24R1�Ƿ�����
// return = 0:�ɹ�,	1:ʧ��
//---------------------------------------------------------------------------------------------------
u8 RF2G4_Check(void)
{
	u8 Array_Test[5]={0X66,0X66,0X66,0X66,0X66};
	u8 C_Test = 0;
	
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 			// SPI�ٶ�Ϊ9Mhz��SI24R1�����SPIʱ��Ϊ10Mhz��
	
	RF2G4_Write_Cont(W_REGISTER+TX_ADDR,Array_Test,5);	// д��5���ֽڵĵ�ַ.	
	
	RF2G4_Read_Cont(TX_ADDR,Array_Test,5); 				// ����д��ĵ�ַ  
	
	for(; C_Test<5; C_Test++)
	if(Array_Test[C_Test]!=0X66)
		break;
	
	if(C_Test!=5)return 1;		// SI24R1����	
	
	return 0;		 			// SI24R1��ȷ
}
//---------------------------------------------------------------------------------------------------



// ��ʼ��SI24R1Ϊ����ģʽ
//-----------------------------------------------------------------------------------------------------------
void RF2G4_RX_Mode(void)
{
	RF2G4_CE = 0;	  
  	RF2G4_Write_Cont(W_REGISTER+RX_ADDR_P0, (u8*)RF2G4_ADDR_RX, RX_ADR_WIDTH);	// ����RX�ڵ��ַ
	  
  	RF2G4_Write_Reg(W_REGISTER+EN_AA,0x01);    		// ʹ��ͨ��0���Զ�Ӧ��
  	RF2G4_Write_Reg(W_REGISTER+EN_RXADDR,0x01);		// ʹ��ͨ��0�Ľ��յ�ַ
  	RF2G4_Write_Reg(W_REGISTER+RF_CH,40);	     	// ����RFͨ��Ƶ��
  	RF2G4_Write_Reg(W_REGISTER+RX_PW_P0,14);		// ����ͨ��0����Ч���ݿ�ȣ�14λ��
  	RF2G4_Write_Reg(W_REGISTER+RF_SETUP,0x27);		// ����:���书��7dBm����Ƶ������250kbps
  	RF2G4_Write_Reg(W_REGISTER+CONFIG, 0x0F);		// ���ò���;����ģʽ������ģʽ��CRC=2Byte������CRC��������
  	RF2G4_CE = 1; 	// CEΪ��,�������ģʽ 
}					
//-----------------------------------------------------------------------------------------------------------

// ��ʼ��SI24R1Ϊ����ģʽ
//-------------------------------------------------------------------------------------------------------------------
void RF2G4_TX_Mode(void)
{														 
	RF2G4_CE = 0;	    
  	RF2G4_Write_Cont(W_REGISTER+TX_ADDR,(u8*)RF2G4_ADDR_TX,TX_ADR_WIDTH);		// ����TX�ڵ��ַ
  	RF2G4_Write_Cont(W_REGISTER+RX_ADDR_P0,(u8*)RF2G4_ADDR_RX,RX_ADR_WIDTH); 	// ����RX�ڵ��ַ��ACK��	  

  	RF2G4_Write_Reg(W_REGISTER+EN_AA,0x01);     	// ʹ��ͨ��0���Զ�Ӧ��    
  	RF2G4_Write_Reg(W_REGISTER+EN_RXADDR,0x01); 	// ʹ��ͨ��0�Ľ��յ�ַ  
  	RF2G4_Write_Reg(W_REGISTER+SETUP_RETR,0x1A);	// �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	RF2G4_Write_Reg(W_REGISTER+RF_CH,40);       	// ����RFͨ��Ƶ��
  	RF2G4_Write_Reg(W_REGISTER+RF_SETUP,0x27);  	// ����:���书��7dBm����Ƶ������250kbps  
  	RF2G4_Write_Reg(W_REGISTER+CONFIG,0x0E);    	// ���ò���;����ģʽ������ģʽ��CRC=2Byte������CRC��������
	RF2G4_CE = 1;	//CEΪ��,10us����������
}
//-------------------------------------------------------------------------------------------------------------------



// SI24R1����һ֡����
// P_Data	= �����������׵�ַ
// N_Data	= ���������ݵĸ���
// return	= �������״��
//-------------------------------------------------------------------------------------------
u8 RF2G4_Tx_Packet(u8* P_Data,u8 N_Data)
{
	u8 V_Reg;
	
 	SPI2_SetSpeed(SPI_BaudRatePrescaler_4);	// SPI�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	
	RF2G4_CE = 0;
	
  	RF2G4_Write_Cont(W_TX_PAYLOAD,P_Data,N_Data);	// ��Ҫ���͵�����д�뷢�ͻ�����
	
 	RF2G4_CE = 1;			// ��������	   
	
	while(RF2G4_IRQ != 0);	// �ȴ��������
	
	V_Reg = RF2G4_Read_Reg(STATUS);  // ��ȡ״̬�Ĵ�����ֵ	   
	
	RF2G4_Write_Reg(W_REGISTER+STATUS,V_Reg); 	// ���TX_DS/MAX_RT�жϱ�־
	
	
	if(V_Reg & TX_MAX)	// �ж��Ƿ�ﵽ����ط�����
	{
		RF2G4_Write_Reg(FLUSH_TX,0xFF);	// ���TX_FIFO 
		
		return TX_FAIL_MAX; 	// ���ط���ʧ�ܣ��ﵽ����ط�������
	}
	
	if(V_Reg & TX_OK)	
	{
		return TX_SUCCESS;		// ���ط��ͳɹ�
	}

	return 0xFF;				// ���ط���ʧ��
}
//-------------------------------------------------------------------------------------------


// SI24R1����һ֡����
// P_Data	= ����������ݵ��׵�ַ
// N_Data	= ���������ݵĸ���
// return	= ���ݽ������
//-------------------------------------------------------------------------------------------
u8 RF2G4_Rx_Packet(u8* P_Data,u8 N_Data)
{
	u8 V_Reg;		    							   
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 	// SPI�ٶ�Ϊ9Mhz��SI24R1�����SPIʱ��Ϊ10Mhz��   
	
	V_Reg=RF2G4_Read_Reg(STATUS);  				// ��ȡ״̬�Ĵ�����ֵ    	 
	
	RF2G4_Write_Reg(W_REGISTER+STATUS,V_Reg); 	// ���TX_DS/MAX_RT�жϱ�־
	
	if(V_Reg & RX_OK)		// �ж��Ƿ���յ�����
	{
		RF2G4_Read_Cont(R_RX_PAYLOAD,P_Data,N_Data);	// ��ȡ����
		
		RF2G4_Write_Reg(FLUSH_RX,0xFF);					// ���RX_FIFO 
		
		return RX_SUCCESS; 	// ���ؽ��ճɹ�
	}	  
	
	return RX_FAIL;			// ���ؽ���ʧ��
}
//-------------------------------------------------------------------------------------------

