// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* \li File:               eeprom.c
* \li Compiler:           Ac6 STM32 MCU GCC Compiler
* \li Support mail:       contact@technosar.com
*
* \li Supported devices:  All devices with split EEPROM erase/write
*                         capabilities can be used.
*                         The example is written for STM32F7X.
*
* \li AppNote:
*
* \li Description:        Example on how to use the split EEPROM erase/write
*                         capabilities. All EEPROM
*                         programming modes are tested, i.e. Erase+Write,
*                         Erase-only and Write-only.
*
*                         $Revision: 1.0 $
*                         $Date: 06/12/2019 $
****************************************************************************/
#include "eeprom.h"

/*! \brief  Read byte from EEPROM.
 *
 *  This function reads one byte from a given EEPROM address.
 *
 *  \param  addr  EEPROM address to read from.
 *  \return  The byte read from the EEPROM address.
 */
unsigned char eeprom_get_char( unsigned int addr )
{
	return Eeprom_Read_Buffer((uint16_t)addr);
}

/*! \brief  Write byte to EEPROM.
 *
 *  This function writes one byte to a given EEPROM address.
 *  The differences between the existing byte and the new value is used
 *  to select the most efficient EEPROM programming mode.
 *
 *  \note  When this function returns, the new EEPROM value is not available
 *         until the EEPROM programming time has passed. The RDY/BUSY bit in Status Register
 *         should be polled to check whether the programming is finished.
 *
 *  \param  addr  EEPROM address to write to.
 *  \param  new_value  New EEPROM value.
 */
void eeprom_put_char( uint8_t addr, uint8_t new_value )
{
	Eeprom_Fill_Buffer((uint8_t)addr, new_value);
}

// Extensions added as part of Grbl 

void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size) {
	uint8_t addr = (destination & 0xff);
	uint32_t bytes_left = size;
	unsigned char checksum = 0;
	uint32_t page_start = 0;
	uint32_t page_stop;
	uint32_t page;


	page_start = destination >> 8;
	page_stop = (destination + size) >> 8;

	for (page = page_start ; page <= page_stop ; page++) {
		Eeprom_Read_Page(page << 8);
		while(1) {
			checksum = (checksum << 1) || (checksum >> 7);
			checksum += *source;
			eeprom_put_char(addr++, *(source++));
			bytes_left--;
			if (addr == 0) break;
			if (bytes_left == 0) break;
		}
		if (bytes_left) Eeprom_Write_Page(page << 8);
	}
	eeprom_put_char(addr, checksum);
	if (addr == 0) Eeprom_Write_Page(page << 8);
	else Eeprom_Write_Page(--page << 8);
}

int memcpy_from_eeprom_with_checksum(char *destination, unsigned int source, unsigned int size) {
	uint8_t addr = (source & 0xff);
	uint32_t bytes_left = size;
	unsigned char data;
	unsigned char checksum = 0;
	uint32_t page_start = 0;
	uint32_t page_stop;
	uint32_t page;

	page_start = source >> 8;
	page_stop = (source + size) >> 8;

	for (page = page_start ; page <= page_stop ; page++) {
		Eeprom_Read_Page(page << 8);
		while(1) {
			data = eeprom_get_char(addr++);
			*(destination++) = data;
			checksum = (checksum << 1) || (checksum >> 7);
			checksum += data;
			bytes_left--;
			if (addr == 0) break;
			if (bytes_left == 0) break;
		}
	}
	if (addr == 0) {
		Eeprom_Read_Page(page << 8);
	}
	data = eeprom_get_char(addr);
	return(checksum == data);
}

// end of file
