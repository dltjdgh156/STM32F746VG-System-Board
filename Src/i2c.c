//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	i2c.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"
#include "lwip.h"
#include "lwip/netif.h"


// Defines



// Variables
uint8_t g_arI2c1RxBuf[1280] = {0, }, g_arI2c1TxBuf[1280] = {0, };
uint8_t g_arI2c4RxBuf[8] = {0, }, g_arI2c4TxBuf[4] = {0x2c, 0x06};
uint8_t g_deI2C1ConnFlg = 0;
uint8_t g_deI2C4ConnFlg = 0, g_deI2C4ConnCnt = 0;


// Structures



// Functions
void I2C_INIT(void);					// Initialize Inter Integrated Circuit
void I2C1_READ(void);
void I2C1_WRITE(void);
void I2C4_READ(void);
void I2C4_WRITE(void);

void I2C_INIT(void)
{
	uint8_t l_deTemp[16] = {0};

	if(g_deSw3LongFlg == 0)
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xa0, 0, 2, g_arI2c1TxBuf, 2, 100);		// Read Flag Data
		HAL_Delay(10);

		CPU_I2C.ResetFlag = g_arI2c1TxBuf[0];
		CPU_I2C.ClearFlag = g_arI2c1TxBuf[1];
	}

	if(CPU_I2C.ResetFlag != 1)
	{
		CPU_I2C.ResetFlag = 1;
		CPU_I2C.Alarm003 = 9900;
		CPU_I2C.Alarm005 = 9900;
		CPU_I2C.Alarm010 = 9900;
		CPU_I2C.Alarm025 = 9900;
		CPU_I2C.Alarm050 = 9900;
		CPU_I2C.Alarm100 = 9900;
		CPU_I2C.ModeAlarm = 0;				// [21]
		CPU_I2C.ModeIo = 0;					// [22]
		CPU_I2C.Mode485 = 0;				// [23]
		CPU_I2C.ModeLan = 1;				// [24]
		CPU_I2C.ModeTh = 0;					// [25]
		CPU_I2C.ModeWlss = 0;				// [26]
		CPU_I2C.ModeSma = 1;				// [27]
		CPU_I2C.ModeDef = 0;				// [28]
		CPU_I2C.ModeRsvd = 0;				// [29]
		CPU_I2C.SndTyp = 0;					// [30]
		CPU_I2C.SndVlm = 0;					// [31]
		CPU_I2C.AlarmLifetime = 43800;		// [32]~[35]
		CPU_I2C.AlarmTempWrn = 65;			// [36]~[39]
		CPU_I2C.AlarmTempErr = 75;			// [40]~[43]
		CPU_I2C.AlarmHumiErr = 85;			// [44]~[47]
		CPU_I2C.TempSlope = 1;				// [48]~[51]
		CPU_I2C.TempOffset = 0;				// [52]~[55]
		CPU_I2C.HumiSlope = 1;				// [56]~[59]
		CPU_I2C.HumiOffset = 0;				// [60]~[63]

		CPU_I2C.Rs485Baud = 9600;
		CPU_I2C.Rs485Data = 8;
		CPU_I2C.Rs485Prty = 0;
		CPU_I2C.Rs485Stop = 1;
		CPU_I2C.Rs485Flow = 0;
		CPU_I2C.Rs485Mod = 0;
		CPU_I2C.Rs485Dest = 0;
		CPU_I2C.Rs485Ptcl = 1;
		CPU_I2C.Rs485Dvcd = 0xa8;
		CPU_I2C.Rs485Dvld = 0;
		l_deTemp[0] = 192;	l_deTemp[1] = 168;	l_deTemp[2] = 0;	l_deTemp[3] = 110;
		memcpy(CPU_I2C.LanModIp, l_deTemp, 4);
		l_deTemp[0] = 255;	l_deTemp[1] = 255;	l_deTemp[2] = 255;	l_deTemp[3] = 0;
		memcpy(CPU_I2C.LanModSub, l_deTemp, 4);
		l_deTemp[0] = 192;	l_deTemp[1] = 168;	l_deTemp[2] = 0;	l_deTemp[3] = 1;
		memcpy(CPU_I2C.LanModGw, l_deTemp, 4);
		CPU_I2C.LanModPort = 49152;
		l_deTemp[0] = 0;	l_deTemp[1] = 1;	l_deTemp[2] = 2;	l_deTemp[3] = 3;
		l_deTemp[4] = 4;	l_deTemp[5] = 5;
		memcpy(CPU_I2C.LanModMac, l_deTemp, 6);
		l_deTemp[0] = 192;	l_deTemp[1] = 168;	l_deTemp[2] = 210;	l_deTemp[3] = 11;
		memcpy(CPU_I2C.LanDestIp, l_deTemp, 4);
		CPU_I2C.LanDestPort = 49152;


		l_deTemp[0] = 0;	l_deTemp[1] = 0;	l_deTemp[2] = 0;	l_deTemp[3] = 0;
		l_deTemp[4] = 0;	l_deTemp[5] = 0;	l_deTemp[6] = 0;	l_deTemp[7] = 0;
		l_deTemp[8] = 0;	l_deTemp[9] = 0;	l_deTemp[10] = 0;	l_deTemp[11] = 0;
		l_deTemp[12] = 0;	l_deTemp[13] = 0;	l_deTemp[14] = 0;	l_deTemp[15] = 0;
		memcpy(CPU_I2C.WifiSsid,l_deTemp, 16);

		l_deTemp[0] = 0;	l_deTemp[1] = 0;	l_deTemp[2] = 0;	l_deTemp[3] = 0;
		l_deTemp[4] = 0;	l_deTemp[5] = 0;	l_deTemp[6] = 0;	l_deTemp[7] = 0;
		l_deTemp[8] = 0;	l_deTemp[9] = 0;	l_deTemp[10] = 0;	l_deTemp[11] = 0;
		l_deTemp[12] = 0;	l_deTemp[13] = 0;	l_deTemp[14] = 0;	l_deTemp[15] = 0;
		memcpy(CPU_I2C.WifiPswd, l_deTemp, 16);

		l_deTemp[0] = 192;	l_deTemp[1] = 168;	l_deTemp[2] = 210;	l_deTemp[3] = 110;
		memcpy(CPU_I2C.WifiModIp, l_deTemp, 4);
		l_deTemp[0] = 255;	l_deTemp[1] = 255;	l_deTemp[2] = 255;	l_deTemp[3] = 0;
		memcpy(CPU_I2C.WifiModSub, l_deTemp, 4);
		l_deTemp[0] = 192;	l_deTemp[1] = 168;	l_deTemp[2] = 210;	l_deTemp[3] = 1;
		memcpy(CPU_I2C.WifiModGw, l_deTemp, 4);
		CPU_I2C.WifiModPort = 49152;
		l_deTemp[0] = 0;	l_deTemp[1] = 1;	l_deTemp[2] = 2;	l_deTemp[3] = 3;
		l_deTemp[4] = 4;	l_deTemp[5] = 5;
		memcpy(CPU_I2C.WifiModMac, l_deTemp, 6);
		l_deTemp[0] = 192;	l_deTemp[1] = 168;	l_deTemp[2] = 210;	l_deTemp[3] = 11;
		memcpy(CPU_I2C.WifiDestIp, l_deTemp, 4);
		CPU_I2C.WifiDestPort = 49152;

		l_deTemp[0] = 0;	l_deTemp[1] = 0;	l_deTemp[2] = 0;	l_deTemp[3] = 0;
		l_deTemp[4] = 0;	l_deTemp[5] = 0;	l_deTemp[6] = 0;	l_deTemp[7] = 0;
		l_deTemp[8] = 0;	l_deTemp[9] = 0;	l_deTemp[10] = 0;	l_deTemp[11] = 0;
		l_deTemp[12] = 0;	l_deTemp[13] = 0;	l_deTemp[14] = 0;	l_deTemp[15] = 0;
		memcpy(CPU_I2C.BleServUuid, l_deTemp, 16);
		l_deTemp[0] = 0;	l_deTemp[1] = 0;	l_deTemp[2] = 0;	l_deTemp[3] = 0;
		l_deTemp[4] = 0;	l_deTemp[5] = 0;	l_deTemp[6] = 0;	l_deTemp[7] = 0;
		l_deTemp[8] = 0;	l_deTemp[9] = 0;	l_deTemp[10] = 0;	l_deTemp[11] = 0;
		l_deTemp[12] = 0;	l_deTemp[13] = 0;	l_deTemp[14] = 0;	l_deTemp[15] = 0;
		memcpy(CPU_I2C.BleCharUuid, l_deTemp, 16);
		l_deTemp[0] = 0;	l_deTemp[1] = 0;	l_deTemp[2] = 0;	l_deTemp[3] = 0;
		l_deTemp[4] = 0;	l_deTemp[5] = 0;	l_deTemp[6] = 0;	l_deTemp[7] = 0;
		l_deTemp[8] = 0;	l_deTemp[9] = 0;	l_deTemp[10] = 0;	l_deTemp[11] = 0;
		l_deTemp[12] = 0;	l_deTemp[13] = 0;	l_deTemp[14] = 0;	l_deTemp[15] = 0;
		memcpy(CPU_I2C.BleSpareUuid, l_deTemp, 16);
		l_deTemp[0] = 0;	l_deTemp[1] = 0;	l_deTemp[2] = 0;	l_deTemp[3] = 0;
		l_deTemp[4] = 0;	l_deTemp[5] = 0;	l_deTemp[6] = 0;	l_deTemp[7] = 0;
		l_deTemp[8] = 0;	l_deTemp[9] = 0;	l_deTemp[10] = 0;	l_deTemp[11] = 0;
		l_deTemp[12] = 0;	l_deTemp[13] = 0;	l_deTemp[14] = 0;	l_deTemp[15] = 0;
		memcpy(CPU_I2C.BlePincode, l_deTemp, 16);

		memcpy(g_arI2c1TxBuf, &CPU_I2C.ResetFlag, 256);

		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 0, 2, g_arI2c1TxBuf+0, 1, 100);				// Reset Flag
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 8, 2, g_arI2c1TxBuf+8, 56, 100);			// Alarm, Mode, Sound, Temp,humi
		HAL_Delay(10);

		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 64, 2, g_arI2c1TxBuf+64, 64, 100);			// Serial, Network
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 128, 2, g_arI2c1TxBuf+128, 64, 100);		// Wireless(WIFI)
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 192, 2, g_arI2c1TxBuf+192, 64, 100);		// Wireless(BLE)
		HAL_Delay(10);
	}

	if(CPU_I2C.ClearFlag != 1)
	{
		CPU_I2C.ClearFlag = 1;
		CPU_I2C.LogCnt = 0;
		memset(CPU_I2C.LogData, 0xff, 1024);

		g_arI2c1TxBuf[1] = CPU_I2C.ClearFlag;
		memcpy(&g_arI2c1TxBuf[20], &CPU_I2C.LogCnt, 1);
		memcpy(&g_arI2c1TxBuf[256], &CPU_I2C.LogData, 1024);

		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1, 2, g_arI2c1TxBuf+1, 1, 100);				// ClearFlag
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 20, 2, g_arI2c1TxBuf+20, 1, 100);			// LogCnt
		HAL_Delay(10);

		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 256, 2, g_arI2c1TxBuf+256, 64, 100);		// LogData0
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 320, 2, g_arI2c1TxBuf+320, 64, 100);		// LogData1
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 384, 2, g_arI2c1TxBuf+384, 64, 100);		// LogData2
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 448, 2, g_arI2c1TxBuf+448, 64, 100);		// LogData3
		HAL_Delay(10);

		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 512, 2, g_arI2c1TxBuf+512, 64, 100);		// LogData4
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 576, 2, g_arI2c1TxBuf+576, 64, 100);		// LogData5
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 640, 2, g_arI2c1TxBuf+640, 64, 100);		// LogData6
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 704, 2, g_arI2c1TxBuf+704, 64, 100);		// LogData7
		HAL_Delay(10);

		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 768, 2, g_arI2c1TxBuf+768, 64, 100);		// LogData8
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 832, 2, g_arI2c1TxBuf+832, 64, 100);		// LogData9
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 896, 2, g_arI2c1TxBuf+896, 64, 100);		// LogData10
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 960, 2, g_arI2c1TxBuf+960, 64, 100);		// LogData11
		HAL_Delay(10);

		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1024, 2, g_arI2c1TxBuf+1024, 64, 100);		// LogData8
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1088, 2, g_arI2c1TxBuf+1088, 64, 100);		// LogData9
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1152, 2, g_arI2c1TxBuf+1152, 64, 100);		// LogData10
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1216, 2, g_arI2c1TxBuf+1216, 64, 100);		// LogData11
		HAL_Delay(10);
	}
	// Parameters
	if(HAL_I2C_Mem_Read(&hi2c1, 0xa0, 0, 2, g_arI2c1TxBuf, 64, 100) == 0)
		g_deI2C1ConnFlg = 1;
	else
		g_deI2C1ConnFlg = 0;
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 64, 2, g_arI2c1TxBuf+64, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 128, 2, g_arI2c1TxBuf+128, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 192, 2, g_arI2c1TxBuf+192, 64, 100);
	HAL_Delay(10);
	memcpy(&CPU_I2C.ResetFlag, g_arI2c1TxBuf+0, 256);
	// Log(No.0 ~ No.31)
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 256, 2, g_arI2c1TxBuf+256, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 320, 2, g_arI2c1TxBuf+320, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 384, 2, g_arI2c1TxBuf+384, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 448, 2, g_arI2c1TxBuf+448, 64, 100);
	HAL_Delay(10);
	memcpy(&CPU_I2C.LogData[0][0], g_arI2c1TxBuf+256, 256);
	// Log(No.32 ~ No.63)
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 512, 2, g_arI2c1TxBuf+512, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 576, 2, g_arI2c1TxBuf+576, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 640, 2, g_arI2c1TxBuf+640, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 704, 2, g_arI2c1TxBuf+704, 64, 100);
	HAL_Delay(10);
	memcpy(&CPU_I2C.LogData[32][0], g_arI2c1TxBuf+512, 256);
	// Log(No.64 ~ No.95)
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 768, 2, g_arI2c1TxBuf+768, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 832, 2, g_arI2c1TxBuf+832, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 896, 2, g_arI2c1TxBuf+896, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 960, 2, g_arI2c1TxBuf+960, 64, 100);
	HAL_Delay(10);
	memcpy(&CPU_I2C.LogData[64][0], g_arI2c1TxBuf+768, 256);
	// Log(No.96 ~ No.127)
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 1024, 2, g_arI2c1TxBuf+1024, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 1088, 2, g_arI2c1TxBuf+1088, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 1152, 2, g_arI2c1TxBuf+1152, 64, 100);
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, 0xa0, 1216, 2, g_arI2c1TxBuf+1216, 64, 100);
	HAL_Delay(10);
	memcpy(&CPU_I2C.LogData[96][0], g_arI2c1TxBuf+1024, 256);

	// 2-3, 4-5, 6-7(6 Bytes)
	g_arRtcUptime[0] = CPU_I2C.AccDay;
	g_arRtcUptime[1] = CPU_I2C.AccHour;
	g_arRtcUptime[2] = CPU_I2C.AccMin;
	// 8-9, 10-11, 12-13, 14-15, 16-17, 18-19(12 Bytes)
	g_arMainAlarmNum[1] = CPU_I2C.Alarm003;
	g_arMainAlarmNum[2] = CPU_I2C.Alarm005;
	g_arMainAlarmNum[3] = CPU_I2C.Alarm010;
	g_arMainAlarmNum[4] = CPU_I2C.Alarm025;
	g_arMainAlarmNum[5] = CPU_I2C.Alarm050;
	g_arMainAlarmNum[6] = CPU_I2C.Alarm100;
	// 20-21(2 Byte)
	g_deMainLogCntNum = CPU_I2C.LogCnt;
	// 21, 22, 23, 24, 25, 26, 27, 28(8 Bytes + rsvd)
	g_deMainModeAlarmNum = CPU_I2C.ModeAlarm;
	g_deMainModeIoNum = CPU_I2C.ModeIo;
	g_deMainMode485Num = CPU_I2C.Mode485;
	g_deMainModeLanNum = CPU_I2C.ModeLan;
	g_deMainModeThNum = CPU_I2C.ModeTh;
	g_deMainModeWlssNum = CPU_I2C.ModeWlss;
	g_deMainModeSmaNum = CPU_I2C.ModeSma;
	g_deMainModeDefNum = CPU_I2C.ModeDef;
	// 30, 31(2 Bytes)
	g_deMainSndTypNum = CPU_I2C.SndTyp;
	g_deMainSndVlmNum = CPU_I2C.SndVlm;

	// 32-55(24 Bytes)
	g_deMainLifetimeNum = CPU_I2C.AlarmLifetime;
	g_deMainThOtWrnNum = (uint8_t)CPU_I2C.AlarmTempWrn;
	g_deMainThOtErrNum = (uint8_t)CPU_I2C.AlarmTempErr;
	g_deMainThOhErrNum = (uint8_t)CPU_I2C.AlarmHumiErr;

	g_deMainThTempSlpNum = CPU_I2C.TempSlope;
	g_deMainThTempOffNum = CPU_I2C.TempOffset;
	g_deMainThHumiSlpNum = CPU_I2C.HumiSlope;
	g_deMainThHumiOffNum = CPU_I2C.HumiOffset;

	// 64-75(12 Bytes)
	g_deMain485BaudNum = CPU_I2C.Rs485Baud;
	g_deMain485DataNum = CPU_I2C.Rs485Data;
	g_deMain485PrtyNum = CPU_I2C.Rs485Prty;
	g_deMain485StopNum = CPU_I2C.Rs485Stop;
	g_deMain485FlowNum = CPU_I2C.Rs485Flow;
	g_deMain485ModNum = CPU_I2C.Rs485Mod;
	g_deMain485DestNum = CPU_I2C.Rs485Dest;
	g_deMain485PtclNum = CPU_I2C.Rs485Ptcl;
	g_deMain485DvcdNum = CPU_I2C.Rs485Dvcd;
	g_deMain485DvldNum = CPU_I2C.Rs485Dvld;
	// 76-105(30 Bytes)
	memcpy(g_arMainLanModIpNum, CPU_I2C.LanModIp, 4);
	memcpy(g_arMainLanModSubNum, CPU_I2C.LanModSub, 4);
	memcpy(g_arMainLanModGwNum, CPU_I2C.LanModGw, 4);
	g_deMainLanModPortNum = CPU_I2C.LanModPort;
	memcpy(g_arMainLanModMacNum, CPU_I2C.LanModMac, 6);
	memcpy(g_arMainLanDestIpNum, CPU_I2C.LanDestIp, 4);
	g_deMainLanDestPortNum = CPU_I2C.LanDestPort;
	g_deMainLanPtclNum = CPU_I2C.LanPtcl;
	g_deMainLanDvcdNum = CPU_I2C.LanDvcd;
	g_deMainLanDvldNum = CPU_I2C.LanDvld;

	// 128-189(62 Bytes)
	memcpy(g_arMainWifiSsidNum, CPU_I2C.WifiSsid, 16);
	memcpy(g_arMainWifiPswdNum, CPU_I2C.WifiPswd, 16);
	memcpy(g_arMainWifiModIpNum, CPU_I2C.WifiModIp, 4);
	memcpy(g_arMainWifiModSubNum, CPU_I2C.WifiModSub, 4);
	memcpy(g_arMainWifiModGwNum, CPU_I2C.WifiModGw, 4);
	g_deMainWifiModPortNum = CPU_I2C.WifiModPort;
	memcpy(g_arMainWifiModMacNum, CPU_I2C.WifiModMac, 6);
	memcpy(g_arMainWifiDestIpNum, CPU_I2C.WifiDestIp, 4);
	g_deMainWifiDestPortNum = CPU_I2C.WifiDestPort;
	g_deMainWifiPtclNum = CPU_I2C.WifiPtcl;
	g_deMainWifiDvcdNum = CPU_I2C.WifiDvcd;
	g_deMainWifiDvldNum = CPU_I2C.WifiDvld;

	// 192-255(64 Bytes)
