/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "adcdac.h"
#include "card.h"
#include "eth.h"
#include "gpio.h"
#include "i2c.h"
#include "icd.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "uart.h"
#include "util.h"

#include "ILI9341.h"
#include "LCD.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart7;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void Diagnosis(void);
extern void Inspection(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DI_SW3_Pin GPIO_PIN_2
#define DI_SW3_GPIO_Port GPIOE
#define DI_SW4_Pin GPIO_PIN_3
#define DI_SW4_GPIO_Port GPIOE
#define DI_EXT_IN1_Pin GPIO_PIN_4
#define DI_EXT_IN1_GPIO_Port GPIOE
#define DI_EXT_IN2_Pin GPIO_PIN_5
#define DI_EXT_IN2_GPIO_Port GPIOE
#define DI_EXT_IN3_Pin GPIO_PIN_6
#define DI_EXT_IN3_GPIO_Port GPIOE
#define GND_PC13_Pin GPIO_PIN_13
#define GND_PC13_GPIO_Port GPIOC
#define GND_PC0_Pin GPIO_PIN_0
#define GND_PC0_GPIO_Port GPIOC
#define DI_EXT_IN4_Pin GPIO_PIN_2
#define DI_EXT_IN4_GPIO_Port GPIOC
#define DO_EXT_OUT1_Pin GPIO_PIN_3
#define DO_EXT_OUT1_GPIO_Port GPIOC
#define PO_BUZZ_Pin GPIO_PIN_0
#define PO_BUZZ_GPIO_Port GPIOA
#define DO_EXT_OUT2_Pin GPIO_PIN_3
#define DO_EXT_OUT2_GPIO_Port GPIOA
#define DAC_OUT1_Pin GPIO_PIN_4
#define DAC_OUT1_GPIO_Port GPIOA
#define DO_EXT_OUT3_Pin GPIO_PIN_5
#define DO_EXT_OUT3_GPIO_Port GPIOA
#define DO_EXT_OUT4_Pin GPIO_PIN_6
#define DO_EXT_OUT4_GPIO_Port GPIOA
#define GND_PB0_Pin GPIO_PIN_0
#define GND_PB0_GPIO_Port GPIOB
#define GND_PB1_Pin GPIO_PIN_1
#define GND_PB1_GPIO_Port GPIOB
#define GND_PB2_Pin GPIO_PIN_2
#define GND_PB2_GPIO_Port GPIOB
#define RS485_RX_Pin GPIO_PIN_7
#define RS485_RX_GPIO_Port GPIOE
#define RS485_TX_Pin GPIO_PIN_8
#define RS485_TX_GPIO_Port GPIOE
#define RS485_DE_Pin GPIO_PIN_9
#define RS485_DE_GPIO_Port GPIOE
#define GND_PE10_Pin GPIO_PIN_10
#define GND_PE10_GPIO_Port GPIOE
#define GND_PE11_Pin GPIO_PIN_11
#define GND_PE11_GPIO_Port GPIOE
#define GND_PE12_Pin GPIO_PIN_12
#define GND_PE12_GPIO_Port GPIOE
#define WIFI__RST_Pin GPIO_PIN_13
#define WIFI__RST_GPIO_Port GPIOE
#define LCD__CS_Pin GPIO_PIN_14
#define LCD__CS_GPIO_Port GPIOE
#define LCD__RST_Pin GPIO_PIN_15
#define LCD__RST_GPIO_Port GPIOE
#define LCD_SCK_Pin GPIO_PIN_10
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_DA_CMD_Pin GPIO_PIN_14
#define LCD_DA_CMD_GPIO_Port GPIOB
#define LCD_MOSI_Pin GPIO_PIN_15
#define LCD_MOSI_GPIO_Port GPIOB
#define GND_PD8_Pin GPIO_PIN_8
#define GND_PD8_GPIO_Port GPIOD
#define DI_SENPWR_Pin GPIO_PIN_9
#define DI_SENPWR_GPIO_Port GPIOD
#define DO_SENPWR_Pin GPIO_PIN_10
#define DO_SENPWR_GPIO_Port GPIOD
#define GND_PD11_Pin GPIO_PIN_11
#define GND_PD11_GPIO_Port GPIOD
#define GND_PD12_Pin GPIO_PIN_12
#define GND_PD12_GPIO_Port GPIOD
#define LED_LCD_Pin GPIO_PIN_13
#define LED_LCD_GPIO_Port GPIOD
#define GND_PD14_Pin GPIO_PIN_14
#define GND_PD14_GPIO_Port GPIOD
#define GND_PD15_Pin GPIO_PIN_15
#define GND_PD15_GPIO_Port GPIOD
#define TH_TX_Pin GPIO_PIN_6
#define TH_TX_GPIO_Port GPIOC
#define TH_RX_Pin GPIO_PIN_7
#define TH_RX_GPIO_Port GPIOC
#define GND_PA8_Pin GPIO_PIN_8
#define GND_PA8_GPIO_Port GPIOA
#define GND_PA9_Pin GPIO_PIN_9
#define GND_PA9_GPIO_Port GPIOA
#define GND_PA10_Pin GPIO_PIN_10
#define GND_PA10_GPIO_Port GPIOA
#define GND_PA11_Pin GPIO_PIN_11
#define GND_PA11_GPIO_Port GPIOA
#define GND_PA12_Pin GPIO_PIN_12
#define GND_PA12_GPIO_Port GPIOA
#define DO_LED1_Pin GPIO_PIN_0
#define DO_LED1_GPIO_Port GPIOD
#define DO_LED2_Pin GPIO_PIN_1
#define DO_LED2_GPIO_Port GPIOD
#define SD_DETECT_Pin GPIO_PIN_3
#define SD_DETECT_GPIO_Port GPIOD
#define GND_PD4_Pin GPIO_PIN_4
#define GND_PD4_GPIO_Port GPIOD
#define PM_TX_Pin GPIO_PIN_5
#define PM_TX_GPIO_Port GPIOD
#define PM_RX_Pin GPIO_PIN_6
#define PM_RX_GPIO_Port GPIOD
#define GND_PD7_Pin GPIO_PIN_7
#define GND_PD7_GPIO_Port GPIOD
#define GND_PB5_Pin GPIO_PIN_5
#define GND_PB5_GPIO_Port GPIOB
#define EEPROM_SCL_Pin GPIO_PIN_6
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin GPIO_PIN_7
#define EEPROM_SDA_GPIO_Port GPIOB
#define GND_PB8_Pin GPIO_PIN_8
#define GND_PB8_GPIO_Port GPIOB
#define LAN__RST_Pin GPIO_PIN_9
#define LAN__RST_GPIO_Port GPIOB
#define DI_SW1_Pin GPIO_PIN_0
#define DI_SW1_GPIO_Port GPIOE
#define DI_SW2_Pin GPIO_PIN_1
#define DI_SW2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
extern uint8_t g_de600sFlg, g_de1sFlg, g_de500msFlg, g_de200msFlg, g_de100msFlg, g_de10msFlg, g_de1msFlg, g_de100usFlg;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
