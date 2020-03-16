#include "rf2G4.h"


// 2G4通信地址
//------------------------------------------------------------------------------------------		
const u8 RF2G4_ADDR_TX[5] = { 'j', 'i', 'x', 'i', 'n' } ;		// 0X6A,0X69,0X78,0X69,0X6E
const u8 RF2G4_ADDR_RX[5] = { 'j', 'i', 'x', 'i', 'n' } ;		// ASCII：jixin
//------------------------------------------------------------------------------------------

u8 RF2G4_Send_Data[14] = { 0 };		// 2.4G发射数据缓存数组
			 
u8 RF2G4_Receive_Data[14] = { 0 };	// 2.4G接收数据缓存数组


// 	无线发射(RF2G4_Send_Data[14])：共输出14个数据，分别为
//--------------------------------------------------------------
// 	0  == WKUP					// 1:按下	0：松开
//	1  == KEY1					// 1:按下	0：松开
//	2  == KEY_UP				// 1:按下	0：松开
//	3  == KEY_DOWN				// 1:按下	0：松开
//	4  == KEY_LEFT				// 1:按下	0：松开
//	5  == KEY_RIGHT				// 1:按下	0：松开
//	6  == KEY_A					// 1:按下	0：松开
//	7  == KEY_AA				// 1:按下	0：松开
//	8  == KEY_B					// 1:按下	0：松开
//	9  == KEY_BB				// 1:按下	0：松开

//	10 == KEY_LeftRocker		// 1:按下	0：松开
//	11 == KEY_RightRocker		// 1:按下	0：松开

//  12 == Rocker_FORWARD_x		// 前进/后退：档位（0～255）
//  13 == Rocker_BACK_x			// 左转/右转：档位（0～255）
//--------------------------------------------------------------


// RF2G4初始化
//------------------------------------------------------------------------------------------
void RF2G4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 	// 使能PB端口时钟
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12;		// PB10、PB12推挽 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	// 推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);						// 初始化指定IO
	GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_12);				// PB10、PB12输出低
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 				// PB11输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
						 

	SPI2_Init_JX();    		// 初始化SPI	 
 
	RF2G4_CE=0; 			// 使能SI24R1
	
	RF2G4_CSN=1;			// SPI片选取消  
	
}
//------------------------------------------------------------------------------------------	 	 


// 2G4的SPI读写
//==========================================================================================

// SPI对SI24R1寄存器的写操作
// Reg 			= 寄存器
// Write_Value 	= 写入的值
// return		= 状态值
//--------------------------------------------------------
u8 RF2G4_Write_Reg(u8 Reg,u8 Write_Value)
{
	u8 V_Reg;	
	
   	RF2G4_CSN = 0;                 		// 使能SPI传输
	
  	V_Reg =SPI2_ReadWriteByte(Reg);		// 发送寄存器号 
	
  	SPI2_ReadWriteByte(Write_Value);	// 写入寄存器的值
	
  	RF2G4_CSN = 1;                 		// 禁止SPI传输	 
	
  	return(V_Reg);       				// 返回状态值
}
//--------------------------------------------------------

// 向寄存器中写入指定长度的数据
// Reg		= 寄存器
// P_Data 	= 数据指针
// N_Data 	= 数据个数
// return 	= 此次读到的状态寄存器值
//--------------------------------------------------------
u8 RF2G4_Write_Cont(u8 Reg, u8* P_Data, u8 N_Data)
{
	u8 V_Reg = 0;
	u8 C_Write = 0;
	
 	RF2G4_CSN = 0;          			// 使能SPI传输
	
  	V_Reg = SPI2_ReadWriteByte(Reg);	// 选定寄存器,并读取状态值
	
  	for(; C_Write<N_Data; C_Write++)
	{
		SPI2_ReadWriteByte(*P_Data++); 	// 写入数据
	}
		 
  	RF2G4_CSN = 1;       				// 关闭SPI传输
	
  	return V_Reg;          				// 返回读到的状态值
}
//--------------------------------------------------------