//	memcpy(g_arMainBleServUuidNum, CPU_I2C.BleServUuid, 16);
//	memcpy(g_arMainBleCharUuidNum, CPU_I2C.BleCharUuid, 16);
//	memcpy(g_arMainBleSpareUuidNum, CPU_I2C.BleSpareUuid, 16);
//	memcpy(g_arMainBlePinCodeNum, CPU_I2C.BlePincode, 16);

	// 256-1279(1024 Bytes)
	memcpy(g_arMainLogDataNum, CPU_I2C.LogData, 1024);

//	if(HAL_I2C_Master_Transmit(&hi2c4, 0x88, g_arI2c4TxBuf, 2, 10) == 0)
//	{
//
//	}
	MOD_DATA.ModuleUptime = ((uint32_t)g_arRtcUptime[0]*24)+(uint32_t)g_arRtcUptime[1];

	HAL_Delay(100);

}

void I2C1_READ(void)
{
	if(memcmp(&CPU_I2C.AccDay, g_arI2c1TxBuf+2, 6) != 0)
	{
		if(HAL_I2C_Mem_Read(&hi2c1, 0xa0, 2, 2, g_arI2c1TxBuf+2, 6, 100) == 0)
			g_deI2C1ConnFlg = 1;
		else
			g_deI2C1ConnFlg = 0;
		HAL_Delay(10);
		memcpy(&CPU_I2C.AccDay, g_arI2c1TxBuf+2, 6);
	}
	if(memcmp(&CPU_I2C.Alarm003, g_arI2c1TxBuf+8, 12) != 0)
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xa0, 8, 2, g_arI2c1TxBuf+8, 12, 100);
		HAL_Delay(10);
		memcpy(&CPU_I2C.Alarm003, g_arI2c1TxBuf+8, 12);
	}
	if(memcmp(&CPU_I2C.ModeAlarm, g_arI2c1TxBuf+21, 9) != 0)
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xa0, 21, 2, g_arI2c1TxBuf+21, 9, 100);
		HAL_Delay(10);
		memcpy(&CPU_I2C.ModeIo, g_arI2c1TxBuf+21, 9);
	}
	if(memcmp(&CPU_I2C.SndTyp, g_arI2c1TxBuf+30, 2) != 0)
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xa0, 30, 2, g_arI2c1TxBuf+30, 2, 100);
		HAL_Delay(10);
		memcpy(&CPU_I2C.SndTyp, g_arI2c1TxBuf+30, 2);
	}
	if(memcmp(&CPU_I2C.AlarmLifetime, g_arI2c1TxBuf+32, 32) != 0)
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xa0, 32, 2, g_arI2c1TxBuf+32, 32, 100);
		HAL_Delay(10);
		memcpy(&CPU_I2C.AlarmLifetime, g_arI2c1TxBuf+32, 32);
	}
	if(memcmp(&CPU_I2C.Rs485Baud, g_arI2c1TxBuf+64, 14) != 0)
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xa0, 64, 2, g_arI2c1TxBuf+64, 14, 100);
		HAL_Delay(10);
		memcpy(&CPU_I2C.Rs485Baud, g_arI2c1TxBuf+64, 14);
	}
	if(memcmp(&CPU_I2C.LanModIp[0], g_arI2c1TxBuf+78, 30) != 0)
	{
		HAL_I2C_Mem_Read(&hi2c1, 0xa0, 78, 2, g_arI2c1TxBuf+78, 30, 100);
		HAL_Delay(10);
		memcpy(&CPU_I2C.LanModIp[0], g_arI2c1TxBuf+78, 30);
	}
}

