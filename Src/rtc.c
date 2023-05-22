//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	rtc.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"


// Defines



// Variables
uint8_t g_deRtcDateBuf[4] = {0, }, g_deRtcTimeBuf[4] = {0, };
uint16_t g_arRtcUptime[4] = {0, };

// Structures
RealDate_t RDate;
RealTime_t RTime;


// Functions
void RTC_INIT(void);
void RTC_COUNT(void);
void RTC_ADDTION(void);

void RTC_INIT(void)
{
	RDate.YearHAsc = (uint8_t)(((RTC->DR) & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos)+'0';
	RDate.YearLAsc = (uint8_t)(((RTC->DR) & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos)+'0';
	RDate.Dash1Asc = '-';
	RDate.MonthHAsc = (uint8_t)(((RTC->DR) & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos)+'0';
	RDate.MonthLAsc = (uint8_t)(((RTC->DR) & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos)+'0';
	RDate.Dash2Asc = '-';
	RDate.DateHAsc = (uint8_t)(((RTC->DR) & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos)+'0';
	RDate.DateLAsc = (uint8_t)(((RTC->DR) & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos)+'0';

	RTime.HourHAsc = (uint8_t)(((RTC->TR) & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos)+'0';
	RTime.HourLAsc = (uint8_t)(((RTC->TR) & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos)+'0';
	RTime.Colon1Asc = ':';
	RTime.MinuteHAsc = (uint8_t)(((RTC->TR) & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos)+'0';
	RTime.MinuteLAsc = (uint8_t)(((RTC->TR) & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos)+'0';
	RTime.Colon2Asc = ':';
	RTime.SecondHAsc = (uint8_t)(((RTC->TR) & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos)+'0';
	RTime.SecondLAsc = (uint8_t)(((RTC->TR) & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos)+'0';
}

void RTC_COUNT(void)
{
	static uint8_t l_deSecond_k = 0;
	static uint16_t l_deRtcCnt = 1250, l_deTenMiliCnt = 0;

	l_deTenMiliCnt++;
	if(l_deTenMiliCnt == 100)	// Update Per 10 msec
	{
		g_deRtcTimeBuf[3]++;
		if(l_deSecond_k != ((uint8_t)(((RTC->TR) & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos)))
		{
			l_deSecond_k = (uint8_t)(((RTC->TR) & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos);
			g_deRtcTimeBuf[3] = 0;
		}
	}

	l_deRtcCnt++;
	if(l_deRtcCnt == 2500)	// Update Per 250 msec
	{
		// Using for Display LCD in Real Time(Ascii)
		RDate.YearHAsc = (uint8_t)(((RTC->DR) & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos)+'0';
		RDate.YearLAsc = (uint8_t)(((RTC->DR) & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos)+'0';
		RDate.MonthHAsc = (uint8_t)(((RTC->DR) & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos)+'0';
		RDate.MonthLAsc = (uint8_t)(((RTC->DR) & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos)+'0';
		RDate.DateHAsc = (uint8_t)(((RTC->DR) & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos)+'0';
		RDate.DateLAsc = (uint8_t)(((RTC->DR) & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos)+'0';

		// Using for Display LCD in Real Time(Ascii)
		RTime.HourHAsc = (uint8_t)(((RTC->TR) & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos)+'0';
		RTime.HourLAsc = (uint8_t)(((RTC->TR) & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos)+'0';
		RTime.MinuteHAsc = (uint8_t)(((RTC->TR) & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos)+'0';
		RTime.MinuteLAsc = (uint8_t)(((RTC->TR) & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos)+'0';
		RTime.SecondHAsc = (uint8_t)(((RTC->TR) & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos)+'0';
		RTime.SecondLAsc = (uint8_t)(((RTC->TR) & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos)+'0';

		// Using for Event Log(Numeric-Decimal)(D[0]:Years, D[1]:Months, D[2]:Days, D[3]:Weeks, T[0]:Hours, T[1]:Minutes, T[2]:Seconds, T[3]:10 Mili-Seconds)
		g_deRtcDateBuf[0] = (uint8_t)(((RTC->DR) & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos)*10 + (uint8_t)(((RTC->DR) & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos);
		g_deRtcDateBuf[1] = (uint8_t)(((RTC->DR) & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos)*10 + (uint8_t)(((RTC->DR) & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos);
		g_deRtcDateBuf[2] = (uint8_t)(((RTC->DR) & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos)*10 + (uint8_t)(((RTC->DR) & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos);
		g_deRtcDateBuf[3] = (uint8_t)(((RTC->DR) & RTC_DR_WDU_Msk) >> RTC_DR_WDU_Pos);
		g_deRtcTimeBuf[0] = (uint8_t)(((RTC->TR) & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos)*10 + (uint8_t)(((RTC->TR) & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos);
		g_deRtcTimeBuf[1] = (uint8_t)(((RTC->TR) & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos)*10 + (uint8_t)(((RTC->TR) & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos);
		g_deRtcTimeBuf[2] = (uint8_t)(((RTC->TR) & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos)*10 + (uint8_t)(((RTC->TR) & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos);

		l_deRtcCnt = 0;
	}
}

void RTC_ADDTION(void)
{
	// Write Up-time Every 10 Minutes([0]:Days(=1 Hour * 24), [1]:Hours(=10 Minutes * 6), [2]:10 Minutes, [3]:rsvd)
	g_arRtcUptime[2]++;		// Update Per 10 Minutes
	if(g_arRtcUptime[2] >= 6)
	{
		g_arRtcUptime[1]++;
		g_arRtcUptime[2] -= 6;
	}
	if(g_arRtcUptime[1] >= 24)
	{
		g_arRtcUptime[0]++;
		g_arRtcUptime[1] -= 24;
	}

	MOD_DATA.ModuleUptime = ((uint32_t)g_arRtcUptime[0]*24)+(uint32_t)g_arRtcUptime[1];
	memcpy(&CPU_I2C.AccDay, g_arRtcUptime, 6);

	I2C1_WRITE();
}