// SPI对SI24R1寄存器的读操作
// Reg 		= 寄存器
// return 	= 写入的值
//--------------------------------------------------------
u8 RF2G4_Read_Reg(u8 Reg)
{
	u8 V_Reg;	    
	
 	RF2G4_CSN = 0;          			// 使能SPI传输		
	
  	SPI2_ReadWriteByte(Reg);   			// 发送寄存器号
	
  	V_Reg=SPI2_ReadWriteByte(0xFF);		// 读取寄存器内容
	
  	RF2G4_CSN = 1;          			// 禁止SPI传输		    
	
  	return(V_Reg);           			// 返回状态值
}	
//--------------------------------------------------------



// 从寄存器中读出指定长度的数据
// Reg = 寄存器(位置)
// P_Data = 数据指针
// N_Data = 数据个数
// return = 此次读到的状态寄存器值
//----------------------------------------------------------------------------
u8 RF2G4_Read_Cont(u8 Reg, u8* P_Data, u8 N_Data)
{
	u8 V_Reg;
	u8 C_Read = 0;	
	
  	RF2G4_CSN = 0;           			// 使能SPI传输
	
  	V_Reg=SPI2_ReadWriteByte(Reg);		// 发送寄存器值(位置),并读取状态值
	
 	for(; C_Read<N_Data;C_Read++)
	
	P_Data[C_Read] = SPI2_ReadWriteByte(0xFF);	// 读出数据

  	RF2G4_CSN = 1;       				// 关闭SPI传输
	
  	return V_Reg;        				// 返回读到的状态值
}
//----------------------------------------------------------------------------
//==========================================================================================			   


// 检测SI24R1是否正常
// return = 0:成功,	1:失败
//---------------------------------------------------------------------------------------------------
u8 RF2G4_Check(void)
{
	u8 Array_Test[5]={0X66,0X66,0X66,0X66,0X66};
	u8 C_Test = 0;
	
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 			// SPI速度为9Mhz（SI24R1的最大SPI时钟为10Mhz）
	
	RF2G4_Write_Cont(W_REGISTER+TX_ADDR,Array_Test,5);	// 写入5个字节的地址.	
	
	RF2G4_Read_Cont(TX_ADDR,Array_Test,5); 				// 读出写入的地址  
	
	for(; C_Test<5; C_Test++)
	if(Array_Test[C_Test]!=0X66)
		break;
	
	if(C_Test!=5)return 1;		// SI24R1错误	
	
	return 0;		 			// SI24R1正确
}
//---------------------------------------------------------------------------------------------------



// 初始化SI24R1为接收模式
//-----------------------------------------------------------------------------------------------------------
void RF2G4_RX_Mode(void)
{
	RF2G4_CE = 0;	  
  	RF2G4_Write_Cont(W_REGISTER+RX_ADDR_P0, (u8*)RF2G4_ADDR_RX, RX_ADR_WIDTH);	// 设置RX节点地址
	  
  	RF2G4_Write_Reg(W_REGISTER+EN_AA,0x01);    		// 使能通道0的自动应答
  	RF2G4_Write_Reg(W_REGISTER+EN_RXADDR,0x01);		// 使能通道0的接收地址
  	RF2G4_Write_Reg(W_REGISTER+RF_CH,40);	     	// 设置RF通信频率
  	RF2G4_Write_Reg(W_REGISTER+RX_PW_P0,14);		// 设置通道0的有效数据宽度（14位）
  	RF2G4_Write_Reg(W_REGISTER+RF_SETUP,0x27);		// 设置:发射功率7dBm、射频数据率250kbps
  	RF2G4_Write_Reg(W_REGISTER+CONFIG, 0x0F);		// 配置参数;接收模式、开机模式、CRC=2Byte、开启CRC、。。。
  	RF2G4_CE = 1; 	// CE为高,进入接收模式 
}					
//-----------------------------------------------------------------------------------------------------------