void I2C1_WRITE(void)
{
	if(memcmp(g_arI2c1TxBuf+2, &CPU_I2C.AccDay, 6) != 0)
	{
		memcpy(g_arI2c1TxBuf+2, &CPU_I2C.AccDay, 6);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 2, 2, g_arI2c1TxBuf+2, 6, 100);
		HAL_Delay(10);
	}
	if(memcmp(g_arI2c1TxBuf+8, &CPU_I2C.Alarm003, 12) != 0)
	{
		memcpy(g_arI2c1TxBuf+8, &CPU_I2C.Alarm003, 12);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 8, 2, g_arI2c1TxBuf+8, 12, 100);
		HAL_Delay(10);
	}
	if(memcmp(g_arI2c1TxBuf+20, &CPU_I2C.LogCnt, 1) != 0)
	{
		memcpy(g_arI2c1TxBuf+20, &CPU_I2C.LogCnt, 1);
		memcpy(g_arI2c1TxBuf+(256+(uint16_t)(CPU_I2C.LogCnt << 3)), CPU_I2C.LogData[CPU_I2C.LogCnt], 8);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 20, 2, g_arI2c1TxBuf+20, 1, 100);
		HAL_Delay(10);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 256+(uint16_t)(CPU_I2C.LogCnt << 3), 2, g_arI2c1TxBuf+(256+(uint16_t)(CPU_I2C.LogCnt << 3)), 8, 100);
		HAL_Delay(10);
	}
	if(memcmp(g_arI2c1TxBuf+21, &CPU_I2C.ModeAlarm, 9) != 0)
	{
		memcpy(g_arI2c1TxBuf+21, &CPU_I2C.ModeAlarm, 9);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 21, 2, g_arI2c1TxBuf+21, 9, 100);
		HAL_Delay(10);
	}
	if(memcmp(g_arI2c1TxBuf+30, &CPU_I2C.SndTyp, 2) != 0)
	{
		memcpy(g_arI2c1TxBuf+30, &CPU_I2C.SndTyp, 2);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 30, 2, g_arI2c1TxBuf+30, 2, 100);
		HAL_Delay(10);
	}
	if(memcmp(g_arI2c1TxBuf+32, &CPU_I2C.AlarmLifetime, 32) != 0)
	{
		memcpy(g_arI2c1TxBuf+32, &CPU_I2C.AlarmLifetime, 32);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 32, 2, g_arI2c1TxBuf+32, 32, 100);
		HAL_Delay(10);
	}
	if(memcmp(g_arI2c1TxBuf+64, &CPU_I2C.Rs485Baud, 14) != 0)
	{
		memcpy(g_arI2c1TxBuf+64, &CPU_I2C.Rs485Baud, 14);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 64, 2, g_arI2c1TxBuf+64, 14, 100);
		HAL_Delay(10);
	}
	if(memcmp(g_arI2c1TxBuf+78, &CPU_I2C.LanModIp[0], 30) != 0)
	{
		memcpy(g_arI2c1TxBuf+78, &CPU_I2C.LanModIp[0], 30);
		HAL_I2C_Mem_Write(&hi2c1, 0xa1, 78, 2, g_arI2c1TxBuf+78, 30, 100);
		HAL_Delay(10);
	}
}

