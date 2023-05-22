//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME				:
//	PROJECT	NAME				:
//	FILE	NAME				:	LCD.h
//	MAKE	BY					:
//	DESCRIPTION					:	tft lcd module control functions
//	CREATED DATE				:	0000.00.00
//	----------------------------------------------------------------------------------------------------


#ifndef LCD_H_
#define LCD_H_
// Includes



// Defines



// Variables
extern uint16_t g_deChCursor;
extern uint8_t g_deBkgdStt;				// 0-Nothing, 1-Main, 2-Setting, 3- Setting Details(Current State)
extern uint8_t g_deMainSelStt;			// 1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 7-Status, 8-Log1, 9-Log2, 10-Log3, 11-Version
extern uint8_t g_deSetSelStt;			// 1-Alarm, 2-Date/time, 3-Mode, 4-Network, 5-Sound
extern uint8_t g_deDetSelStt;			// 1.1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 2.1~3-Date, 4~6-Time, 3.1-IP, 2-Subnet, 3-Gateway, 4-MAC, 4.1-Off/Beep/Melody, 2-Volume, 5.1-Moving Average, 2-Unit, 3-Wifi
extern uint8_t g_deChSelStt;			// Character(ASCII Code, 1 is Most Significant)
extern uint8_t g_deBkgdStt_k;			// 0-Nothing, 1-Main, 2-Setting, 3- Setting Details(Previous State)
extern uint8_t g_deMainSelStt_k;		// 1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 7-Status, 8-Log1, 9-Log2, 10-Log3, 11-Attachment, 12-Version
extern uint8_t g_deSetSelStt_k;			// 1-Alarm, 2-Date/time, 3-Mode, 4-Network, 5-Sound
extern uint8_t g_deDetSelStt_k;			// 1.1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 2.1~3-Date, 4~6-Time, 3.1-IP, 2-Subnet, 3-Gateway, 4-MAC, 4.1-Off/Beep/Melody, 2-Volume, 5.1-Moving Average, 2-Unit, 3-Wifi
extern uint8_t g_deChSelStt_k;			// Character(ASCII Code, 1 is Most Significant)

extern uint8_t g_deMainPmStt;
extern uint8_t g_deMainFanStt;
extern uint8_t g_deMainCommStt;
extern uint8_t g_deMainThStt;
extern int8_t g_arMainPmDateAsc[8];
extern int8_t g_arMainPmTimeAsc[8];
extern int8_t g_arMainFanDateAsc[8];
extern int8_t g_arMainFanTimeAsc[8];
extern int8_t g_arMainCommDateAsc[8];
extern int8_t g_arMainCommTimeAsc[8];
extern int8_t g_arMainThDateAsc[8];
extern int8_t g_arMainThTimeAsc[8];

extern uint8_t g_deMainModeDefFlg;

extern uint16_t g_arMainAlarmNum[6+1];
extern uint8_t g_arMainAccTimeNum[6+1];

extern uint32_t g_deMainLifetimeNum;
extern uint8_t g_deMainLogCntNum;
extern uint8_t g_deMainLogCodeNum;

extern uint8_t g_deMainModeAlarmNum;
extern uint8_t g_deMainModeIoNum;
extern uint8_t g_deMainMode485Num;
extern uint8_t g_deMainModeLanNum;
extern uint8_t g_deMainModeThNum;
extern uint8_t g_deMainModeWlssNum;
extern uint8_t g_deMainModeSmaNum;
extern uint8_t g_deMainModeDefNum;

extern uint32_t g_deMain485BaudNum;
extern uint8_t g_deMain485DataNum;
extern uint8_t g_deMain485PrtyNum;
extern uint8_t g_deMain485StopNum;
extern uint8_t g_deMain485FlowNum;
extern uint8_t g_deMain485ModNum;
extern uint8_t g_deMain485DestNum;
extern uint8_t g_deMain485PtclNum;
extern uint8_t g_deMain485DvcdNum;
extern uint16_t g_deMain485DvldNum;

extern uint8_t g_arMainLanModIpNum[4];
extern uint8_t g_arMainLanModSubNum[4];
extern uint8_t g_arMainLanModGwNum[4];
extern uint16_t g_deMainLanModPortNum;

extern uint8_t g_arMainLanModMacNum[6];
extern uint8_t g_arMainLanDestIpNum[4];
extern uint16_t g_deMainLanDestPortNum;
extern uint8_t g_deMainLanPtclNum;
extern uint8_t g_deMainLanDvcdNum;
extern uint16_t g_deMainLanDvldNum;

extern uint8_t g_arMainWifiSsidNum[16];
extern uint8_t g_arMainWifiPswdNum[16];
extern uint8_t g_arMainWifiModIpNum[4];
extern uint8_t g_arMainWifiModSubNum[4];
extern uint8_t g_arMainWifiModGwNum[4];
extern uint8_t g_arMainWifiModMacNum[6];
extern uint16_t g_deMainWifiModPortNum;
extern uint8_t g_arMainWifiDestIpNum[4];
extern uint16_t g_deMainWifiDestPortNum;
extern uint8_t g_deMainWifiPtclNum;
extern uint8_t g_deMainWifiDvcdNum;
extern uint16_t g_deMainWifiDvldNum;

extern uint8_t g_deMainSndTypNum;
extern uint8_t g_deMainSndVlmNum;

extern uint8_t g_deMainThOtErrNum;
extern uint8_t g_deMainThOtWrnNum;
extern uint8_t g_deMainThOhErrNum;
extern int16_t g_deMainThTempSlpNum;
extern int16_t g_deMainThTempOffNum;
extern int16_t g_deMainThHumiSlpNum;
extern int16_t g_deMainThHumiOffNum;

extern uint8_t g_arMainLogDataNum[128][8];
extern int8_t g_arMainUppDatAsc[13][4];

extern uint32_t g_deSetLifeNum;

extern uint8_t g_deRedrawingFlg;

// Structures



// Functions
extern void LCD_INIT(void);
extern void LCD_UPDATE(void);


#endif /* LCD_H_ */