// 初始化SI24R1为发射模式
//-------------------------------------------------------------------------------------------------------------------
void RF2G4_TX_Mode(void)
{														 
	RF2G4_CE = 0;	    
  	RF2G4_Write_Cont(W_REGISTER+TX_ADDR,(u8*)RF2G4_ADDR_TX,TX_ADR_WIDTH);		// 设置TX节点地址
  	RF2G4_Write_Cont(W_REGISTER+RX_ADDR_P0,(u8*)RF2G4_ADDR_RX,RX_ADR_WIDTH); 	// 设置RX节点地址（ACK）	  

  	RF2G4_Write_Reg(W_REGISTER+EN_AA,0x01);     	// 使能通道0的自动应答    
  	RF2G4_Write_Reg(W_REGISTER+EN_RXADDR,0x01); 	// 使能通道0的接收地址  
  	RF2G4_Write_Reg(W_REGISTER+SETUP_RETR,0x1A);	// 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	RF2G4_Write_Reg(W_REGISTER+RF_CH,40);       	// 设置RF通信频率
  	RF2G4_Write_Reg(W_REGISTER+RF_SETUP,0x27);  	// 设置:发射功率7dBm、射频数据率250kbps  
  	RF2G4_Write_Reg(W_REGISTER+CONFIG,0x0E);    	// 配置参数;接收模式、开机模式、CRC=2Byte、开启CRC、。。。
	RF2G4_CE = 1;	//CE为高,10us后启动发送
}
//-------------------------------------------------------------------------------------------------------------------



// SI24R1发送一帧数据
// P_Data	= 待发送数据首地址
// N_Data	= 待发送数据的个数
// return	= 发送完成状况
//-------------------------------------------------------------------------------------------
u8 RF2G4_Tx_Packet(u8* P_Data,u8 N_Data)
{
	u8 V_Reg;
	
 	SPI2_SetSpeed(SPI_BaudRatePrescaler_4);	// SPI速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	
	RF2G4_CE = 0;
	
  	RF2G4_Write_Cont(W_TX_PAYLOAD,P_Data,N_Data);	// 将要发送的数据写入发送缓存器
	
 	RF2G4_CE = 1;			// 启动发送	   
	
	while(RF2G4_IRQ != 0);	// 等待发送完成
	
	V_Reg = RF2G4_Read_Reg(STATUS);  // 读取状态寄存器的值	   
	
	RF2G4_Write_Reg(W_REGISTER+STATUS,V_Reg); 	// 清除TX_DS/MAX_RT中断标志
	
	
	if(V_Reg & TX_MAX)	// 判断是否达到最大重发次数
	{
		RF2G4_Write_Reg(FLUSH_TX,0xFF);	// 清除TX_FIFO 
		
		return TX_FAIL_MAX; 	// 返回发送失败（达到最大重发次数）
	}
	
	if(V_Reg & TX_OK)	
	{
		return TX_SUCCESS;		// 返回发送成功
	}

	return 0xFF;				// 返回发送失败
}
//-------------------------------------------------------------------------------------------


// SI24R1接收一帧数据
// P_Data	= 缓存接收数据的首地址
// N_Data	= 待缓存数据的个数
// return	= 数据接收情况
//-------------------------------------------------------------------------------------------
u8 RF2G4_Rx_Packet(u8* P_Data,u8 N_Data)
{
	u8 V_Reg;		    							   
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 	// SPI速度为9Mhz（SI24R1的最大SPI时钟为10Mhz）   
	
	V_Reg=RF2G4_Read_Reg(STATUS);  				// 读取状态寄存器的值    	 
	
	RF2G4_Write_Reg(W_REGISTER+STATUS,V_Reg); 	// 清除TX_DS/MAX_RT中断标志
	
	if(V_Reg & RX_OK)		// 判断是否接收到数据
	{
		RF2G4_Read_Cont(R_RX_PAYLOAD,P_Data,N_Data);	// 读取数据
		
		RF2G4_Write_Reg(FLUSH_RX,0xFF);					// 清除RX_FIFO 
		
		return RX_SUCCESS; 	// 返回接收成功
	}	  
	
	return RX_FAIL;			// 返回接收失败
}
//-------------------------------------------------------------------------------------------

