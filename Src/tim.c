//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	tim.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"
#include "lwip.h"


// Defines



// Variables
uint8_t g_deTimer2Flg = 0;
uint32_t g_deTimer2Frq = 2, g_deTimer2Cnt = 0, g_deTimer6Cnt = 0;


// Structures
EffDataTrRate_t Edtr;


// Functions
void TIM_INIT(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void TIM_INIT(void)
{
	  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	  HAL_TIM_Base_Start_IT(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t l_deRateCnt = 0;
	static uint16_t l_de1sCnt = 5000, l_de500msCnt = 0, l_de200msCnt = 250, l_de100msCnt = 500, l_de10msCnt = 50, l_de1msCnt = 5;
	static uint32_t l_de600sCnt = 0;

	if(htim->Instance == TIM2)
	{

	}

	if(htim->Instance == TIM6)	// periodic interrupt function(main timer)
	{
		(Dout.Led1Inv == 1)?(GPIOD->BSRR = 0x00000001):(GPIOD->BSRR = 0x00010000);// Signal of Routine
		Dout.Led2Inv = 1;
		GPIOD->BSRR = 0x00000002;// Start Signal of Interrupt Routine(Set to High)

		RTC_COUNT();					// Count Up Internal Counter, Renewal of Internal Date/Time

		g_deTimer6Cnt++;
		if(g_deTimer6Cnt == 36000000)	// Count Up Per 100 usec For 1 Hour
		{
			g_deTimer6Cnt = 0;
		}

		l_deRateCnt++;
		if(l_deRateCnt == 10000)	// Update Per 1 sec ; None
		{
			l_deRateCnt = 0;
		}

		l_de600sCnt++;
		if(l_de600sCnt == 6000000)		// Periodic 10 Minutes ; None
		{
			g_de600sFlg = 1;
			l_de600sCnt = 0;
		}
		l_de1sCnt++;
		if(l_de1sCnt == 10000)		// Periodic 1 Second ; Particle-matter Request
		{
			UART2_TRANS();

			g_de1sFlg = 1;
			l_de1sCnt = 0;
		}
		l_de500msCnt++;
		if(l_de500msCnt == 5000)		// Periodic 500 Mili-Seconds ; LCD Cursor
		{
			g_deChCursor ^= 0xffff;

			g_de500msFlg = 1;
			l_de500msCnt = 0;
		}
		l_de200msCnt++;
		if(l_de200msCnt == 2000)		// Periodic 200 Mili-Seconds ; Ethernet(Client)/RS485/SD-Card Transmission
		{
			if(l_de1sCnt == 2750)
			{
				ETH_CLNT_TRANS();
			}
			if(l_de1sCnt == 4750)
			{
				UART7_TRANS();
			}
			if(l_de1sCnt == 8750)
			{
				CARD_TRANS();
			}
			g_de200msFlg = 1;
			l_de200msCnt = 0;
		}
		l_de100msCnt++;
		if(l_de100msCnt == 1000)		// Periodic 100 Mili-Seconds ; Error Status, Temperature/Humidity Request
		{
			Diagnosis();
			Inspection();
			if(l_de1sCnt == 1500)
			{
			//	I2C4_WRITE();
			}
			if(l_de1sCnt == 2500)
			{
			//	I2C4_READ();
			}

			g_de100msFlg = 1;
			l_de100msCnt = 0;
		}
		l_de10msCnt++;
		if(l_de10msCnt == 100)		// Periodic 10 Mili-Seconds ; Sound, Ethernet Status
		{
			TIM2_WRITE();
			netif_set_link_callback(&gnetif, ethernetif_update_config);

			g_de10msFlg = 1;
			l_de10msCnt = 0;
		}
		l_de1msCnt++;
		if(l_de1msCnt == 10)		// Periodic 1 Mili-Second ; Gpio(Read/Write)
		{
			GPIO_READ();			// Read Gpio
			GPIO_WRITE();			// Write Gpio

			g_de1msFlg = 1;
			l_de1msCnt = 0;
		}
		g_de100usFlg = 1;

		MX_LWIP_Process();		// Renewal Ethernet

		Dout.Led2Inv = 0;
		GPIOD->BSRR = 0x00020000;// End Signal of Interrupt Routine(Reset to Low)
	}
	HAL_TIM_Base_Start_IT(&htim6);
}
void TIM2_WRITE(void)	// 10 ms
{
	if(g_deMainSndVlmNum == 0)		// 'Off'
		g_deTimer2Frq = 2;
	else							// Except 'Off'
	{
		if((g_deMainModeAlarmNum == 1) && ((BError.all != 0) || (BAlarm.all & ~0x0001) != 0))		// Sound for Alarm(Except 0.3 um)
		{
			if(g_deMainSndTypNum == 0)	// Beep
			{
				// Beep : On - 1 Second, Off - 1 Second
				if(g_deTimer2Cnt < 100)
					g_deTimer2Frq = 499;
				else
					g_deTimer2Frq = 2;

				g_deTimer2Cnt++;
				if(g_deTimer2Cnt == 200)
				{
					g_deTimer2Cnt = 0;
				}
			}
			else						// Melody
			{
				// Bagatelle No. 25 in A minor(Fur Elise), Solo
				g_deTimer2Cnt++;
				switch(g_deTimer2Cnt)
				{
					case 1*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 2*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 3*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 4*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 5*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 6*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 7*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 8*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 9*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 11*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 12*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4C;	break;
					case 13*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 14*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 15*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 17*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 18*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 19*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4GS;	break;
					case 20*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 21*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 23*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 24*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 25*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 26*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 27*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 28*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 29*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 30*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 31*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 32*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 33*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 35*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 36*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4C;	break;
					case 37*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 38*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 39*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 41*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 42*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 43*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 44*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 45*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 47*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 48*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 49*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 50*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 51*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 53*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 54*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4G;	break;
					case 55*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5F;	break;
					case 56*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 57*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 59*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 60*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4F;	break;
					case 61*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 62*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 63*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 65*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 66*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 67*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 68*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 69*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 71*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 72*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 73*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 74*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 75*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 76*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 77*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 78*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 79*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 80*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 81*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 82*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 83*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 84*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 85*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 86*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 87*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 89*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 90*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4C;	break;
					case 91*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 92*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 93*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 95*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 96*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 97*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4GS;	break;
					case 98*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 99*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 101*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 102*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 103*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 104*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 105*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 106*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5DS;	break;
					case 107*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5E;	break;
					case 108*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 109*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5D;	break;
					case 110*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 111*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 113*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 114*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4C;	break;
					case 115*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 116*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 117*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 119*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 120*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4E;	break;
					case 121*BEAT_EGHT	:	g_deTimer2Frq = SCALE_5C;	break;
					case 122*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4B;	break;
					case 123*BEAT_EGHT	:	g_deTimer2Frq = SCALE_4A;	break;
					case 127*BEAT_EGHT	:	g_deTimer2Frq = SCALE_NO;	break;
					case 128*BEAT_EGHT :	g_deTimer2Cnt = 0;			break;
					default :
						break;
				}
			}
		}
		else if(g_deTimer2Flg == 1)		// Sound for Switch(Beep Only)
		{
			// Make a Sound for 100 Mili-Seconds
			g_deTimer2Frq = 499;
			g_deTimer2Cnt++;

			if(g_deTimer2Cnt == 10)
			{
				g_deTimer2Cnt = 0;
				g_deTimer2Flg = 0;
			}
		}
		else							// No Operation
		{
			g_deTimer2Flg = 0;
			g_deTimer2Frq = 2;
		}
	}
	TIM2->ARR = g_deTimer2Frq;
	TIM2->CCR1 = ((g_deTimer2Frq+1) >> 1);
}
