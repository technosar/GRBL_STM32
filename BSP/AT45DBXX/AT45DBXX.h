/*********************************************************************************************************
*
* File                : AT45DBXX.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 5.15
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2015, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef _AT45DBXX_H
#define _AT45DBXX_H

#include "stm32f7xx.h"
#include <math.h>
#include "spi.h"

#define BUF1_WRITE 0x84
#define BUF2_WRITE 0x87
#define BUF1_READ 0xD4
#define BUF2_READ 0xD6
#define BBUF1_TO_MM_PAGE_PROG_WITH_ERASE 0x83
#define BBUF2_TO_MM_PAGE_PROG_WITH_ERASE 0x86
#define MM_PAGE_TO_B1_XFER 0x53
#define MM_PAGE_TO_B2_XFER 0x55
#define PAGE_ERASE 0x81   // 512/528 bytes per page
#define SECTOR_ERASE 0x7C // 128k bytes per sector
#define READ_STATE_REGISTER 0xD7
#define Read_ID 0x9F

#define EEPROM_PAGE0 0


void AT45DBXX_Read_ID(uint8_t *Data);

void Eeprom_Fill_Buffer(uint16_t BufferOffset,uint8_t Data);
void Eeprom_Write_Page(uint16_t PageOffset);

void Eeprom_Read_Page(uint16_t BufferOffset);
uint8_t Eeprom_Read_Buffer(uint16_t BufferOffset);

void Eeprom_Chip_Erase(void);
void Eeprom_Write_CR(void);

#endif /*_AT45DBXX_H*/