void I2C4_READ(void)
{
//	if(g_deMainModeThNum == 1)
//	{
//		if(HAL_I2C_Master_Receive(&hi2c4, 0x88, g_arI2c4RxBuf, 6, 10) == 0)
//		{
//			if(Crc8Maxim(g_arI2c4RxBuf[1], Crc8Maxim(g_arI2c4RxBuf[0], 0xff) == g_arI2c4RxBuf[2]))
//			{
//				TH_I2C.Temperature = -45 + ((175*((float)(((uint16_t)g_arI2c4RxBuf[0] << 8) | ((uint16_t)g_arI2c4RxBuf[1])))) / 65535);
//				TH_I2C.Temperature = ((float)g_deMainThTempSlpNum)/100*TH_I2C.Temperature+((float)g_deMainThTempOffNum)/100;
//			}
//			if(Crc8Maxim(g_arI2c4RxBuf[5], Crc8Maxim(g_arI2c4RxBuf[4], 0xff) == g_arI2c4RxBuf[6]))
//			{
//				TH_I2C.Humidity = (100*((float)(((uint16_t)g_arI2c4RxBuf[3] << 8) | ((uint16_t)g_arI2c4RxBuf[4])))) / 65535;
//				TH_I2C.Humidity = ((float)g_deMainThHumiSlpNum)/100*TH_I2C.Humidity+((float)g_deMainThHumiOffNum)/100;
//			}
//
//			g_deI2C4ConnFlg = 1;
//			g_deI2C4ConnCnt = 0;
//		}
//	}
}

void I2C4_WRITE(void)
{
//	if(g_deMainModeThNum == 1)
//	{
//		if(HAL_I2C_Master_Transmit(&hi2c4, 0x88, g_arI2c4TxBuf, 2, 10) != 0)
//		{
//			HAL_I2C_Master_Transmit(&hi2c4, 0x00, g_arI2c4TxBuf+2, 2, 10);
//			HAL_I2C_Master_Transmit(&hi2c4, 0x88, g_arI2c4TxBuf, 2, 10);
//		}
//	}
}
