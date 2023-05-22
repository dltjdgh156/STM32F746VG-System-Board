
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "flash_if.h"

// Bootloader 영역 Sector 0 ~ 2 : 96KB
#define BOOTLOADER_ADDRESS					(ADDR_FLASH_SECTOR_0)		// 0x0800 0000

// Application Flag 영역 Sector 3 : 32KB
#define APPLICATION_FLAG_ADDRESS			(ADDR_FLASH_SECTOR_3)		// 0x0818 0000

// Application Code 영역 Sector 5 ~ 7 : 768KB
#define APPLICATION_ADDRESS  				(ADDR_FLASH_SECTOR_5)		// 0x0804 0000

//#define BOOTLOADER_VTOR						(BOOTLOADER_ADRESS & 0x000F8000)

#endif  /* __FLASH_IF_H */
