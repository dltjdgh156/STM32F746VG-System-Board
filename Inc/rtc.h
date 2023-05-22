//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	rtc.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef C_RTC_H_
#define C_RTC_H_
// Includes



// Defines



// Variables
extern uint8_t g_deRtcDateBuf[4], g_deRtcTimeBuf[4];
extern uint16_t g_arRtcUptime[4];


// Structures
typedef struct
{
	uint8_t YearHAsc	: 8;
	uint8_t YearLAsc	: 8;
	uint8_t Dash1Asc	: 8;
	uint8_t MonthHAsc	: 8;
	uint8_t MonthLAsc	: 8;
	uint8_t Dash2Asc	: 8;
	uint8_t DateHAsc	: 8;
	uint8_t DateLAsc	: 8;
	uint8_t : 8;
}
RealDate_t;
extern RealDate_t RDate;

typedef struct
{
	uint8_t HourHAsc	: 8;
	uint8_t HourLAsc	: 8;
	uint8_t Colon1Asc	: 8;
	uint8_t MinuteHAsc	: 8;
	uint8_t MinuteLAsc	: 8;
	uint8_t Colon2Asc	: 8;
	uint8_t SecondHAsc	: 8;
	uint8_t SecondLAsc	: 8;
	uint8_t : 8;
}
RealTime_t;
extern RealTime_t RTime;



// Functions
extern void RTC_INIT(void);
extern void RTC_COUNT(void);
extern void RTC_ADDTION(void);


#endif /* C_RTC_H_ */
