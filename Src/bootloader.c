/*
 * bootloader.c
 *
 *  Created on: 2020. 6. 2.
 *      Author: New
 */

/* Private includes ----------------------------------------------------------*/
#include "common.h"
#include "bootloader.h"
#include "eth.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);
pFunction JumpToApplication;
uint32_t JumpAddress;

volatile uint32_t u4f_application_flag_address = APPLICATION_FLAG_ADDRESS;

extern UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
static void sfun_Flash_event(uint32_t u4f_flash_address, uint32_t u4f_flag);
static uint32_t sfun_Flash_GetSector(uint32_t Address);

/* Private user code ---------------------------------------------------------*/

// Application Flag ���⿡ ��� (1byte�� ���� �����ϰ� ���� �Լ�) (������ ���� ������ HAL_FLASH_Program �Լ� �ø��� ��)
// 1. Flash Unlock ����
// 2. Flash Clear flag ����
// 3. ����ϰ��� �ϴ� Flash �ּҷ� ���� ���
// 4. �ش� ������ ���� �����
// 5. �ش� �ּҿ� �� ����
// 6. �ٽ� Flash Lock ����
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void sfun_Flash_event(uint32_t u4f_flash_address, uint32_t u4f_flag)
{
	HAL_FLASH_Unlock();

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR );

	uint32_t u4f_sector = sfun_Flash_GetSector(u4f_flash_address);

	FLASH_Erase_Sector(u4f_sector , FLASH_VOLTAGE_RANGE_3);

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, u4f_flash_address, u4f_flag);

	HAL_FLASH_Lock();
}

// Flash�� Address�� �Է����� ������, �ش� ���Ͱ� ��µ�(FLASH_Erase_Sector�� ���)
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static uint32_t sfun_Flash_GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_SECTOR_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_SECTOR_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_SECTOR_2;
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_SECTOR_3;
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_SECTOR_4;
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_SECTOR_5;
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_SECTOR_6;
	}
	else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_SECTOR_7;
	}
	return sector;
}

/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
void FUN_bootloader_init()
{
	sfun_Flash_event(u4f_application_flag_address, 1);
}

/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
void FUN_bootloader_freerun()
{
	// boot �δ� ��� OK
	if(u1f_bootjump_flag == 1)
	{
		HAL_UART_Abort_IT(&huart2);
		sfun_Flash_event(u4f_application_flag_address, 0);

		HAL_Delay(100);

		__disable_irq();

		//
		for(volatile uint8_t i = 0; i < 8; i ++)
		{
			  NVIC->ICER[i] = 0xFFFFFFFF;
		}
		for(volatile uint8_t i = 0; i < 8; i ++)
		{
			  NVIC->ICPR[i] = 0xFFFFFFFF;
		}
		__enable_irq();

		if (((*(__IO uint32_t*)BOOTLOADER_ADDRESS) & 0x2FF80000 ) == 0x20000000)
		{
			JumpAddress = *(__IO uint32_t*) (BOOTLOADER_ADDRESS + 4);
			/* Jump to user application */
			JumpToApplication = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) BOOTLOADER_ADDRESS);
			JumpToApplication();
		}
	}
}

