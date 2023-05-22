//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	card.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"
#include "fatfs.h"


// Defines



// Variables
uint8_t g_arCardRxBuf[8] = {0, }, g_arCardTxBuf[128] = {0, };
int8_t g_deFileName[] = "Data_Log_        _      .txt";
char SDPath[4];


// Structures

FATFS SDFatFs; /* File system object for SD card logical drive */
FIL MyFile; /* File object */


// Functions
void CARD_INIT(void);
void CARD_RECV(void);
void CARD_TRANS(void);

void CARD_INIT(void)
{
	uint8_t l_deCardWrCnt = 0;
	int8_t l_deTableHeader[] = "\b\b\bDATE\tTIME\tPM0.3\tPM0.5\tPM1.0\tPM2.5\tPM5.0\tPM10.\tALARM\n";

	// Initialize of Text File
	// phase 1 - Set Title of File
	g_deFileName[9] = '2';
	g_deFileName[10] = '0';
	g_deFileName[11] = (int8_t)(((RTC->DR) & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos)+'0';
	g_deFileName[12] = (int8_t)(((RTC->DR) & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos)+'0';
	g_deFileName[13] = (int8_t)(((RTC->DR) & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos)+'0';
	g_deFileName[14] = (int8_t)(((RTC->DR) & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos)+'0';
	g_deFileName[15] = (int8_t)(((RTC->DR) & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos)+'0';
	g_deFileName[16] = (int8_t)(((RTC->DR) & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos)+'0';

	g_deFileName[18] = (int8_t)(((RTC->TR) & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos)+'0';
	g_deFileName[19] = (int8_t)(((RTC->TR) & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos)+'0';
	g_deFileName[20] = (int8_t)(((RTC->TR) & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos)+'0';
	g_deFileName[21] = (int8_t)(((RTC->TR) & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos)+'0';
	g_deFileName[22] = (int8_t)(((RTC->TR) & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos)+'0';
	g_deFileName[23] = (int8_t)(((RTC->TR) & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos)+'0';

	// phase 2 - Sequence of Create File(Create&Open-Write-Close)
	if(f_mount(&SDFatFs, (TCHAR const*) SDPath, 0) == 0)
	{
		f_open(&MyFile, (char*)g_deFileName, FA_CREATE_ALWAYS | FA_WRITE);
		f_write(&MyFile, (void*)l_deTableHeader, sizeof(l_deTableHeader)-1, (void *) &l_deCardWrCnt);
		f_close(&MyFile);
	}

	// Set Clear Buffer for Write(0x20 = Ascii ' ')
	memset(g_arCardTxBuf, 0x20, sizeof(g_arCardTxBuf));

	g_arCardTxBuf[9] = '\t';	// 9(before 10)
	g_arCardTxBuf[19] = '\t';	// 19(before 20)
	g_arCardTxBuf[29] = '\t';	// 29(before 30)
	g_arCardTxBuf[39] = '\t';	// 39(before 40)
	g_arCardTxBuf[49] = '\t';	// 49(before 50)
	g_arCardTxBuf[59] = '\t';	// 59(before 60)
	g_arCardTxBuf[69] = '\t';	// 69(before 70)
	g_arCardTxBuf[79] = '\t';	// 79(before 80)
	g_arCardTxBuf[89] = '\n';	// 89
}

void CARD_RECV(void)
{

}

void CARD_TRANS(void)
{
	uint8_t l_deCardWrCnt = 0;

	// Sequence of Write File(Set Buffer-Open-Write-Close)
	if(f_mount(&SDFatFs, (TCHAR const*) SDPath, 0) == 0)
	{
		memcpy(g_arCardTxBuf, &RDate, sizeof(RDate));
		memcpy(g_arCardTxBuf+10, &RTime, sizeof(RTime));
		memcpy(g_arCardTxBuf+20, &PCnt.Pm003, sizeof(PCnt.Pm003));
		memcpy(g_arCardTxBuf+30, &PCnt.Pm005, sizeof(PCnt.Pm005));
		memcpy(g_arCardTxBuf+40, &PCnt.Pm010, sizeof(PCnt.Pm010));
		memcpy(g_arCardTxBuf+50, &PCnt.Pm025, sizeof(PCnt.Pm025));
		memcpy(g_arCardTxBuf+60, &PCnt.Pm050, sizeof(PCnt.Pm050));
		memcpy(g_arCardTxBuf+70, &PCnt.Pm100, sizeof(PCnt.Pm100));
		memcpy(g_arCardTxBuf+80, &PAlarm, sizeof(PAlarm));
		f_open(&MyFile, (char*) g_deFileName, FA_OPEN_APPEND | FA_WRITE);
		f_write(&MyFile, (void*)g_arCardTxBuf, 90, (void *) &l_deCardWrCnt);
		f_close(&MyFile);
	}
}

