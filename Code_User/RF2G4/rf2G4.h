#ifndef __RF2G4_H
#define __RF2G4_H	


#include "stm32f10x.h"
#include "bit_band.h"
#include "spi.h"
#include "key.h"
#include "adc.h"

//--------------------------------------
// 2G4通信地址
//--------------------------------
extern const u8 RF2G4_ADDR_TX[5];

extern const u8 RF2G4_ADDR_RX[5];
//--------------------------------

// 2.4G发射数据缓存数组
extern u8 RF2G4_Send_Data[14];

// 2.4G接收数据缓存数组
extern u8 RF2G4_Receive_Data[14];
//--------------------------------------


//SI24R1寄存器操作命令
//-----------------------------------------------------------------------------------
#define 	R_REGISTER    		0x00  	// 读寄存器,低5位为寄存器地址
#define 	W_REGISTER   		0x20  	// 写寄存器,低5位为寄存器地址
#define 	R_RX_PAYLOAD     	0x61  	// 读RX有效数据,1~32字节
#define 	W_TX_PAYLOAD     	0xA0  	// 写TX有效数据,1~32字节
#define 	FLUSH_TX        	0xE1  	// 清除TX FIFO寄存器.发射模式下用
#define 	FLUSH_RX        	0xE2  	// 清除RX FIFO寄存器.接收模式下用
#define 	REUSE_TX_PL     	0xE3  	// 重新使用上一包数据,CE为高,数据包被不断发送.
#define 	NOP             	0xFF  	// 空操作,可以用来读状态寄存器	 
//-----------------------------------------------------------------------------------

//SPI(SI24R1)寄存器地址
//----------------------------------------------------------------------------------------------------------------------
#define 	CONFIG          	0x00  	// 配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
										// bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define 	EN_AA           	0x01  	// 使能自动应答功能  bit0~5,对应通道0~5
#define 	EN_RXADDR       	0x02  	// 接收地址允许,bit0~5,对应通道0~5
#define 	SETUP_AW        	0x03  	// 设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define 	SETUP_RETR      	0x04  	// 建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define 	RF_CH           	0x05  	// RF通道,bit6:0,工作通道频率;
#define 	RF_SETUP        	0x06  	// RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define 	STATUS          	0x07  	// 状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
										// bit5:数据发送完成中断;bit6:接收数据中断;
										
#define 	OBSERVE_TX      	0x08  	// 发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define 	RSSI              	0x09  	// 载波检测寄存器,bit0,载波检测;
#define 	RX_ADDR_P0      	0x0A  	// 数据通道0接收地址,最大长度5个字节,低字节在前
#define 	RX_ADDR_P1      	0x0B  	// 数据通道1接收地址,最大长度5个字节,低字节在前
#define 	RX_ADDR_P2      	0x0C  	// 数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define 	RX_ADDR_P3      	0x0D  	// 数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define 	RX_ADDR_P4      	0x0E  	// 数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define 	RX_ADDR_P5      	0x0F  	// 数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define 	TX_ADDR         	0x10  	// 发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define 	RX_PW_P0        	0x11  	// 接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define 	RX_PW_P1        	0x12  	// 接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define 	RX_PW_P2        	0x13  	// 接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define 	RX_PW_P3        	0x14  	// 接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define 	RX_PW_P4        	0x15  	// 接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define 	RX_PW_P5        	0x16  	// 接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define 	FIFO_STATUS 		0x17  	// FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
										// bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;
										
#define 	TX_MAX  			0x10  	// 达到最大发送次数中断
#define 	TX_OK   			0x20  	// TX发送完成中断
#define 	RX_OK   			0x40  	// 接收到数据中断


#define		TX_SUCCESS			0		// 发送成功
#define		TX_FAIL				1		// 发送失败
#define		TX_FAIL_MAX			2		// 发送失败
#define		RX_SUCCESS			0		// 接收成功
#define		RX_FAIL				1		// 接收失败
//----------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------
//SI24R1连接
#define 	RF2G4_CE   		PB_out(12) 	// SI24R1选信号
#define 	RF2G4_CSN  		PB_out(10) 	// SPI片选信号	   
#define 	RF2G4_IRQ  		PB_in(11)  	// IRQ主机数据输入

//SI24R1收发数据宽度
#define 	TX_ADR_WIDTH    5   		// 发射地址宽度（5字节）
#define 	RX_ADR_WIDTH    5   		// 接收地址宽度（5字节）
//---------------------------------------------------------------					   	   

//-------------------------------------------------------------------------------------
void RF2G4_Init(void);									// RF2G4初始化
u8 RF2G4_Write_Reg(u8 Reg,u8 Write_Value);				// SPI对SI24R1寄存器的写操作
u8 RF2G4_Write_Cont(u8 Reg, u8* P_Data, u8 N_Data);		// 向寄存器中写入指定长度的数据
u8 RF2G4_Read_Reg(u8 Reg);								// SPI对SI24R1寄存器的读操作
u8 RF2G4_Read_Cont(u8 Reg, u8* P_Data, u8 N_Data);		// 从寄存器中读出指定长度的数据

u8 RF2G4_Check(void);		// 检测SI24R1是否正常

void RF2G4_RX_Mode(void);	// 初始化SI24R1为接收模式
void RF2G4_TX_Mode(void);	// 初始化SI24R1为发射模式

u8 RF2G4_Tx_Packet(u8* P_Data,u8 N_Data);	// SI24R1发送一帧数据
u8 RF2G4_Rx_Packet(u8* P_Data,u8 N_Data);	// SI24R1接收一帧数据
//-------------------------------------------------------------------------------------

#endif	/* __RF2G4_H */
