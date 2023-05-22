//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	adcdac.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"


// Defines



// Variables
uint32_t g_deAdc1RxBuf, g_deAdc2RxBuf[8] = {0, }, g_deAdc3RxBuf[8] = {0, };
uint16_t g_deDac1TxBuf = 0;


// Structures



// Functions
void ADCDAC_INIT(void);
void ADCDAC_READ(void);
void ADCDAC_WRITE(void);

void ADCDAC_INIT(void)
{
	HAL_ADC_Start(&hadc1);					// Use for Voltage of Battery
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);	// Use for Sound Level

	HAL_Delay(100);
}

void ADCDAC_READ(void)
{
	g_deAdc1RxBuf = HAL_ADC_GetValue(&hadc1);
	MOD_DATA.BatteryVolt = (float)(g_deAdc1RxBuf << 2)*P3V30_BIT12;
	HAL_ADC_Start(&hadc1);
}

void ADCDAC_WRITE(void)
{
	if(g_deMainSndVlmNum == 0)			// Off
		g_deDac1TxBuf = 0;
	else if(g_deMainSndVlmNum == 1)		// Low
		g_deDac1TxBuf = 50;
	else if(g_deMainSndVlmNum == 2)		// Middle
		g_deDac1TxBuf = 100;
	else								// High
		g_deDac1TxBuf = 1000;

	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint16_t)(((float)g_deDac1TxBuf)/1000*4095));
}
