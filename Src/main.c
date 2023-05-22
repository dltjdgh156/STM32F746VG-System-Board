/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bootloader.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart7;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
uint8_t g_de600sFlg = 0, g_de1sFlg = 0, g_de500msFlg = 0, g_de200msFlg = 0, g_de100msFlg = 0, g_de10msFlg = 0, g_de1msFlg = 0, g_de100usFlg = 0;
uint32_t g_deMainCnt = 0;								// Counter of Main Function
uint32_t g_deDiag = 0, g_deDiag_k = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_UART7_Init(void);
static void MX_DAC_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
void Diagnosis(void);
void Inspection(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SDMMC1_SD_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_LWIP_Init();
  MX_UART7_Init();
  MX_DAC_Init();
  MX_ADC1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  // Initialize
  // I2C_INIT() is into low_level_init
  ADCDAC_INIT();
  CARD_INIT();
  ETH_INIT();
  GPIO_INIT();
  RTC_INIT();
  UART_INIT();
  TIM_INIT();
  LCD_INIT();
  FUN_bootloader_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if(g_deMainCnt == 4000000000)
	{
		g_deMainCnt = 0;
	}
	else
	{
		g_deMainCnt++;
	}

	// Periodic Active Function
	if(g_de600sFlg == 1)
	{
		RTC_ADDTION();
		g_de600sFlg = 0;
	}
	if(g_de1sFlg == 1)
	{
		(MOD_DATA.BatteryVolt < 2.05) ? (BStatus.bit.BatteryUv = 1) : (BStatus.bit.BatteryUv = 0);
		(MOD_DATA.ModuleUptime > g_deMainLifetimeNum) ? (BStatus.bit.UptimeOt = 1) : (BStatus.bit.UptimeOt = 0);
		g_de1sFlg = 0;
	}
	if(g_de500msFlg == 1)
	{
		Dout.Led1Inv ^= 1;
		g_de500msFlg = 0;
	}
	if(g_de200msFlg == 1)
	{
		ADCDAC_READ();
		ADCDAC_WRITE();
		g_de200msFlg = 0;
	}
	if(g_de100msFlg == 1)
	{
		ADCDAC_READ();
		g_de100msFlg = 0;
	}
	if(g_de10msFlg == 1)
	{
		g_de10msFlg = 0;
	}
	if(g_de1msFlg == 1)
	{
		g_de1msFlg = 0;
	}
	if(g_de100usFlg == 1)
	{
		g_de100usFlg = 0;
	}
	LCD_UPDATE();
	FUN_bootloader_freerun();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART6|RCC_PERIPHCLK_UART7
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_SDMMC1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_VBAT;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization 
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config 
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x6000030D;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
#if 0
  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */
#endif
  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
#if 0
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
#endif
  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 107;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 499;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 2;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1079;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 9;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief UART7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART7_Init(void)
{

  /* USER CODE BEGIN UART7_Init 0 */

  /* USER CODE END UART7_Init 0 */

  /* USER CODE BEGIN UART7_Init 1 */

  /* USER CODE END UART7_Init 1 */
  huart7.Instance = UART7;
  huart7.Init.BaudRate = 9600;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart7, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART7_Init 2 */

  /* USER CODE END UART7_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DO_EXT_OUT1_GPIO_Port, DO_EXT_OUT1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DO_EXT_OUT2_Pin|DO_EXT_OUT3_Pin|DO_EXT_OUT4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, WIFI__RST_Pin|LCD__CS_Pin|LCD__RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DA_CMD_Pin|LAN__RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DO_SENPWR_Pin|LED_LCD_Pin|DO_LED1_Pin|DO_LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DI_SW3_Pin DI_SW4_Pin DI_EXT_IN1_Pin DI_EXT_IN2_Pin 
                           DI_EXT_IN3_Pin GND_PE10_Pin GND_PE11_Pin GND_PE12_Pin 
                           DI_SW1_Pin DI_SW2_Pin */
  GPIO_InitStruct.Pin = DI_SW3_Pin|DI_SW4_Pin|DI_EXT_IN1_Pin|DI_EXT_IN2_Pin 
                          |DI_EXT_IN3_Pin|GND_PE10_Pin|GND_PE11_Pin|GND_PE12_Pin 
                          |DI_SW1_Pin|DI_SW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : GND_PC13_Pin GND_PC0_Pin DI_EXT_IN4_Pin */
  GPIO_InitStruct.Pin = GND_PC13_Pin|GND_PC0_Pin|DI_EXT_IN4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DO_EXT_OUT1_Pin */
  GPIO_InitStruct.Pin = DO_EXT_OUT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DO_EXT_OUT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DO_EXT_OUT2_Pin DO_EXT_OUT3_Pin DO_EXT_OUT4_Pin */
  GPIO_InitStruct.Pin = DO_EXT_OUT2_Pin|DO_EXT_OUT3_Pin|DO_EXT_OUT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DAC_OUT1_Pin */
  GPIO_InitStruct.Pin = DAC_OUT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DAC_OUT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GND_PB0_Pin GND_PB1_Pin GND_PB2_Pin GND_PB5_Pin 
                           GND_PB8_Pin */
  GPIO_InitStruct.Pin = GND_PB0_Pin|GND_PB1_Pin|GND_PB2_Pin|GND_PB5_Pin 
                          |GND_PB8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : WIFI__RST_Pin LCD__CS_Pin LCD__RST_Pin */
  GPIO_InitStruct.Pin = WIFI__RST_Pin|LCD__CS_Pin|LCD__RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DA_CMD_Pin LAN__RST_Pin */
  GPIO_InitStruct.Pin = LCD_DA_CMD_Pin|LAN__RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GND_PD8_Pin DI_SENPWR_Pin GND_PD11_Pin GND_PD12_Pin 
                           GND_PD14_Pin GND_PD15_Pin SD_DETECT_Pin GND_PD4_Pin 
                           GND_PD7_Pin */
  GPIO_InitStruct.Pin = GND_PD8_Pin|DI_SENPWR_Pin|GND_PD11_Pin|GND_PD12_Pin 
                          |GND_PD14_Pin|GND_PD15_Pin|SD_DETECT_Pin|GND_PD4_Pin 
                          |GND_PD7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DO_SENPWR_Pin LED_LCD_Pin DO_LED1_Pin DO_LED2_Pin */
  GPIO_InitStruct.Pin = DO_SENPWR_Pin|LED_LCD_Pin|DO_LED1_Pin|DO_LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : GND_PA8_Pin GND_PA9_Pin GND_PA10_Pin GND_PA11_Pin 
                           GND_PA12_Pin */
  GPIO_InitStruct.Pin = GND_PA8_Pin|GND_PA9_Pin|GND_PA10_Pin|GND_PA11_Pin 
                          |GND_PA12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

void SetRTCStartTime(void)
{
  sTime.Hours = 13;
  sTime.Minutes = 57;
  sTime.Seconds = 00;

  sDate.Year = 19;
  sDate.Month = 7;
  sDate.Date = 4;

  HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
  HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
}

void GetRTCTime(void)
{
  static RTC_TimeTypeDef oldTime;

  HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);

  if(oldTime.Seconds != sTime.Seconds)
  {
//    UART5_Printf("%d-%d-%d %d:%d:%d\r\n",sDate.Year,sDate.Month,sDate.Date,sTime.Hours,sTime.Minutes,sTime.Seconds);
  }

  oldTime.Seconds = sTime.Seconds;
}

void Diagnosis(void)	// Check Error Event (Periodic Function 100 ms)
{
	if(g_deMainModeIoNum == 0)	// if I/O is Unused
	{
		// Check Particle-Matter(Error Clear Threshold is 5% of Limit)
		(PM_UART.Cnt.Pm003 > g_arMainAlarmNum[1])?(BAlarm.bit.Pm003OpAlm = 1):((PM_UART.Cnt.Pm003 < (g_arMainAlarmNum[1]*0.95))?(BAlarm.bit.Pm003OpAlm = 0):0);
		(PM_UART.Cnt.Pm005 > g_arMainAlarmNum[2])?(BAlarm.bit.Pm005OpAlm = 1):((PM_UART.Cnt.Pm005 < (g_arMainAlarmNum[2]*0.95))?(BAlarm.bit.Pm005OpAlm = 0):0);
		(PM_UART.Cnt.Pm010 > g_arMainAlarmNum[3])?(BAlarm.bit.Pm010OpAlm = 1):((PM_UART.Cnt.Pm010 < (g_arMainAlarmNum[3]*0.95))?(BAlarm.bit.Pm010OpAlm = 0):0);
		(PM_UART.Cnt.Pm025 > g_arMainAlarmNum[4])?(BAlarm.bit.Pm025OpAlm = 1):((PM_UART.Cnt.Pm025 < (g_arMainAlarmNum[4]*0.95))?(BAlarm.bit.Pm025OpAlm = 0):0);
		(PM_UART.Cnt.Pm050 > g_arMainAlarmNum[5])?(BAlarm.bit.Pm050OpAlm = 1):((PM_UART.Cnt.Pm050 < (g_arMainAlarmNum[5]*0.95))?(BAlarm.bit.Pm050OpAlm = 0):0);
		(PM_UART.Cnt.Pm100 > g_arMainAlarmNum[6])?(BAlarm.bit.Pm100OpAlm = 1):((PM_UART.Cnt.Pm100 < (g_arMainAlarmNum[6]*0.95))?(BAlarm.bit.Pm100OpAlm = 0):0);
//		// Check Temperature & Humidity(Error Clear Threshold is 5% of Limit)
//		if(g_deMainModeThNum == 1)
//		{
//			(TH_I2C.Temperature > g_deMainThOtWrnNum)?(BResult.bit.TempStatWarn = 1):((TH_I2C.Temperature < (g_deMainThOtWrnNum*0.95))?(BResult.bit.TempStatWarn = 0):0);
//			(TH_I2C.Temperature > g_deMainThOtErrNum)?(BResult.bit.TempStatErr = 1):((TH_I2C.Temperature < (g_deMainThOtErrNum*0.95))?(BResult.bit.TempStatErr = 0):0);
//			(TH_I2C.Humidity > g_deMainThOhErrNum)?(BResult.bit.HumiStatErr = 1):((TH_I2C.Humidity < (g_deMainThOhErrNum*0.95))?(BResult.bit.HumiStatErr = 0):0);
//		}
//		else
//		{
//			BResult.bit.TempStatWarn = 0;
//			BResult.bit.TempStatErr = 0;
//			BResult.bit.HumiStatErr = 0;
//		}
	}
	else	// if I/O is Used
	{
		// Check Particle-Matter(Error Clear is Only Using External Input)
		(PM_UART.Cnt.Pm003 > g_arMainAlarmNum[1])?(BAlarm.bit.Pm003OpAlm = 1):0;
		(PM_UART.Cnt.Pm005 > g_arMainAlarmNum[2])?(BAlarm.bit.Pm005OpAlm = 1):0;
		(PM_UART.Cnt.Pm010 > g_arMainAlarmNum[3])?(BAlarm.bit.Pm010OpAlm = 1):0;
		(PM_UART.Cnt.Pm025 > g_arMainAlarmNum[4])?(BAlarm.bit.Pm025OpAlm = 1):0;
		(PM_UART.Cnt.Pm050 > g_arMainAlarmNum[5])?(BAlarm.bit.Pm050OpAlm = 1):0;
		(PM_UART.Cnt.Pm100 > g_arMainAlarmNum[6])?(BAlarm.bit.Pm100OpAlm = 1):0;
//		// Check Temperature & Humidity(Error Clear is Only Using External Input)
//		if(g_deMainModeThNum == 1)
//		{
//			(TH_I2C.Temperature > g_deMainThOtWrnNum)?(BResult.bit.TempStatWarn = 1):0;
//			(TH_I2C.Temperature > g_deMainThOtErrNum)?(BResult.bit.TempStatErr = 1):0;
//			(TH_I2C.Humidity > g_deMainThOhErrNum)?(BResult.bit.HumiStatErr = 1):0;
//		}
//		else
//		{
//			BResult.bit.TempStatWarn = 0;
//			BResult.bit.TempStatErr = 0;
//			BResult.bit.HumiStatErr = 0;
//		}
		// Check External Input for Clear
		if(Din.ExternalInput1Inv == 0)	// if Set Clear
			BAlarm.all = 0;
	}

	if((BAlarm_k.all != (BAlarm.all & ~0x0001)) || (BError_k.all != BError.all))	// Check Status(Except 003), Error
	{
		// Check Event for Alarm
		if(BAlarm.bit.Pm005OpAlm != BAlarm_k.bit.Pm005OpAlm)
			(BAlarm.bit.Pm005OpAlm == 0)?(g_deMainLogCodeNum = 0x42):(g_deMainLogCodeNum = 0x43);			// Event Code 0x42(Error Reset), 0x43(Error Set)
		if(BAlarm.bit.Pm010OpAlm != BAlarm_k.bit.Pm010OpAlm)
			(BAlarm.bit.Pm010OpAlm == 0)?(g_deMainLogCodeNum = 0x44):(g_deMainLogCodeNum = 0x45);			// Event Code 0x44(Error Reset), 0x45(Error Set)
		if(BAlarm.bit.Pm025OpAlm != BAlarm_k.bit.Pm025OpAlm)
			(BAlarm.bit.Pm025OpAlm == 0)?(g_deMainLogCodeNum = 0x46):(g_deMainLogCodeNum = 0x47);			// Event Code 0x46(Error Reset), 0x47(Error Set)
		if(BAlarm.bit.Pm050OpAlm != BAlarm_k.bit.Pm050OpAlm)
			(BAlarm.bit.Pm050OpAlm == 0)?(g_deMainLogCodeNum = 0x48):(g_deMainLogCodeNum = 0x49);			// Event Code 0x48(Error Reset), 0x49(Error Set)
		if(BAlarm.bit.Pm100OpAlm != BAlarm_k.bit.Pm100OpAlm)
			(BAlarm.bit.Pm100OpAlm == 0)?(g_deMainLogCodeNum = 0x4A):(g_deMainLogCodeNum = 0x4B);			// Event Code 0x4A(Error Reset), 0x4B(Error Set)

		// Check Event for Error
		if(BError.bit.FanAsErr != BError_k.bit.FanAsErr)
			(BError.bit.FanAsErr == 0)?(g_deMainLogCodeNum = 0x60):(g_deMainLogCodeNum = 0x61);				// Event Code 0x60(Error Reset), 0x61(Error Set)
		if(BError.bit.EepromConnErr != BError_k.bit.EepromConnErr)
			(BError.bit.EepromConnErr == 0)?(g_deMainLogCodeNum = 0x62):(g_deMainLogCodeNum = 0x63);		// Event Code 0x62(Error Reset), 0x63(Error Set)
		if(BError.bit.PmConnErr != BError_k.bit.PmConnErr)
			(BError.bit.PmConnErr == 0)?(g_deMainLogCodeNum = 0x64):(g_deMainLogCodeNum = 0x65);			// Event Code 0x64(Error Reset), 0x65(Error Set)
		if(BError.bit.SdConnErr != BError_k.bit.SdConnErr)
			(BError.bit.SdConnErr == 0)?(g_deMainLogCodeNum = 0x66):(g_deMainLogCodeNum = 0x67);			// Event Code 0x66(Error Reset), 0x67(Error Set)

		// List up for Error Page in Main Display
		switch(g_deMainLogCodeNum)
		{
			case 0x60 :
			case 0x61 :
				g_deMainFanStt = g_deMainLogCodeNum;
				memcpy(g_arMainFanDateAsc, &RDate, 8);
				memcpy(g_arMainFanTimeAsc, &RTime, 8);
				break;
			case 0x64 :
			case 0x65 :
				g_deMainCommStt = g_deMainLogCodeNum;
				memcpy(g_arMainCommDateAsc, &RDate, 8);
				memcpy(g_arMainCommTimeAsc, &RTime, 8);
				break;
			default :
				break;
		}

//		// Check Event for Temperature & Humidity Error
//		if(BResult.bit.TempStatWarn != BResult_k.bit.TempStatWarn)
//			(BResult.bit.TempStatWarn == 0)?(g_deMainLogCodeNum = 12):(g_deMainLogCodeNum = 13);		// Event Code 12(Error Reset), 13(Error Set)
//		if(BResult.bit.TempStatErr != BResult_k.bit.TempStatErr)
//			(BResult.bit.TempStatErr == 0)?(g_deMainLogCodeNum = 44):(g_deMainLogCodeNum = 45);		// Event Code 44(Error Reset), 45(Error Set)
//		if(BResult.bit.HumiStatErr != BResult_k.bit.HumiStatErr)
//			(BResult.bit.HumiStatErr == 0)?(g_deMainLogCodeNum = 46):(g_deMainLogCodeNum = 47);		// Event Code 46(Error Reset), 47(Error Set)
//		g_deMainThStt = g_deMainLogCodeNum;
//		memcpy(g_arMainThDateAsc, &RDate, 8);
//		memcpy(g_arMainThTimeAsc, &RTime, 8);

		// Write Log Code & Date,Time of Event(8-Bytes for Each Event, Max. Number of Event is 128(1024 Bytes))
		(g_deMainLogCntNum < 127) ? (g_deMainLogCntNum++) : (g_deMainLogCntNum = 0);

		g_arMainLogDataNum[g_deMainLogCntNum][0] = g_deMainLogCodeNum;
		g_arMainLogDataNum[g_deMainLogCntNum][1] = 1;
		g_arMainLogDataNum[g_deMainLogCntNum][2] = g_deRtcDateBuf[0];
		g_arMainLogDataNum[g_deMainLogCntNum][3] = g_deRtcDateBuf[1];
		g_arMainLogDataNum[g_deMainLogCntNum][4] = g_deRtcDateBuf[2];
		g_arMainLogDataNum[g_deMainLogCntNum][5] = g_deRtcTimeBuf[0];
		g_arMainLogDataNum[g_deMainLogCntNum][6] = g_deRtcTimeBuf[1];
		g_arMainLogDataNum[g_deMainLogCntNum][7] = g_deRtcTimeBuf[2];

		CPU_I2C.LogCnt = g_deMainLogCntNum;
		memcpy(CPU_I2C.LogData[CPU_I2C.LogCnt], g_arMainLogDataNum[g_deMainLogCntNum], 8);

		I2C1_WRITE();
	}
	BAlarm_k.all = BAlarm.all & ~0x0001;
	BError_k.all = BError.all;
}

void Inspection(void)	// Check Status for Communications (Periodic Function 100 ms)
{
	((PM_UART.Alarm & 3) != 0)?(BError.bit.FanAsErr = 1):(BError.bit.FanAsErr = 0);		// FAN(Immediate, after 10 sec)

	if(g_deMainModeLanNum != 0)						// Check Only When Using Lan
	{
		if(BStatus.bit.LanDisconn == 0)			// LAN(5 Sec)
		{
			switch(g_deMainModeLanNum)
			{
				case 1 :						// Server Only
					if(g_deEthSrvrConnFlg == 0)
					{
						g_deEthSrvrConnCnt++;
						if(g_deEthSrvrConnCnt == 50)
							BStatus.bit.LanDisconn = 1;
					}
					g_deEthSrvrConnFlg = 0;
					g_deEthClntConnFlg = 1;
					break;
				case 2 :						// Client Only
					if(g_deEthClntConnFlg == 0)
					{
						g_deEthClntConnCnt++;
						if(g_deEthClntConnCnt == 50)
							BStatus.bit.LanDisconn = 1;
					}
					g_deEthSrvrConnFlg = 1;
					g_deEthClntConnFlg = 0;
					break;
				case 3 :						// Server & Client
					if(g_deEthSrvrConnFlg == 0)
					{
						g_deEthSrvrConnCnt++;
						if(g_deEthSrvrConnCnt == 50)
							BStatus.bit.LanDisconn = 1;
					}
					if(g_deEthClntConnFlg == 0)
					{
						g_deEthClntConnCnt++;
						if(g_deEthClntConnCnt == 50)
							BStatus.bit.LanDisconn = 1;
					}
					g_deEthSrvrConnFlg = 0;
					g_deEthClntConnFlg = 0;
					break;
				default :
					break;
			}
		}
		else
		{
			switch(g_deMainModeLanNum)
			{
				case 1 :
					if(g_deEthSrvrConnFlg == 1)
						BStatus.bit.LanDisconn = 0;
					break;
				case 2 :
					if(g_deEthClntConnFlg == 1)
						BStatus.bit.LanDisconn = 0;
					break;
				case 3 :
					if((g_deEthSrvrConnFlg == 1) && (g_deEthClntConnFlg == 1))
						BStatus.bit.LanDisconn = 0;
					break;
				default :
					break;
			}
		}
	}
	else
		BStatus.bit.LanDisconn = 0;

	(g_deI2C1ConnFlg == 0)?(BError.bit.EepromConnErr = 1):(BError.bit.EepromConnErr = 0);	// EERPOM(Immediate)

//	if(g_deMainModeThNum == 1)						// Check Only When Using TH Sensor
//	{
//		if(BResult.bit.I2C4ConnErr == 0)		// TH SENSOR(3 Sec)
//		{
//			if(g_deI2C4ConnFlg == 0)
//			{
//				g_deI2C4ConnCnt++;
//				if(g_deI2C4ConnCnt == 30)
//					BResult.bit.I2C4ConnErr = 1;
//			}
//			g_deI2C4ConnFlg = 0;
//		}
//		else
//		{
//			if(g_deI2C4ConnFlg == 1)
//				BResult.bit.I2C4ConnErr = 0;
//		}
//	}
//	else
//		BResult.bit.I2C4ConnErr = 0;

//	if(BResult.bit.Uart1ConnErr == 0)		// WIFI,BLE(3 Sec)
//	{
//
//	}
//	else
//	{
//
//	}

	if(BError.bit.PmConnErr == 0)		// PM SENSOR(10 Sec)
	{
		if(g_deUart2ConnFlg == 0)
		{
			g_deUart2ConnCnt++;
			if(g_deUart2ConnCnt == 100)
				BError.bit.PmConnErr = 1;
		}
		g_deUart2ConnFlg = 0;
	}
	else
	{
		if(g_deUart2ConnFlg == 1)
			BError.bit.PmConnErr = 0;
	}

	if(g_deMainMode485Num != 0)						// Check Only When Using RS485
	{
		if(BStatus.bit.Rs485Disconn == 0)		// RS485(5 Sec)
		{
			if(g_deUart7ConnFlg == 0)
			{
				g_deUart7ConnCnt++;
				if(g_deUart7ConnCnt == 50)
					BStatus.bit.Rs485Disconn = 1;
			}
			g_deUart7ConnFlg = 0;
		}
		else
		{
			if(g_deUart7ConnFlg == 1)
				BStatus.bit.Rs485Disconn = 0;
		}
	}
	else
		BStatus.bit.Rs485Disconn = 0;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
