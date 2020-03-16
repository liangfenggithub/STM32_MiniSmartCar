#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

		  	    													  
void SPI2_Init_JX(void);			// 初始化SPI口

void SPI2_SetSpeed(u8 SpeedSet); 	// 设置SPI速度  

u8 SPI2_ReadWriteByte(u8 TxData);	// SPI读写一个字节
		 
		 
#endif	/* __SPI_H */

