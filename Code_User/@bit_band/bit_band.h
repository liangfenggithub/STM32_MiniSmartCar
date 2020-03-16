//****************************************************************************************************
//	在"stm32f10x.h"中：
//
//	#define FLASH_BASE            ((uint32_t)0x08000000) 	// FLASH基地址
//	#define SRAM_BASE             ((uint32_t)0x20000000) 	// SRAM基地址（位带区）
//	#define PERIPH_BASE           ((uint32_t)0x40000000) 	// 外设(寄存器)基地址（位带区）
//
//	#define SRAM_BB_BASE          ((uint32_t)0x22000000) 	// SRAM的别名区的基地址
//	#define PERIPH_BB_BASE        ((uint32_t)0x42000000) 	// 外设的别名区的基地址
//
//	#define FSMC_R_BASE           ((uint32_t)0xA0000000) 	// FSMC寄存器基地址
//
//
//	bit_word_addr = bit_band_base + (byte_offset * 32) + (bit_number * 4)
//
//	bit_word_addr：别名区中，对应的'字'的地址，它和位带区的某个位是映射关系
//	bit_band_base：别名区的起始地址，SRAM_BB_BASE 或 PERIPH_BB_BASE。
//	byte_offset：包含目标位的字节所在位带区的地址
//	bit_number：目标在字节中的序号（由低到高）
//
//****************************************************************************************************
#ifndef __BIT_BAND_H
#define __BIT_BAND_H

#include "stm32f10x.h"	

// 注：参数：Pin_x不应该超过15
//****************************************************************************************************
#define	PA_out(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X1080C * 32) + (Pin_x * 4)))
#define	PB_out(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X10C0C * 32) + (Pin_x * 4)))
#define	PC_out(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X1100C * 32) + (Pin_x * 4)))
#define	PD_out(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X1140C * 32) + (Pin_x * 4)))
#define	PE_out(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X1180C * 32) + (Pin_x * 4)))
#define	PF_out(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X11C0C * 32) + (Pin_x * 4)))
#define	PG_out(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X1200C * 32) + (Pin_x * 4)))


#define	PA_in(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X10808 * 32) + (Pin_x * 4)))
#define	PB_in(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X10C08 * 32) + (Pin_x * 4)))
#define	PC_in(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X11008 * 32) + (Pin_x * 4)))
#define	PD_in(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X11408 * 32) + (Pin_x * 4)))
#define	PE_in(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X11808 * 32) + (Pin_x * 4)))
#define	PF_in(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X11C08 * 32) + (Pin_x * 4)))
#define	PG_in(Pin_x)		* ((volatile unsigned long *)(0x42000000 + (0X12008 * 32) + (Pin_x * 4)))
//****************************************************************************************************

#endif	/* __BIT_BAND_H */
