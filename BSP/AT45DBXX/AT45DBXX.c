

#include "AT45DBXX.h"

// read the status register
// --------------------------------------------------------------------------------
// | bit 7  |  bit 6  |  bit 5  |  bit 4  |  bit 3  |  bit 2  |  bit 1  |  bit 0  |
// --------------------------------------------------------------------------------
// |RDY/BUSY|  COMP   |    0    |    1    |    1    |    1    | PROTECT |PAGE SIZE|
// --------------------------------------------------------------------------------
// bit 7 = 1 -> RDY
// bit 6 = 1 -> buffer compare
// bits 4,3,2 = 1  -> AT42DB041D
// bit 1 = 1 -> sector protected
// bit 0 = 1 -> page size = 256 else page size = 264
// By default the status register = 0x9D

static void AT45DBXX_BUSY(void)
{
	uint8_t status=0;
	uint8_t cmd[1];
	cmd[0] = READ_STATE_REGISTER;
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	do
	{
		HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi1, cmd, 1, 1000);	
		HAL_SPI_Receive(&hspi1, &status, 1, 1000);
		HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	}while(!(status & 0x80));

}

void Eeprom_Chip_Erase(void)
{
	uint8_t cmd[4];
	cmd[0] = 0xc7;
	cmd[1] = 0x94;
	cmd[2] = 0x80;
	cmd[3] = 0x9a;
	AT45DBXX_BUSY();
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 4, 1000);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	AT45DBXX_BUSY();
}

void Eeprom_Write_CR(void)
{
	uint8_t cmd[4];
	cmd[0] = 0x3D;
	cmd[1] = 0x2A;
	cmd[2] = 0x80;
	cmd[3] = 0xA6;
	AT45DBXX_BUSY();
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 4, 1000);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

void Eeprom_Read_ID(uint8_t *ID)
{
	uint8_t cmd[1];
	cmd[0] = Read_ID;
	AT45DBXX_BUSY();
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 1, 1000);	
	HAL_SPI_Receive(&hspi1,ID, 4, 1000);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

void Eeprom_Fill_Buffer(uint16_t BufferOffset,uint8_t Data)
{		
	uint8_t cmd[5];
	cmd[0] = BUF1_WRITE;
	cmd[1] = 0xFF;
	cmd[2] = 0xFF;
	cmd[3] = (uint8_t)BufferOffset;
	cmd[4] = Data;
	AT45DBXX_BUSY();
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 5, 1000);	
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

void Eeprom_Write_Page(uint16_t PageOffset)
{
	uint8_t cmd[4];
	cmd[0] = BBUF1_TO_MM_PAGE_PROG_WITH_ERASE;
	cmd[1] = (uint8_t)(PageOffset>>16);
	cmd[2] = (uint8_t)(PageOffset>>8);
	cmd[3] = 0xFF;
	AT45DBXX_BUSY();
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 4, 1000);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	AT45DBXX_BUSY();
}


uint8_t Eeprom_Read_Buffer(uint16_t BufferOffset)
{		
	uint8_t cmd[5];
	cmd[0] = BUF1_READ;
	cmd[1] = 0xFF;
	cmd[2] = 0xFF;
	cmd[3] = (uint8_t)BufferOffset;
	cmd[4] = 0xFF;
	AT45DBXX_BUSY();
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 5, 1000);
	HAL_SPI_Receive(&hspi1, cmd, 1, 1000);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	return cmd[0];	
}

void Eeprom_Read_Page(uint16_t BufferOffset)
{
	uint8_t cmd[4];
	cmd[0] = MM_PAGE_TO_B1_XFER;
	cmd[1] = (uint8_t)(BufferOffset>>16);
	cmd[2] = (uint8_t)(BufferOffset>>8);
	cmd[3] = 0xFF;
	AT45DBXX_BUSY();
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, cmd, 4, 1000);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}
