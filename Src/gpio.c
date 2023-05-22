//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	gpio.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"


// Defines



// Variables
uint8_t g_deSw1ShortFlg = 0, g_deSw2ShortFlg = 0, g_deSw3ShortFlg = 0, g_deSw4ShortFlg = 0, g_deSw1LongFlg = 0, g_deSw2LongFlg = 0, g_deSw3LongFlg = 0, g_deSw4LongFlg = 0;
uint8_t g_deSw1ShortFlg_k = 0, g_deSw2ShortFlg_k = 0, g_deSw3ShortFlg_k = 0, g_deSw4ShortFlg_k = 0, g_deSw1LongFlg_k = 0, g_deSw2LongFlg_k = 0, g_deSw3LongFlg_k = 0, g_deSw4LongFlg_k = 0;
uint8_t g_deSwCmdFlg = 0;
uint8_t g_deScrnSaveFlg = 0, g_deScrnSaveFlg_k = 0;
uint32_t g_deScrnSaveCnt = 0;

// Structures
DigitalIn_t Din;
DigitalOut_t Dout;


// Functions
void GPIO_INIT(void);					// Initialize General Purpose Input Output
void GPIO_READ(void);
void GPIO_WRITE(void);

void GPIO_INIT(void)
{
	memset((void*)&Din, 0, sizeof(Din));
	memset((void*)&Dout, 0, sizeof(Dout));

	// Gpio Output Data Configure
	Dout.ExternalOutput1 = 0;
	Dout.ExternalOutput2 = 0;
	Dout.ExternalOutput3 = 0;
	Dout.ExternalOutput4 = 0;
	Dout.LanResetInv = 1;
	HAL_GPIO_WritePin(LAN__RST_GPIO_Port, LAN__RST_Pin, Dout.LanResetInv);
	Dout.LcdLed = 1;
	Dout.LcdResetInv = 1;
	Dout.Led1Inv = 1;
	Dout.Led2Inv = 1;
	Dout.SensorPower = 1;
	HAL_GPIO_WritePin(WIFI__RST_GPIO_Port, WIFI__RST_Pin, 1);

	GPIO_READ();
	GPIO_WRITE();
	HAL_Delay(100);
}
void GPIO_READ(void)
{
	static uint16_t l_Sw1Cnt = 0, l_Sw2Cnt = 0, l_Sw3Cnt = 0, l_Sw4Cnt = 0;

	if(g_deMainModeIoNum == 0)
	{
		Din.ExternalInput1Inv = 1;
		Din.ExternalInput2Inv = 1;
		Din.ExternalInput3Inv = 1;
		Din.ExternalInput4Inv = 1;
	}
	else
	{
		Din.ExternalInput1Inv = HAL_GPIO_ReadPin(DI_EXT_IN1_GPIO_Port, DI_EXT_IN1_Pin);		// Read Data of /ExternalInput1
		Din.ExternalInput2Inv = HAL_GPIO_ReadPin(DI_EXT_IN2_GPIO_Port, DI_EXT_IN2_Pin);		// Read Data of /ExternalInput2
		Din.ExternalInput3Inv = HAL_GPIO_ReadPin(DI_EXT_IN3_GPIO_Port, DI_EXT_IN3_Pin);		// Read Data of /ExternalInput3
		Din.ExternalInput4Inv = HAL_GPIO_ReadPin(DI_EXT_IN4_GPIO_Port, DI_EXT_IN4_Pin);		// Read Data of /ExternalInput4
	}

	Din.Switch1Inv = HAL_GPIO_ReadPin(DI_SW1_GPIO_Port, DI_SW1_Pin);					// Read Data of /SwitchInput1
	Din.Switch2Inv = HAL_GPIO_ReadPin(DI_SW2_GPIO_Port, DI_SW2_Pin);					// Read Data of /SwitchInput2
	Din.Switch3Inv = HAL_GPIO_ReadPin(DI_SW3_GPIO_Port, DI_SW3_Pin);					// Read Data of /SwitchInput3
	Din.Switch4Inv = HAL_GPIO_ReadPin(DI_SW4_GPIO_Port, DI_SW4_Pin);					// Read Data of /SwitchInput4

	BDio.bit.In1AlarmEnable = ~Din.ExternalInput1Inv;
	BDio.bit.In2AlarmReset = ~Din.ExternalInput2Inv;
	BDio.bit.In3Reserved = ~Din.ExternalInput3Inv;
	BDio.bit.In4Reserved = ~Din.ExternalInput4Inv;

	g_deSw1ShortFlg_k = g_deSw1ShortFlg;
	g_deSw2ShortFlg_k = g_deSw2ShortFlg;
	g_deSw3ShortFlg_k = g_deSw3ShortFlg;
	g_deSw4ShortFlg_k = g_deSw4ShortFlg;
	g_deSw1LongFlg_k = g_deSw1LongFlg;
	g_deSw2LongFlg_k = g_deSw2LongFlg;
	g_deSw3LongFlg_k = g_deSw3LongFlg;
	g_deSw4LongFlg_k = g_deSw4LongFlg;

	// Switch Input is not Possible During Screen Switching
	if(g_deBkgdStt == g_deBkgdStt_k)
	{
		// Check the Counter of Consecutive Inputs
		if(Din.Switch1Inv == 0)
		{
			l_Sw1Cnt++;
			if(l_Sw1Cnt > SW_MAX_CNT)		// Limitation of Counter Overflow
			{
				l_Sw1Cnt = SW_LONG_CNT+1;
			}
		}
		else
		{
			l_Sw1Cnt = 0;
		}

		if(Din.Switch2Inv == 0)
		{
			l_Sw2Cnt++;
			if(l_Sw2Cnt > SW_MAX_CNT)		// Limitation of Counter Overflow
			{
				l_Sw2Cnt = SW_LONG_CNT+1;
			}
		}
		else
		{
			l_Sw2Cnt = 0;
		}

		if(Din.Switch3Inv == 0)
		{
			l_Sw3Cnt++;
			if(l_Sw3Cnt > SW_MAX_CNT)		// Limitation of Counter Overflow
			{
				l_Sw3Cnt = SW_LONG_CNT+1;
			}
		}
		else
		{
			g_deSw3ShortFlg = 0;
			g_deSw3LongFlg = 0;
			l_Sw3Cnt = 0;
		}

		if(Din.Switch4Inv == 0)
		{
			l_Sw4Cnt++;
			if(l_Sw4Cnt > SW_MAX_CNT)		// Limitation of Counter Overflow
			{
				l_Sw4Cnt = SW_LONG_CNT+1;
			}
		}
		else
		{
			g_deSw4ShortFlg = 0;
			g_deSw4LongFlg = 0;
			l_Sw4Cnt = 0;
		}

		// Switch1 for Normal/Continuous/Special Operation & Sound(If Used)
		if(l_Sw1Cnt == SW_SHORT_CNT)
		{
			g_deSw1ShortFlg = 1;
			if(g_deSw1ShortFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		if(l_Sw1Cnt >= SW_MIDDLE_CNT)
		{
			if((l_Sw1Cnt % SW_MIDDLE_CNT) == 0)
			{
				g_deSw1ShortFlg = 1;
				g_deTimer2Flg = 1;
			}
		}

		if(l_Sw1Cnt == SW_LONG_CNT)
		{
			g_deSw1LongFlg = 1;
			if(g_deSw1LongFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		// Switch2 for Normal/Continuous/Special Operation & Sound(If Used)
		if(l_Sw2Cnt == SW_SHORT_CNT)
		{
			g_deSw2ShortFlg = 1;
			if(g_deSw2ShortFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		if(l_Sw2Cnt >= SW_MIDDLE_CNT)
		{
			if((l_Sw2Cnt % SW_MIDDLE_CNT) == 0)
			{
				g_deSw2ShortFlg = 1;
				g_deTimer2Flg = 1;
			}
		}

		if(l_Sw2Cnt == SW_LONG_CNT)
		{
			g_deSw2LongFlg = 1;
			if(g_deSw2LongFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		// Switch3 for Normal/Special Operation & Sound(If Used)
		if(l_Sw3Cnt == SW_SHORT_CNT)
		{
			g_deSw3ShortFlg = 1;
			if(g_deSw3ShortFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		if(l_Sw3Cnt == SW_LONG_CNT)
		{
			g_deSw3LongFlg = 1;
			if(g_deSw3LongFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		// Switch4 for Normal/Special Operation & Sound(If Used)
		if(l_Sw4Cnt == SW_LONG_CNT)
		{
			g_deSw4LongFlg = 1;
			if(g_deSw4LongFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		if(l_Sw4Cnt == SW_SHORT_CNT)
		{
			g_deSw4ShortFlg = 1;
			if(g_deSw4ShortFlg_k == 0)
				g_deTimer2Flg = 1;
		}

		// Sequence of Log Clear
		if(g_deBkgdStt == 1)
		{
			if((g_deMainSelStt == 8) || (g_deMainSelStt == 9) || (g_deMainSelStt == 10))
			{
				if(g_deSw4LongFlg == 1)
				{
					if(g_deSw3LongFlg == 1)
					{
						g_deSwCmdFlg = 1;
					}
				}
				else
					g_deSwCmdFlg = 0;
			}
		}

		// Sequence of Parameter Reset
		if(g_deBkgdStt == 2)
		{
			if(g_deSw4ShortFlg == 1)
			{
				if(g_deSw4LongFlg == 1)
				{
					if((g_deSw1LongFlg == 1) && (g_deSw2LongFlg == 1))
					{
						g_deSwCmdFlg = 2;
					}
				}
			}
			else
				g_deSwCmdFlg = 0;
		}
	}
	else
	{
		l_Sw1Cnt = 0;
		l_Sw2Cnt = 0;
		l_Sw3Cnt = 0;
		l_Sw4Cnt = 0;

		g_deSw1ShortFlg = 0;
		g_deSw2ShortFlg = 0;
		g_deSw3ShortFlg = 0;
		g_deSw4ShortFlg = 0;
	}

	// if There is no Input for more than 5 Minutes, Display Turns Off
	if((l_Sw1Cnt == 0) && (l_Sw2Cnt == 0) && (l_Sw3Cnt == 0) && (l_Sw4Cnt == 0))
	{
		if(g_deScrnSaveCnt < 300000)	// 5 Minutes @ 1 kHz (5 min x 60 sec/min x 1000 msec/sec)
			g_deScrnSaveCnt++;
		else
			g_deScrnSaveFlg = 1;
	}
	else
	{
		g_deScrnSaveCnt = 0;
		g_deScrnSaveFlg = 0;
	}
}

void GPIO_WRITE(void)
{
	HAL_GPIO_WritePin(DO_SENPWR_GPIO_Port, DO_SENPWR_Pin, Dout.SensorPower);												// Need to Check the Re-freeze and Fix

	// if External Input and Output is used, it will be Check Alarm Status and then Write
	if(g_deMainModeIoNum == 0)
	{
		HAL_GPIO_WritePin(DO_EXT_OUT1_GPIO_Port, DO_EXT_OUT1_Pin, Dout.ExternalOutput1 = 0);
		HAL_GPIO_WritePin(DO_EXT_OUT2_GPIO_Port, DO_EXT_OUT2_Pin, Dout.ExternalOutput2 = 0);
		HAL_GPIO_WritePin(DO_EXT_OUT3_GPIO_Port, DO_EXT_OUT3_Pin, Dout.ExternalOutput3 = 0);
		HAL_GPIO_WritePin(DO_EXT_OUT4_GPIO_Port, DO_EXT_OUT4_Pin, Dout.ExternalOutput4 = 0);
	}
	else
	{
		(((BAlarm.all & ~0xffc1) == 0) && (BError.all == 0)) ? (Dout.ExternalOutput1 = 1) : (Dout.ExternalOutput1 = 0);		// Running : Only Except PM Alarm & All of Error is Clear
		((BAlarm.all & ~0xffc1) == 0) ? (Dout.ExternalOutput2 = 0) : (Dout.ExternalOutput2 = 1);							// PM Alarm : Only PM Alarm
		(BError.all == 0) ? (Dout.ExternalOutput3 = 0) : (Dout.ExternalOutput3 = 1);										// Module Error : All of Error
		Dout.ExternalOutput4 = 0;																							// Reserved : None

		HAL_GPIO_WritePin(DO_EXT_OUT1_GPIO_Port, DO_EXT_OUT1_Pin, Dout.ExternalOutput1);			// Write data of ExternalOutput1
		HAL_GPIO_WritePin(DO_EXT_OUT2_GPIO_Port, DO_EXT_OUT2_Pin, Dout.ExternalOutput2);
		HAL_GPIO_WritePin(DO_EXT_OUT3_GPIO_Port, DO_EXT_OUT3_Pin, Dout.ExternalOutput3);
		HAL_GPIO_WritePin(DO_EXT_OUT4_GPIO_Port, DO_EXT_OUT4_Pin, Dout.ExternalOutput4);
	}

	BDio.bit.Out1Running = Dout.ExternalOutput1;
	BDio.bit.Out2PmAlarm = Dout.ExternalOutput2;
	BDio.bit.Out3ModError = Dout.ExternalOutput3;
	BDio.bit.Out4Reserved = Dout.ExternalOutput4;
}
