//****************************************************************************************************
//	��"stm32f10x.h"�У�
//
//	#define FLASH_BASE            ((uint32_t)0x08000000) 	// FLASH����ַ
//	#define SRAM_BASE             ((uint32_t)0x20000000) 	// SRAM����ַ��λ������
//	#define PERIPH_BASE           ((uint32_t)0x40000000) 	// ����(�Ĵ���)����ַ��λ������
//
//	#define SRAM_BB_BASE          ((uint32_t)0x22000000) 	// SRAM�ı������Ļ���ַ
//	#define PERIPH_BB_BASE        ((uint32_t)0x42000000) 	// ����ı������Ļ���ַ
//
//	#define FSMC_R_BASE           ((uint32_t)0xA0000000) 	// FSMC�Ĵ�������ַ
//
//
//	bit_word_addr = bit_band_base + (byte_offset * 32) + (bit_number * 4)
//
//	bit_word_addr���������У���Ӧ��'��'�ĵ�ַ������λ������ĳ��λ��ӳ���ϵ
//	bit_band_base������������ʼ��ַ��SRAM_BB_BASE �� PERIPH_BB_BASE��
//	byte_offset������Ŀ��λ���ֽ�����λ�����ĵ�ַ
//	bit_number��Ŀ�����ֽ��е���ţ��ɵ͵��ߣ�
//
//****************************************************************************************************
#ifndef __BIT_BAND_H
#define __BIT_BAND_H

#include "stm32f10x.h"	

// ע��������Pin_x��Ӧ�ó���15
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
