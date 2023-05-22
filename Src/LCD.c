//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	LCD.c
//	MAKE	BY		:
//	DESCRIPTION		:	tft lcd module control functions
//	CREATED DATE	:	0000.00.00
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"
#include "lwip.h"
#include "lwip/netif.h"

// 배경 및 문자 출력 선택
#define MAIN_STATE			(1)
#define SETTING_STATE		(2)
#define DETAILS_STATE		(3)

// Details Changed Page Number
#define	PAGE_1_OF_2	(((g_deDetSelStt >= 1) && (g_deDetSelStt <= 6)) && ((g_deDetSelStt_k < 1) || (g_deDetSelStt_k > 6)))
#define	PAGE_2_OF_2	((((g_deDetSelStt >= 7) && (g_deDetSelStt <= 12)) || (g_deDetSelStt == 0)) && ((g_deDetSelStt_k < 7) && (g_deDetSelStt_k > 0)))
#define	PAGE_1_OF_3	(((g_deDetSelStt >= 1) && (g_deDetSelStt <= 6)) && ((g_deDetSelStt_k < 1) || (g_deDetSelStt_k > 6)))
#define	PAGE_2_OF_3	(((g_deDetSelStt >= 7) && (g_deDetSelStt <= 12)) && ((g_deDetSelStt_k < 7) || (g_deDetSelStt_k > 12)))
#define	PAGE_3_OF_3	((((g_deDetSelStt >= 13) && (g_deDetSelStt <= 18)) || (g_deDetSelStt == 0)) && ((g_deDetSelStt_k < 13) && (g_deDetSelStt_k > 0)))

// for Point of Cursor
const uint16_t g_arBgColor[5+1] = {WHITE, RED, ORANGE, YELLOW, GREEN, BLUE};

const uint16_t g_arMainCursorX[4+1] = {0, 32, 112, 192, 272};
const uint16_t g_arSetCursorY[10+1] = {194, 50, 86, 122, 158, 194, 50, 86, 122, 158, 194};
const uint16_t g_arDetCursorX[1+1] = {0, 12};
const uint16_t g_arDetCursorY[18+1] = {204, 60, 84, 108, 132, 156, 180, 60, 84, 108, 132, 156, 180, 60, 84, 108, 132, 156, 180};
const uint16_t g_arCharCursorX[25+1] = {0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 192, 204, 216, 228, 240, 252, 264, 276, 288, 300};
const uint16_t g_arCharCursorY[18+1] = {0, 80, 104, 128, 152, 176, 200, 80, 104, 128, 152, 176, 200, 80, 104, 128, 152, 176, 200};

uint16_t g_deChCursor = BLACK;
const uint16_t g_arCharX[25+1] = {0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 192, 204, 216, 228, 240, 252, 264, 276, 288, 300};
const uint16_t g_arCharY[18+1] = {204, 60, 84, 108, 132, 156, 180, 60, 84, 108, 132, 156, 180, 60, 84, 108, 132, 156, 180};	// 1) ~ 18)
const uint16_t g_arCharSizeX[26] = {0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156, 168, 180, 192, 204, 216, 228, 240, 252, 264, 276, 288, 300};
const uint16_t g_arCharSizeY[4] = {0, 24, 48, 72};

const uint16_t g_arChAlarmX[5] = {0, 164, 176, 188, 200};
const uint16_t g_arChAlarmY[7] = {0, 80, 104, 128, 152, 176, 200};		// 1, 2, 3, 4, 5, 6
const uint16_t g_arChDateTimeX[7] = {0, 56, 68, 92, 104, 128, 140};
const uint16_t g_arChDateTimeY[4] = {0, 104, 0, 152};						// 1, 3
const uint16_t g_arChNetworkX[13] = {0, 128+12*0, 128+12*1, 128+12*2, 128+12*4, 128+12*5, 128+12*6, 128+12*8, 128+12*9, 128+12*10, 128+12*12, 128+12*13, 128+12*14};
const uint16_t g_arChNetMacX[13] = {0, 56+12*0, 56+12*1, 56+12*3, 56+12*4, 56+12*6, 56+12*7, 56+12*9, 56+12*10, 56+12*12, 56+12*13, 56+12*15, 56+12*16};
const uint16_t g_arChNetworkY[5] = {0, 80, 104, 128, 176};					// 1, 2, 3, 4

const uint16_t g_arSetAlMax[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9000, 9900, 9990, 9999};
const uint16_t g_arSetAlMin[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 999, 99, 9, 0};
const uint16_t g_arSetAlSum[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 100, 10, 1};

	  uint16_t g_arSetDateMax[11+1] = {0, 0, 0, 0, 90, 99, 0, 3, 12, 0, 22, 31};
const uint16_t g_arSetDateMin[11+1] = {0, 0, 0, 0, 9, 0, 0, 10, 1, 0, 10, 1};
const uint16_t g_arSetDateSum[11+1] = {0, 0, 0, 0, 10, 1, 0, 10, 1, 0, 10, 1};
const uint16_t g_arSetTimeMax[11+1] = {0, 0, 0, 0, 14, 23, 0, 50, 59, 0, 50, 59};
const uint16_t g_arSetTimeMin[11+1] = {0, 0, 0, 0, 9, 0, 0, 9, 0, 0, 9, 0};
const uint16_t g_arSetTimeSum[11+1] = {0, 0, 0, 0, 10, 1, 0, 10, 1, 0, 10, 1};
const uint32_t g_arSetLifeMax[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 400000, 490000, 499000, 499900, 499990, 499999};
const uint32_t g_arSetLifeMin[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99999, 9999, 999, 99, 9, 0};
const uint32_t g_arSetLifeSum[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100000, 10000, 1000, 100, 10, 1};

const uint16_t g_arSetDvMax[19+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40000, 49000, 49900, 49990, 49999};
const uint16_t g_arSetDvMin[19+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9999, 999, 99, 9, 0};
const uint16_t g_arSetDvSum[19+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 1000, 100, 10, 1};

const uint8_t g_arSetAddrMax[23+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 156, 246, 255, 0, 156, 246, 255, 0, 156, 246, 255, 0, 156, 246, 255};
const uint8_t g_arSetAddrMin[23+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 9, 0, 0, 99, 9, 0, 0, 99, 9, 0, 0, 99, 9, 0};
const uint8_t g_arSetAddrSum[23+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 10, 1, 0, 100, 10, 1, 0, 100, 10, 1, 0, 100, 10, 1};
const uint16_t g_arSetPortMax[13+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 55536, 64536, 65436, 65526, 65525};
const uint16_t g_arSetPortMin[13+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 9999, 999, 99, 9, 0};
const uint16_t g_arSetPortSum[13+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 1000, 100, 10, 1};
const uint8_t g_arSetMacMax[25+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 240, 255, 0, 240, 255, 0, 240, 255, 0, 240, 255, 0, 240, 255, 0, 240, 255};
const uint8_t g_arSetMacMin[25+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 15, 0, 0, 15, 0, 0, 15, 0, 0, 15, 0, 0, 15, 0};
const uint8_t g_arSetMacSum[25+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 1, 0, 16, 1, 0, 16, 1, 0, 16, 1, 0, 16, 1, 0, 16, 1};
//									    0  1  2  3  4  5  6  7 8   9 10     11      12     13       14      15 16 17 18 19    20    21   22   23
const int16_t g_arSetSlpOffMax[25+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	400, 	  0,	490, 	499, 	0, 0, 0, 0,  4000, 4900, 0, 4990, 4999};
const int16_t g_arSetSlpOffMin[25+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	100, 	  0,	10, 	  1, 	0, 0, 0, 0,  -4000, -4900, 0, -4990, -4999};
const int16_t g_arSetSlpOffSum[25+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	100, 	  0,	10, 	  1, 	0, 0, 0, 0,   1000,   100, 0, 10, 1};

// for Selecting Background State
uint8_t g_deBkgdStt = 1;			// 0-Nothing, 1-Main, 2-Setting, 3- Details(Current State)
uint8_t g_deMainSelStt = 2;			// 1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 7-Status, 8-Log1, 9-Log2, 10-Log3, 11-Version
uint8_t g_deSetSelStt = 1;			// 1-Alarm, 2-Date/time, 3-Mode, 4-Network, 5-Sound
uint8_t g_deDetSelStt = 1;			// 1.1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 2.1~3-Date, 4~6-Time, 3.1-IP, 2-Subnet, 3-Gateway, 4-MAC, 4.1-Off/Beep/Melody, 2-Volume, 5.1-Moving Average, 2-Unit, 3-Wifi
uint8_t g_deChSelStt = 0;			// Character(ASCII Code, 1 is Most Significant)

uint8_t g_deBkgdStt_k = 2;			// 0-Nothing, 1-Main, 2-Setting, 3- Setting Details(Previous State)
uint8_t g_deMainSelStt_k = 3;		// 1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 7-Status, 8-Log1, 9-Log2, 10-Log3, 11-Attachment, 12-Version
uint8_t g_deSetSelStt_k = 10;		// 1-Alarm, 2-Date/time, 3-Mode, 4-Network, 5-Sound
uint8_t g_deDetSelStt_k = 10;		// 1.1-PM0.3, 2-PM0.5, 3-PM1.0, 4-PM2.5, 5-PM5.0, 6-PM10, 2.1~3-Date, 4~6-Time, 3.1-IP, 2-Subnet, 3-Gateway, 4-MAC, 4.1-Off/Beep/Melody, 2-Volume, 5.1-Moving Average, 2-Unit, 3-Wifi
uint8_t g_deChSelStt_k = 10;		// Character(ASCII Code, 1 is Most Significant)

// for Main Page
uint8_t g_deMainBlockStt = 2;
uint8_t g_deMainLvClrStt = 1;
uint8_t g_arMainColorStt[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3,};
uint8_t g_arMainColorStt_k[16] = {0, };
uint8_t g_arMainBgClrStt[16] = {0, };
uint8_t g_arMainBgClrStt_k[16] = {0, };

uint8_t g_deMainFanStt = 0;
uint8_t g_deMainCommStt = 0;
uint8_t g_deMainThStt = 0;

int8_t g_arMainFanDateAsc[8] = {0, };
int8_t g_arMainFanTimeAsc[8] = {0, };
int8_t g_arMainCommDateAsc[8] = {0, };
int8_t g_arMainCommTimeAsc[8] = {0, };
//int8_t g_arMainThDateAsc[8] = {0, };
//int8_t g_arMainThTimeAsc[8] = {0, };

uint16_t g_deMainAl003Flg = 0;
uint16_t g_deMainAl005Flg = 0;
uint16_t g_deMainAl010Flg = 0;
uint16_t g_deMainAl025Flg = 0;
uint16_t g_deMainAl050Flg = 0;
uint16_t g_deMainAl100Flg = 0;

uint16_t g_deMainAl003Flg_k = 0;
uint16_t g_deMainAl005Flg_k = 0;
uint16_t g_deMainAl010Flg_k = 0;
uint16_t g_deMainAl025Flg_k = 0;
uint16_t g_deMainAl050Flg_k = 0;
uint16_t g_deMainAl100Flg_k = 0;

uint8_t g_deMainModeDefFlg = 0;

uint16_t g_arMainAlarmNum[6+1] = {0, };

uint8_t g_arMainAccTimeNum[6+1] = {0, };

uint32_t g_deMainLifetimeNum = 51200;
uint8_t g_deMainLogCodeNum = 0;
uint8_t g_deMainLogCntNum = 0;

uint8_t g_deMainModeAlarmNum = 0;
uint8_t g_deMainModeIoNum = 0;
uint8_t g_deMainMode485Num = 0;
uint8_t g_deMainModeLanNum = 0;
uint8_t g_deMainModeThNum = 0;
uint8_t g_deMainModeWlssNum = 0;
uint8_t g_deMainModeSmaNum = 1;
uint8_t g_deMainModeDefNum = 0;

uint32_t g_deMain485BaudNum = 9600;
uint8_t g_deMain485DataNum = 8;
uint8_t g_deMain485PrtyNum = 0;
uint8_t g_deMain485StopNum = 1;
uint8_t g_deMain485FlowNum = 0;
uint8_t g_deMain485ModNum = 0;
uint8_t g_deMain485DestNum = 0;
uint8_t g_deMain485PtclNum = 1;
uint8_t g_deMain485DvcdNum = 0xa8;
uint16_t g_deMain485DvldNum = 0;

uint8_t g_arMainLanModIpNum[4] = {192, 168, 0, 1};
uint8_t g_arMainLanModSubNum[4] = {255, 255, 255, 0};
uint8_t g_arMainLanModGwNum[4] = {192, 168, 0, 1};
uint8_t g_arMainLanModMacNum[6] = {1, 2, 3, 4, 5, 6};
uint16_t g_deMainLanModPortNum = 7;
uint8_t g_arMainLanDestIpNum[4] = {192, 168, 0, 11};
uint16_t g_deMainLanDestPortNum = 7;
uint8_t g_deMainLanPtclNum = 0;
uint8_t g_deMainLanDvcdNum = 0;
uint16_t g_deMainLanDvldNum = 0;

uint8_t g_arMainWifiSsidNum[16] = {0, };
uint8_t g_arMainWifiPswdNum[16] = {0, };
uint8_t g_arMainWifiModIpNum[4] = {192, 168, 0, 1};
uint8_t g_arMainWifiModSubNum[4] = {255, 255, 255, 0};
uint8_t g_arMainWifiModGwNum[4] = {192, 168, 0, 1};
uint8_t g_arMainWifiModMacNum[6] = {1, 2, 3, 4, 5, 6};
uint16_t g_deMainWifiModPortNum = 7;
uint8_t g_arMainWifiDestIpNum[4] = {192, 168, 0, 11};
uint16_t g_deMainWifiDestPortNum = 7;
uint8_t g_deMainWifiPtclNum = 0;
uint8_t g_deMainWifiDvcdNum = 0;
uint16_t g_deMainWifiDvldNum = 0;

uint8_t g_deMainSndTypNum = 0;		// 0 - Beep, 1 - Siren
uint8_t g_deMainSndVlmNum = 0;		// 0 - Off, 1 - Low, 2 - Mid, 3 - High

uint8_t g_deMainThOtWrnNum = 60;
uint8_t g_deMainThOtErrNum = 70;
uint8_t g_deMainThOhErrNum = 80;
int16_t g_deMainThTempSlpNum = 100;
int16_t g_deMainThTempOffNum = 0;
int16_t g_deMainThHumiSlpNum = 100;
int16_t g_deMainThHumiOffNum = 0;

uint8_t g_arMainLogDataNum[128][8] = {0};

const int8_t g_arMainUppTxtAsc[13][5] = {{0,},
		{'0','.','3','u','m'}, {'0','.','5','u','m'}, {'1','.','0','u','m'}, {'2','.','5','u','m'}, {'5','.','0','u','m'}, {'1','0',' ','u','m'},
		{'E','R','R','O','R'}, {'L','O','G','-','1'}, {'L','O','G','-','2'}, {'L','O','G','-','3'}, {'E','X','P','A','N'}, {' ','V','E','R',' '}};
int8_t g_arMainUppDatAsc[13][4] = {{0,},{'0','0','0','0'},{'0','0','0','0'},{'0','0','0','0'},{'0','0','0','0'},{'0','0','0','0'},{'0','0','0','0'},};


int8_t g_arMainLogDateAsc[12+1][8] = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};
int8_t g_arMainLogTimeAsc[12+1][8] = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};
int8_t g_arMainLogCodeAsc[12+1][4] = {{' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '},
		 {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' '}};
int8_t g_arMainTempAsc[4] = {'0', '0', '.', '0'};
int8_t g_arMainHumiAsc[4] = {'0', '0', '.', '0'};
// for Setting Page
uint8_t g_deSetPageStt = 1;		// 1~3
uint8_t g_deSetListStt = 2;		// [0]-SERIAL, [1]-NETWORK, [2]-TEMP,HUMI, [3]-WIRELESS
uint8_t g_deSetListMax = 4;		// 4~8
int8_t g_arSetListNumAsc[9][2] = {{0,},	{'1', '.'}, {'2', '.'}, {'3', '.'}, {'4', '.'}, {'5', '.'}, {'6', '.'}, {'7', '.'}, {'8', '.'}};
int8_t g_arSetListTxtAsc[9][9] = {{0,},
		{'A', 'L', 'A', 'R', 'M', ' ', 'R', 'E', 'F'},{'D', 'A', 'T', 'E', ',', 'T', 'I', 'M', 'E'},
		{'M', 'O', 'D', 'E', ' ', ' ', ' ', ' ', ' '},{'S', 'E', 'R', 'I', 'A', 'L', ' ', ' ', ' '},
		{'N', 'E', 'T', 'W', 'O', 'R', 'K', ' ', ' '},{'W', 'I', 'R', 'E', 'L', 'E', 'S', 'S', ' '},
		{'T', 'E', 'M', 'P', ',', 'H', 'U', 'M', 'I'},{'S', 'O', 'U', 'N', 'D', ' ', ' ', ' ', ' '}};
uint8_t g_arSetOrdrStt[8+1][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		{8, 4, 5, 4, 6, 4, 5, 4, 7, 4, 5, 4, 6, 4, 5, 4},
		{0, 8, 8, 5, 8, 6, 6, 5, 8, 7, 7, 5, 7, 6, 6, 5},
		{0, 0, 0, 8, 0, 8, 8, 6, 0, 8, 8, 7, 8, 7, 7, 6},
		{0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 8, 8, 7},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8}, };

// for Alarm Reference Details Page
uint16_t g_deSetAl003Num = 0;
uint16_t g_deSetAl005Num = 0;
uint16_t g_deSetAl010Num = 0;
uint16_t g_deSetAl025Num = 0;
uint16_t g_deSetAl050Num = 0;
uint16_t g_deSetAl100Num = 0;

uint16_t g_deSetAl003Num_k = 0;
uint16_t g_deSetAl005Num_k = 0;
uint16_t g_deSetAl010Num_k = 0;
uint16_t g_deSetAl025Num_k = 0;
uint16_t g_deSetAl050Num_k = 0;
uint16_t g_deSetAl100Num_k = 0;

// for Date, Time Details Page
uint8_t g_arSetDateNum[3] = {0, 1, 1};
uint8_t g_arSetTimeNum[3] = {0, 0, 0};
uint32_t g_deSetUptimeNum = 0;
uint32_t g_deSetLifeNum = 51200;

uint8_t g_arSetDateNum_k[3] = {0, };
uint8_t g_arSetTimeNum_k[3] = {0, };
uint32_t g_deSetLifeNum_k = 0;

// for Mode Details Page
uint8_t g_deSetModeAlarmNum = 0;	// 0-Unused, 1-Used
uint8_t g_deSetModeIoNum = 0;		// 0-Unused, 1-Used
uint8_t g_deSetMode485Num = 0;		// 0-Unused, 1-Used
uint8_t g_deSetModeLanNum = 0;		// 0-Unused, 1-Server, 2-Client, 3-Dual
uint8_t g_deSetModeWlssNum = 0;		// 0-Unused, 1-Wifi, 2-Ble, 3-Dual(server only)
uint8_t g_deSetModeThNum = 0;		// 0-Unused, 1-Used
uint8_t g_deSetModeSmaNum = 1;		// 1~64
uint8_t g_deSetModeDefNum = 0;		// 0-Unused, 1-Used

uint8_t g_deSetModeAlarmNum_k = 0;
uint8_t g_deSetModeIoNum_k = 0;
uint8_t g_deSetMode485Num_k = 0;
uint8_t g_deSetModeLanNum_k = 0;
uint8_t g_deSetModeWlssNum_k = 0;
uint8_t g_deSetModeThNum_k = 0;
uint8_t g_deSetModeSmaNum_k = 1;
uint8_t g_deSetModeDefNum_k = 0;

// for Serial Details Page
uint32_t g_deSet485BaudNum = 9600;
uint8_t g_deSet485DataNum = 8;
uint8_t g_deSet485PrtyNum = 0;
uint8_t g_deSet485StopNum = 1;
uint8_t g_deSet485FlowNum = 1;
uint8_t g_deSet485ModNum = 0;
uint8_t g_deSet485DestNum = 0;
uint8_t g_deSet485PtclNum = 0;
uint8_t g_deSet485DvcdNum = 0xa8;
uint16_t g_deSet485DvldNum = 0;

uint32_t g_deSet485BaudNum_k = 9600;
uint8_t g_deSet485DataNum_k = 8;
uint8_t g_deSet485PrtyNum_k = 0;
uint8_t g_deSet485StopNum_k = 1;
uint8_t g_deSet485FlowNum_k = 0;
uint8_t g_deSet485ModNum_k = 0;
uint8_t g_deSet485DestNum_k = 0;
uint8_t g_deSet485PtclNum_k = 1;
uint8_t g_deSet485DvcdNum_k = 0xa8;
uint16_t g_deSet485DvldNum_k = 0;

// for Network Details Page
uint8_t g_arSetLanModIpNum[4] = {192, 168, 0, 1};
uint8_t g_arSetLanModSubNum[4] = {255, 255, 255, 0};
uint8_t g_arSetLanModGwNum[4] = {192, 168, 0, 1};
uint8_t g_arSetLanModMacNum[6] = {1, 2, 3, 4, 5, 6};
uint16_t g_deSetLanModPortNum = 7;
uint8_t g_arSetLanDestIpNum[4] = {192, 168, 0, 11};
uint16_t g_deSetLanDestPortNum = 7;
uint8_t g_deSetLanPtclNum = 0;
uint8_t g_deSetLanDvcdNum = 0xa8;
uint16_t g_deSetLanDvldNum = 0;

uint8_t g_arSetLanModIpNum_k[4] = {0, };
uint8_t g_arSetLanModSubNum_k[4] = {0, };
uint8_t g_arSetLanModGwNum_k[4] = {0, };
uint16_t g_deSetLanModPortNum_k = 0;
uint8_t g_arSetLanModMacNum_k[6] = {0, };
uint8_t g_arSetLanDestIpNum_k[4] = {0, };
uint16_t g_deSetLanDestPortNum_k = 0;
uint8_t g_deSetLanPtclNum_k = 0;
uint8_t g_deSetLanDvcdNum_k = 0xa8;
uint16_t g_deSetLanDvldNum_k = 0;

// for Sound Details Page
uint8_t g_deSetSndTypNum = 0;		// 0 - Beep, 1 - Siren
uint8_t g_deSetSndVlmNum = 0;		// 0 - Off, 1 - Low, 2 - Mid, 3 - High

uint8_t g_deSetSndTypNum_k = 0;
uint8_t g_deSetSndVlmNum_k = 0;

// for Sound Details Page
uint8_t g_deSetThOtErrNum = 70;
uint8_t g_deSetThOtWrnNum = 60;
uint8_t g_deSetThOhErrNum = 80;
int16_t g_deSetThTempSlpNum = 100;
int16_t g_deSetThTempOffNum = 0;
int16_t g_deSetThHumiSlpNum = 100;
int16_t g_deSetThHumiOffNum = 0;

uint8_t g_deSetThOtErrNum_k = 0;
uint8_t g_deSetThOtWrnNum_k = 0;
uint8_t g_deSetThOhErrNum_k = 0;
int16_t g_deSetThTempSlpNum_k = 100;
int16_t g_deSetThTempOffNum_k = 0;
int16_t g_deSetThHumiSlpNum_k = 100;
int16_t g_deSetThHumiOffNum_k = 0;

// for Details Page(ASCII Code)
int8_t g_arSetAl003Asc[4] = {' ', ' ', ' ', '0'};
int8_t g_arSetAl005Asc[4] = {' ', ' ', ' ', '0'};
int8_t g_arSetAl010Asc[4] = {' ', ' ', ' ', '0'};
int8_t g_arSetAl025Asc[4] = {' ', ' ', ' ', '0'};
int8_t g_arSetAl050Asc[4] = {' ', ' ', ' ', '0'};
int8_t g_arSetAl100Asc[4] = {' ', ' ', ' ', '0'};

int8_t g_arSetDateAsc[8] = {'0', '0', '-', ' ', '1', '-', ' ', '1'};
int8_t g_arSetTimeAsc[8] = {' ', '0', ':', ' ', '0', ':', ' ', '0'};
int8_t g_arSetUpTimeAsc[6] = {' ', ' ', ' ', ' ', ' ', '0'};
int8_t g_arSetLifeTimeAsc[6] = {' ', ' ', ' ', ' ', ' ', '0'};

int8_t g_arSetModeAlarmAsc[8] = {'U', 'n', 'u', 's', 'e', 'd', ' ', ' '};		// 0-Unused, 1-Used
int8_t g_arSetModeIoAsc[8] = {'U', 'n', 'u', 's', 'e', 'd', ' ', ' '};		// 0-Unused, 1-Used
int8_t g_arSetMode485Asc[8] = {'U', 'n', 'u', 's', 'e', 'd', ' ', ' '};		// 0-Unused, 1-Used
int8_t g_arSetModeLanAsc[8] = {'U', 'n', 'u', 's', 'e', 'd', ' ', ' '};		// 0-Unused, 1-Server, 2-Client, 3-Dual
int8_t g_arSetModeWlssAsc[8] = {'U', 'n', 'u', 's', 'e', 'd', ' ', ' '};	// 0-Unused, 1-Wifi, 2-Ble, 3-Dual(server only)
int8_t g_arSetModeThAsc[8] = {'U', 'n', 'u', 's', 'e', 'd', ' ', ' '};		// 0-Unused, 1-Used
int8_t g_arSetModeSmaAsc[2] = {' ', '1'};
int8_t g_arSetModeDefAsc[8] = {'U', 'n', 'u', 's', 'e', 'd', ' ', ' '};		// 0-Unused, 1-Used

int8_t g_arSet485BaudAsc[6] = {' ', ' ', '9', '6', '0', '0'};
int8_t g_arSet485DataAsc[1] = {'8'};
int8_t g_arSet485PrtyAsc[4] = {'N', 'o', 'n', 'e'};
int8_t g_arSet485StopAsc[1] = {'1'};
int8_t g_arSet485FlowAsc[4] = {'N', 'o', 'n', 'e'};
int8_t g_arSet485ModAsc[2] = {'0', '0'};
int8_t g_arSet485DestAsc[2] = {'0', '0'};
int8_t g_arSet485PtclAsc[8] = {'A', 'C', 'T', ' ', ' ', ' ', ' ', ' '};		// 0-ACT, 1-MC fmt5
int8_t g_arSet485DvcdAsc[1] = {'D'};										// 0-D, 1-W
int8_t g_arSet485DvldAsc[5] = {' ', ' ', ' ', ' ', '0'};

int8_t g_arSetLanModIpAsc[15] = {'1', '9', '2', '.', '1', '6', '8', '.', ' ', ' ', '0', '.', ' ', ' ', '1'};
int8_t g_arSetLanModSubAsc[15] = {'2', '5', '5', '.', '2', '5', '5', '.', '2', '5', '5', '.', ' ', ' ', '0'};
int8_t g_arSetLanModGwAsc[15] = {'1', '9', '2', '.', '1', '6', '8', '.', ' ', ' ', '0', '.', ' ', ' ', '1'};
int8_t g_arSetLanModPortAsc[5] = {'0','0','0','0','0'};
int8_t g_arSetLanModMacAsc[17] = {'0', '1', ':', '0', '2', ':', '0', '3', ':', '0', '4', ':', '0', '5', ':', '0', '6'};
int8_t g_arSetLanDestIpAsc[15] = {'1', '9', '2', '.', '1', '6', '8', '.', ' ', ' ', '0', '.', ' ', '1', '1'};
int8_t g_arSetLanDestPortAsc[5] = {'0','0','0','0','0'};
int8_t g_arSetLanPtclAsc[6] = {'A', 'C', 'T', ' ', ' ', ' '};
int8_t g_arSetLanDvcdAsc[1] = {'D'};
int8_t g_arSetLanDvldAsc[5] = {' ', ' ', ' ', ' ', '0'};

int8_t g_arSetSndTypAsc[6] = {'B', 'e', 'e', 'p', ' '};
int8_t g_arSetSndVlmAsc[4] = {'O', 'f', 'f', ' '};

int8_t g_arSetThOtErrAsc[2] = {'7', '0'};
int8_t g_arSetThOtWrnAsc[2] = {'6', '0'};
int8_t g_arSetThOhErrAsc[2] = {'8', '0'};
int8_t g_arSetThTempSlpAsc[4] = {'1', '.', '0', '0'};
int8_t g_arSetThTempOffAsc[6] = {'+', '0', '0', '.', '0', '0'};
int8_t g_arSetThHumiSlpAsc[4] = {'1', '.', '0', '0'};
int8_t g_arSetThHumiOffAsc[6] = {'+', '0', '0', '.', '0', '0'};

// ICD CMD5 모드 설정 화면 갱신 플래그
uint8_t g_deRedrawingFlg = 0;

// TOP text 배경 색상(COLOR : RGB565)
uint16_t u2f_b_text_color = 0;

uint8_t ExternalInput1Inv_k = 1;

// Structures



// Functions
void LCD_INIT(void);
void LCD_UPDATE(void);

void LCD_MainDisplay(void);
void LCD_SettingDisplay(void);
void LCD_DetailsAlarmDisplay(void);
void LCD_DetailsDatetimeDisplay(void);
void LCD_DetailsModeDisplay(void);
void LCD_DetailsSerialDisplay(void);
void LCD_DetailsNetworkDisplay(void);
void LCD_DetailsWirelessDisplay(void);
void LCD_DetailsTemphumiDisplay(void);
void LCD_DetailsSoundDisplay(void);

void LCD_MainStatus(void);
void LCD_SettingStatus(void);
void LCD_DetailsAlarmStatus(void);
void LCD_DetailsDatetimeStatus(void);
void LCD_DetailsModeStatus(void);
void LCD_DetailsSerialStatus(void);
void LCD_DetailsNetworkStatus(void);
void LCD_DetailsWirelessStatus(void);
void LCD_DetailsTemphumiStatus(void);
void LCD_DetailsSoundStatus(void);

void LCD_INIT(void)
{
	Ili9341_Reset();
	Ili9341_Preset();
	Ili9341_Rotate(270);

	g_deSetListStt = ((!!g_deMainModeThNum) << 3) | ((!!g_deMainModeWlssNum) << 2) | ((!!g_deMainModeLanNum) << 1) | ((!!g_deMainMode485Num) << 0);
	g_deSetListMax = 4+(!!g_deMainModeThNum)+(!!g_deMainModeWlssNum)+(!!g_deMainModeLanNum)+(!!g_deMainMode485Num);

	g_deSetPageStt = 1;
}

void LCD_UPDATE(void)
{
	switch(g_deBkgdStt)
	{
		case MAIN_STATE :								// If Background is Main Page
			LCD_MainDisplay();
			LCD_MainStatus();
			break;
		case SETTING_STATE :							// If Background is Setting Page
			LCD_SettingDisplay();
			LCD_SettingStatus();
			break;
		case DETAILS_STATE :							// If Background is Details Page
			switch(g_arSetOrdrStt[g_deSetSelStt][g_deSetListStt])
			{
				case 1 :
					LCD_DetailsAlarmDisplay();
					LCD_DetailsAlarmStatus();
					break;
				case 2 :
					LCD_DetailsDatetimeDisplay();
					LCD_DetailsDatetimeStatus();
					break;
				case 3 :
					LCD_DetailsModeDisplay();
					LCD_DetailsModeStatus();
					break;
				case 4 :
					LCD_DetailsSerialDisplay();
					LCD_DetailsSerialStatus();
					break;
				case 5 :
					LCD_DetailsNetworkDisplay();
					LCD_DetailsNetworkStatus();
					break;
				case 6 :
					LCD_DetailsWirelessDisplay();
					LCD_DetailsWirelessStatus();
					break;
				case 7 :
					LCD_DetailsTemphumiDisplay();
					LCD_DetailsTemphumiStatus();
					break;
				case 8 :
					LCD_DetailsSoundDisplay();
					LCD_DetailsSoundStatus();
					break;
				default :
					break;
			}
			break;
		default :
			break;
	}
	if(g_deScrnSaveFlg_k == 1)
	{
		if(g_deScrnSaveFlg == 0)
		{
			Ili9341_Display(1);			// Display On
			g_deScrnSaveFlg_k = 0;		// Equal to Screen Saver is not Working
		}
	}
	if(g_deScrnSaveFlg_k == 0)
	{
		if(g_deScrnSaveFlg == 1)
		{
			Ili9341_Display(0);			// Display Off
			g_deScrnSaveFlg_k = 1;		// Equal to Screen Saver is Working
		}
	}
}

void LCD_MainDisplay(void)
{
	// Background Main Page
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		if(g_deMainSelStt == g_deMainSelStt_k)	// After Second Time
		{
			// Upper Side Color Box & Text
			if(g_arMainColorStt[g_deMainBlockStt+0] != g_arMainColorStt_k[g_deMainBlockStt+0])
			{
				Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	  0,	  0,	 75,	 40);
				Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+0]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	  8,	  0,	 60,	 24);
				g_arMainColorStt_k[g_deMainBlockStt+0] = g_arMainColorStt[g_deMainBlockStt+0];
			}
			if(g_arMainColorStt[g_deMainBlockStt+1] != g_arMainColorStt_k[g_deMainBlockStt+1])
			{
				Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 80,	  0,	 75,	 40);
				Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+1]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 88,	  0,	 60,	 24);
				g_arMainColorStt_k[g_deMainBlockStt+1] = g_arMainColorStt[g_deMainBlockStt+1];
			}
			if(g_arMainColorStt[g_deMainBlockStt+2] != g_arMainColorStt_k[g_deMainBlockStt+2])
			{
				Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	160,	  0,	 75,	 40);
				Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+2]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	168,	  0,	 60,	 24);
				g_arMainColorStt_k[g_deMainBlockStt+2] = g_arMainColorStt[g_deMainBlockStt+2];
			}
			if(g_arMainColorStt[g_deMainBlockStt+3] != g_arMainColorStt_k[g_deMainBlockStt+3])
			{
				Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	240,	  0,	 75,	 40);
				Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+3]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	248,	  0,	 60,	 24);
				g_arMainColorStt_k[g_deMainBlockStt+3] = g_arMainColorStt[g_deMainBlockStt+3];
			}
			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+0]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	 15,	 18,	 48,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+1]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 95,	 18,	 48,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+2]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	175,	 18,	 48,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+3]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	255,	 18,	 48,	 24);

			// Left Side Arrow
			if((g_deMainSelStt <= 6))
			{
				switch(g_deMainLvClrStt)
				{
					case 1 :
						Ili9341_HallowItri(WHITE,	  0,	 10,	102,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	132,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	162,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	192,	 11,	 16);
						Ili9341_HallowItri(BLACK,	  0,	 10,	 72,	 11,	 16);
						break;
					case 2 :
						Ili9341_HallowItri(WHITE,	  0,	 10,	 72,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	132,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	162,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	192,	 11,	 16);
						Ili9341_HallowItri(BLACK,	  0,	 10,	102,	 11,	 16);
						break;
					case 3 :
						Ili9341_HallowItri(WHITE,	  0,	 10,	 72,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	102,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	162,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	192,	 11,	 16);
						Ili9341_HallowItri(BLACK,	  0,	 10,	132,	 11,	 16);
						break;
					case 4 :
						Ili9341_HallowItri(WHITE,	  0,	 10,	 72,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	102,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	132,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	192,	 11,	 16);
						Ili9341_HallowItri(BLACK,	  0,	 10,	162,	 11,	 16);
						break;
					case 5 :
						Ili9341_HallowItri(WHITE,	  0,	 10,	 72,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	102,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	132,	 11,	 16);
						Ili9341_HallowItri(WHITE,	  0,	 10,	162,	 11,	 16);
						Ili9341_HallowItri(BLACK,	  0,	 10,	192,	 11,	 16);
						break;
					default :
						break;
				}
			}

			// Center Color Box & Text
			if(g_arMainBgClrStt[g_deMainSelStt] != g_arMainBgClrStt_k[g_deMainSelStt])
			{
				Ili9341_FilledRect((g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 60,	 65,	255,	150);	//
				g_arMainBgClrStt_k[g_deMainSelStt] = g_arMainBgClrStt[g_deMainSelStt];
			}

			switch(g_deMainSelStt)
			{
				case 2 :	// PM0.5
					Ili9341_HallowRect(BLACK,	 60,	 65,	255,	150);
					Ili9341_Ascii((int8_t*)"0.5",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	 80,	 90,	60);
					Ili9341_Ascii((int8_t*)"um",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	175,	 80,	 60,	60);
					Ili9341_Ascii((int8_t*)PCnt.Pm005,	 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	150,	240,	60);
					break;
				case 3 :	// PM1.0
					Ili9341_HallowRect(BLACK,	 60,	 65,	255,	150);
					Ili9341_Ascii((int8_t*)"1.0",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	 80,	 90,	60);
					Ili9341_Ascii((int8_t*)"um",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	175,	 80,	 60,	60);
					Ili9341_Ascii((int8_t*)PCnt.Pm010,	 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	150,	240,	60);
					break;
				case 4 :	// PM2.5
					Ili9341_HallowRect(BLACK,	 60,	 65,	255,	150);
					Ili9341_Ascii((int8_t*)"2.5",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	 80,	 90,	60);
					Ili9341_Ascii((int8_t*)"um",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	175,	 80,	 60,	60);
					Ili9341_Ascii((int8_t*)PCnt.Pm025,	 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	150,	240,	60);
					break;
				case 5 :	// PM5.0
					Ili9341_HallowRect(BLACK,	 60,	 65,	255,	150);
					Ili9341_Ascii((int8_t*)"5.0",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	 80,	 90,	60);
					Ili9341_Ascii((int8_t*)"um",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	175,	 80,	 60,	60);
					Ili9341_Ascii((int8_t*)PCnt.Pm050,	 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	150,	240,	60);
					break;
				case 6 :	// PM10.
					Ili9341_HallowRect(BLACK,	 60,	 65,	255,	150);
					Ili9341_Ascii((int8_t*)" 10",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	 80,	 90,	60);
					Ili9341_Ascii((int8_t*)"um",		 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	175,	 80,	 60,	60);
					Ili9341_Ascii((int8_t*)PCnt.Pm100,	 60,	BLACK,	(g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 70,	150,	240,	60);
					break;
				case 7 :	// Error
					Ili9341_Ascii((int8_t*)"Error",			 36,	BLACK,	WHITE,	 36,	 65,	 90,	 36);
//					Ili9341_Ascii((int8_t*)"PM",			 24,	BLACK,	WHITE,	 12,	105,	 24,	 24);
					Ili9341_Ascii((int8_t*)"FAN",			 24,	BLACK,	WHITE,	 12,	105,	 36,	 24);
					Ili9341_Ascii((int8_t*)"COMM",			 24,	BLACK,	WHITE,	 12,	129,	 48,	 24);


//						if((BResult.bit.Pm005StatWarn == 1) || (BResult.bit.Pm010StatWarn == 1) || (BResult.bit.Pm025StatWarn == 1)
//								|| (BResult.bit.Pm050StatWarn == 1) || (BResult.bit.Pm100StatWarn == 1))
//						{
//							if(g_deMainPmStt == 11)
//								Ili9341_Ascii((int8_t*)"10 ",			 24,	BLACK,	WHITE,	 60,	105,	 36,	 24);
//							else if(g_deMainPmStt == 9)
//								Ili9341_Ascii((int8_t*)"5.0",			 24,	BLACK,	WHITE,	 60,	105,	 36,	 24);
//							else if(g_deMainPmStt == 7)
//								Ili9341_Ascii((int8_t*)"2.5",			 24,	BLACK,	WHITE,	 60,	105,	 36,	 24);
//							else if(g_deMainPmStt == 5)
//								Ili9341_Ascii((int8_t*)"1.0",			 24,	BLACK,	WHITE,	 60,	105,	 36,	 24);
//							else if(g_deMainPmStt == 3)
//								Ili9341_Ascii((int8_t*)"0.5",			 24,	BLACK,	WHITE,	 60,	105,	 36,	 24);
//							Ili9341_Ascii((int8_t*)&g_arMainPmDateAsc,	 24,	BLACK,	WHITE,	108,	105,	 96,	 24);
//							Ili9341_Ascii((int8_t*)&g_arMainPmTimeAsc,	 24,	BLACK,	WHITE,	216,	105,	 96,	 24);
//						}
//						else
//							Ili9341_Ascii((int8_t*)"    NONE                ",	 24,	BLACK,	WHITE,	 60,	 105,	288,	 24);

					if(BError.bit.FanAsErr == 1)
					{
						if((PM_UART.Alarm & 1) == 1)
							Ili9341_Ascii((int8_t*)"OS",			 24,	BLACK,	WHITE,	 72,	105,	 24,	 24);
						else
							Ili9341_Ascii((int8_t*)"US",			 24,	BLACK,	WHITE,	 72,	105,	 24,	 24);
						Ili9341_Ascii((int8_t*)&g_arMainFanDateAsc,	 24,	BLACK,	WHITE,	108,	105,	 96,	 24);
						Ili9341_Ascii((int8_t*)&g_arMainFanTimeAsc,	 24,	BLACK,	WHITE,	216,	105,	 96,	 24);
					}
					else
						Ili9341_Ascii((int8_t*)"   NONE                ",	 24,	BLACK,	WHITE,	 72,	105,	276,	 24);

					if(BError.bit.PmConnErr == 1)
					{
						Ili9341_Ascii((int8_t*)&g_arMainCommDateAsc,	 24,	BLACK,	WHITE,	108,	129,	 96,	 24);
						Ili9341_Ascii((int8_t*)&g_arMainCommTimeAsc,	 24,	BLACK,	WHITE,	216,	129,	 96,	 24);
					}
					else
						Ili9341_Ascii((int8_t*)"  NONE                 ",	 24,	BLACK,	WHITE,	 84,	129,	264,	 24);

//					else		// if Alarm Disabled
//					{
//						Ili9341_Ascii((int8_t*)"    NONE                ",	 24,	BLACK,	WHITE,	 60,	105,	288,	 24);
//						Ili9341_Ascii((int8_t*)"   NONE                ",	 24,	BLACK,	WHITE,	 72,	129,	276,	 24);
//						Ili9341_Ascii((int8_t*)"  NONE                 ",	 24,	BLACK,	WHITE,	 84,	153,	264,	 24);
//					}
					break;
				case 8 :	// Log1
					Ili9341_Ascii((int8_t*)"Log (1/3)",		 36,	BLACK,	WHITE,	 36,	 65,	162,	 36);
					Ili9341_Ascii((int8_t*)"01)",			 24,	BLACK,	WHITE,	 12,	105,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[0],	 24,	BLACK,	WHITE,	 48,	105,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[0],	 24,	BLACK,	WHITE,	108,	105,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[0],	 24,	BLACK,	WHITE,	216,	105,	 96,	 24);
					Ili9341_Ascii((int8_t*)"02)",			 24,	BLACK,	WHITE,	 12,	129,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[1],	 24,	BLACK,	WHITE,	 48,	129,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[1],	 24,	BLACK,	WHITE,	108,	129,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[1],	 24,	BLACK,	WHITE,	216,	129,	 96,	 24);
					Ili9341_Ascii((int8_t*)"03)",			 24,	BLACK,	WHITE,	 12,	153,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[2],	 24,	BLACK,	WHITE,	 48,	153,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[2],	 24,	BLACK,	WHITE,	108,	153,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[2],	 24,	BLACK,	WHITE,	216,	153,	 96,	 24);
					Ili9341_Ascii((int8_t*)"04)",			 24,	BLACK,	WHITE,	 12,	177,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[3],	 24,	BLACK,	WHITE,	 48,	177,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[3],	 24,	BLACK,	WHITE,	108,	177,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[3],	 24,	BLACK,	WHITE,	216,	177,	 96,	 24);
					break;
				case 9 :	// Log2
					Ili9341_Ascii((int8_t*)"Log (2/3)",		 36,	BLACK,	WHITE,	 36,	 65,	162,	 36);
					Ili9341_Ascii((int8_t*)"05)",			 24,	BLACK,	WHITE,	 12,	105,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[4],	 24,	BLACK,	WHITE,	 48,	105,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[4],	 24,	BLACK,	WHITE,	108,	105,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[4],	 24,	BLACK,	WHITE,	216,	105,	 96,	 24);
					Ili9341_Ascii((int8_t*)"06)",			 24,	BLACK,	WHITE,	 12,	129,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[5],	 24,	BLACK,	WHITE,	 48,	129,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[5],	 24,	BLACK,	WHITE,	108,	129,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[5],	 24,	BLACK,	WHITE,	216,	129,	 96,	 24);
					Ili9341_Ascii((int8_t*)"07)",			 24,	BLACK,	WHITE,	 12,	153,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[6],	 24,	BLACK,	WHITE,	 48,	153,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[6],	 24,	BLACK,	WHITE,	108,	153,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[6],	 24,	BLACK,	WHITE,	216,	153,	 96,	 24);
					Ili9341_Ascii((int8_t*)"08)",			 24,	BLACK,	WHITE,	 12,	177,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[7],	 24,	BLACK,	WHITE,	 48,	177,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[7],	 24,	BLACK,	WHITE,	108,	177,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[7],	 24,	BLACK,	WHITE,	216,	177,	 96,	 24);
					break;
				case 10 :	// Log3
					Ili9341_Ascii((int8_t*)"Log (3/3)",		 36,	BLACK,	WHITE,	 36,	 65,	162,	 36);
					Ili9341_Ascii((int8_t*)"09)",			 24,	BLACK,	WHITE,	 12,	105,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[8],	 24,	BLACK,	WHITE,	 48,	105,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[8],	 24,	BLACK,	WHITE,	108,	105,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[8],	 24,	BLACK,	WHITE,	216,	105,	 96,	 24);
					Ili9341_Ascii((int8_t*)"10)",			 24,	BLACK,	WHITE,	 12,	129,	 36,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogCodeAsc[9],	 24,	BLACK,	WHITE,	 48,	129,	 48,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogDateAsc[9],	 24,	BLACK,	WHITE,	108,	129,	 96,	 24);
					Ili9341_Ascii((int8_t*)g_arMainLogTimeAsc[9],	 24,	BLACK,	WHITE,	216,	129,	 96,	 24);
					break;
				case 11 :	// Expansion
					Ili9341_Ascii((int8_t*)"Expansion",		 36,	BLACK,	WHITE,	 36,	 65,	162,	 36);
					Ili9341_Ascii((int8_t*)"InOut     - ",	 24,	BLACK,	WHITE,	 12,	105,	144,	 24);
					Ili9341_Ascii((int8_t*)"Serial    - ",	 24,	BLACK,	WHITE,	 12,	129,	144,	 24);
					Ili9341_Ascii((int8_t*)"Network   - ",	 24,	BLACK,	WHITE,	 12,	153,	144,	 24);
//					Ili9341_Ascii((int8_t*)"Wireless  - ",	 24,	BLACK,	WHITE,	 12,	177,	144,	 24);
//					Ili9341_Ascii((int8_t*)"Temp,Humi - ",	 24,	BLACK,	WHITE,	 12,	177,	144,	 24);
					Ili9341_Ascii((int8_t*)"Reserved  - ",	 24,	BLACK,	WHITE,	 12,	177,	144,	 24);

					if(g_deMainModeIoNum == 0)		// if I/O Unused
					{
						Ili9341_Ascii((int8_t*)"None        ",	 24,	BLACK,	WHITE,	156,	105,	144,	 24);
					}
					else							// if I/O Used
					{
						Ili9341_FilledRect(Din.ExternalInput1Inv?WHITE:BLACK,	157,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"I1",	 12,	Din.ExternalInput1Inv?BLACK:WHITE,	Din.ExternalInput1Inv?WHITE:BLACK,	 158,	111,	 16,	 12);
						Ili9341_FilledRect(Din.ExternalInput2Inv?WHITE:BLACK,	174,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"I2",	 12,	Din.ExternalInput2Inv?BLACK:WHITE,	Din.ExternalInput2Inv?WHITE:BLACK,	 175,	111,	 16,	 12);
						Ili9341_FilledRect(Din.ExternalInput3Inv?WHITE:BLACK,	191,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"I3",	 12,	Din.ExternalInput3Inv?BLACK:WHITE,	Din.ExternalInput3Inv?WHITE:BLACK,	 192,	111,	 16,	 12);
						Ili9341_FilledRect(Din.ExternalInput4Inv?WHITE:BLACK,	208,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"I4",	 12,	Din.ExternalInput4Inv?BLACK:WHITE,	Din.ExternalInput4Inv?WHITE:BLACK,	 209,	111,	 16,	 12);
						Ili9341_FilledRect(Dout.ExternalOutput1?BLACK:WHITE,	232,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"O1",	 12,	Dout.ExternalOutput1?WHITE:BLACK,	Dout.ExternalOutput1?BLACK:WHITE,	 233,	111,	 16,	 12);
						Ili9341_FilledRect(Dout.ExternalOutput2?BLACK:WHITE,	249,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"O2",	 12,	Dout.ExternalOutput2?WHITE:BLACK,	Dout.ExternalOutput2?BLACK:WHITE,	 250,	111,	 16,	 12);
						Ili9341_FilledRect(Dout.ExternalOutput3?BLACK:WHITE,	266,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"O3",	 12,	Dout.ExternalOutput3?WHITE:BLACK,	Dout.ExternalOutput3?BLACK:WHITE,	 267,	111,	 16,	 12);
						Ili9341_FilledRect(Dout.ExternalOutput4?BLACK:WHITE,	283,	110,	 13,	 13);
						Ili9341_Ascii((int8_t*)"O4",	 12,	Dout.ExternalOutput4?WHITE:BLACK,	Dout.ExternalOutput4?BLACK:WHITE,	 284,	111,	 16,	 12);
					}

					if(g_deMainMode485Num == 0)		// if Rs485 Unused
					{
						Ili9341_Ascii((int8_t*)"None        ",	 24,	BLACK,	WHITE,	156,	129,	144,	 24);
					}
					else							// if Rs485 Used
					{
						if(BStatus.bit.Rs485Disconn == 0)
							Ili9341_Ascii((int8_t*)"Connected   ",	 24,	BLACK,	WHITE,	156,	129,	144,	 24);
						else
							Ili9341_Ascii((int8_t*)"Disconnected",	 24,	BLACK,	WHITE,	156,	129,	144,	 24);
					}

					if(g_deMainModeLanNum == 0)		// if Lan Unused
					{
						Ili9341_Ascii((int8_t*)"None        ",	 24,	BLACK,	WHITE,	156,	153,	144,	 24);
					}
					else							// if Lan Used
					{
						if(BStatus.bit.LanDisconn == 0)
							Ili9341_Ascii((int8_t*)"Connected   ",	 24,	BLACK,	WHITE,	156,	153,	144,	 24);
						else
							Ili9341_Ascii((int8_t*)"Disconnected",	 24,	BLACK,	WHITE,	156,	153,	144,	 24);
					}

//					if(g_deMainModeThNum == 0)		// if Th Sensor Unused
//					{
						Ili9341_Ascii((int8_t*)"None        ",	 24,	BLACK,	WHITE,	156,	177,	144,	 24);
//					}
//					else							// if Th Sensor Used
//					{
//						if(BResult.bit.I2C4ConnErr == 0)
//						{
//							Ili9341_Ascii((int8_t*)g_arMainTempAsc,	 24,	BLACK,	WHITE,	156,	177,	 48,	 24);
//							Ili9341_Ascii((int8_t*)"c,",			 24,	BLACK,	WHITE,	204,	177,	 24,	 24);
//							Ili9341_Point(BLACK, 3, 204, 182);
//							Ili9341_Ascii((int8_t*)g_arMainHumiAsc,	 24,	BLACK,	WHITE,	228,	177,	 48,	 24);
//							Ili9341_Ascii((int8_t*)"% ",			 24,	BLACK,	WHITE,	276,	177,	 24,	 24);
//						}
//						else
//							Ili9341_Ascii((int8_t*)"Disconnected",	 24,	BLACK,	WHITE,	156,	177,	144,	 24);
//					}
					break;
				case 12 :	// Version
					Ili9341_Ascii((int8_t*)"Version",		 36,	BLACK,	WHITE,	 36,	 65,	126,	 36);
					Ili9341_Ascii((int8_t*)"Module H/W - ",	 24,	BLACK,	WHITE,	 12,	105,	156,	 24);
					Ili9341_Ascii((int8_t*)"Module S/W - ",	 24,	BLACK,	WHITE,	 12,	129,	156,	 24);
					Ili9341_Ascii((int8_t*)"Sensor H/W - ",	 24,	BLACK,	WHITE,	 12,	153,	156,	 24);
					Ili9341_Ascii((int8_t*)"Sensor S/W - ",	 24,	BLACK,	WHITE,	 12,	177,	156,	 24);
					Ili9341_Ascii((int8_t*)"V5.0",			 24,	BLACK,	WHITE,	168,	105,	 48,	 24);
					Ili9341_Ascii((int8_t*)"V202005051",	 24,	BLACK,	WHITE,	168,	129,	120,	 24);
					Ili9341_Ascii((int8_t*)"V0.5",			 24,	BLACK,	WHITE,	168,	153,	 48,	 24);
					Ili9341_Ascii(PVersion.Version,			 24,	BLACK,	WHITE,	168,	177,	120,	 24);
					break;
				default :
					break;
			}

			// Lower Side Time & Date
			Ili9341_Ascii((int8_t*)&RDate,	24, BLACK, WHITE, 120, 216, 96, 24);
			Ili9341_Ascii((int8_t*)&RTime,	24, BLACK, WHITE, 222, 216, 96, 24);

		}
		else									// First time Only(Moved Point of Cursor)
		{
			// Page Order Status
			if(g_deMainSelStt < 4)
			{
				g_deMainBlockStt = 2;
				Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[g_deMainSelStt_k-1], 48, 16, 11);
				Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[g_deMainSelStt-1], 48, 16, 11);
			}
			else if(g_deMainSelStt > 10)
			{
				g_deMainBlockStt = 9;
				Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[g_deMainSelStt_k-8], 48, 16, 11);
				Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[g_deMainSelStt-8], 48, 16, 11);
			}
			else if(g_deMainSelStt > g_deMainSelStt_k)
			{
				g_deMainBlockStt = g_deMainSelStt-2;
				Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[2], 48, 16, 11);
				Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[3], 48, 16, 11);
			}
			else
			{
				g_deMainBlockStt = g_deMainSelStt-1;
				Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[3], 48, 16, 11);
				Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[2], 48, 16, 11);
			}

			// Upper Side Color Box & Text
			Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	  0,	  0,	 75,	 40);
			Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 80,	  0,	 75,	 40);
			Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	160,	  0,	 75,	 40);
			Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	240,	  0,	 75,	 40);

			Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+0]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	  8,	  0,	 60,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+1]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 88,	  0,	 60,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+2]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	168,	  0,	 60,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+3]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	248,	  0,	 60,	 24);

			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+0]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	 15,	 18,	 48,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+1]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 95,	 18,	 48,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+2]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	175,	 18,	 48,	 24);
			Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+3]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	255,	 18,	 48,	 24);

			// Center Color Box or Left Side Color Bar(Change to PM10. from Error Page Only)
			if((g_deMainSelStt == 6) && (g_deMainSelStt_k ==7))
			{
				Ili9341_FilledRect((g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	  0,	 65,	320,	150);	//

				Ili9341_FilledRect(RED, 	25,  65, 30, 30);
				Ili9341_FilledRect(ORANGE,	25,  95, 30, 30);
				Ili9341_FilledRect(YELLOW,	25, 125, 30, 30);
				Ili9341_FilledRect(GREEN,	25, 155, 30, 30);
				Ili9341_FilledRect(BLUE,	25, 185, 30, 30);
			}
			else if(g_deMainSelStt <= 6)
				Ili9341_FilledRect((g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 60,	 65,	255,	150);	//
			else
				Ili9341_FilledRect((g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	  0,	 65,	320,	150);	//

			// Frame of Input Output(Expansion Page at Using I/O Only)
			if((g_deMainSelStt == 11) && (g_deMainModeIoNum == 1))
			{
				Ili9341_HallowRect(BLACK,	156,	109,	 15,	 15);
				Ili9341_HallowRect(BLACK,	173,	109,	 15,	 15);
				Ili9341_HallowRect(BLACK,	190,	109,	 15,	 15);
				Ili9341_HallowRect(BLACK,	207,	109,	 15,	 15);
				Ili9341_HallowRect(BLACK,	231,	109,	 15,	 15);
				Ili9341_HallowRect(BLACK,	248,	109,	 15,	 15);
				Ili9341_HallowRect(BLACK,	265,	109,	 15,	 15);
				Ili9341_HallowRect(BLACK,	282,	109,	 15,	 15);
			}
			g_deMainSelStt_k = g_deMainSelStt;
		}
	}
	else								// First Time Only(Changed State of Display)
	{
		// Clear Display
		Ili9341_Background(WHITE);

		// Upper Side Box Frame
		for(uint16_t block = 0; block <= 240; block +=80)
		{
			for(uint8_t i = 0; i <= 4; i++)
			{
				Ili9341_LineHori(BLACK, 75, 1+i+block, 40+i);
				Ili9341_LineVert(BLACK, 39, 75+i+block, 1+i);
			}
		}

		// Left Side Color Box
		if(g_deMainSelStt < 7)
		{
			Ili9341_FilledRect(RED, 	25,  65, 30, 30);
			Ili9341_FilledRect(ORANGE,	25,  95, 30, 30);
			Ili9341_FilledRect(YELLOW,	25, 125, 30, 30);
			Ili9341_FilledRect(GREEN,	25, 155, 30, 30);
			Ili9341_FilledRect(BLUE,	25, 185, 30, 30);
		}

		// Page Order Status
		if(g_deMainSelStt < 4)
		{
			g_deMainBlockStt = 2;
			Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[g_deMainSelStt_k-1], 48, 16, 11);
			Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[g_deMainSelStt-1], 48, 16, 11);
		}
		else if(g_deMainSelStt > 9)
		{
			g_deMainBlockStt = 8;
			Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[g_deMainSelStt_k-7], 48, 16, 11);
			Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[g_deMainSelStt-7], 48, 16, 11);
		}
		else if(g_deMainSelStt > g_deMainSelStt_k)
		{
			g_deMainBlockStt = g_deMainSelStt-2;
			Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[2], 48, 16, 11);
			Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[3], 48, 16, 11);
		}
		else
		{
			g_deMainBlockStt = g_deMainSelStt-1;
			Ili9341_HallowItri(WHITE,  90, g_arMainCursorX[3], 48, 16, 11);
			Ili9341_HallowItri(BLACK,  90, g_arMainCursorX[2], 48, 16, 11);
		}

		// Upper Side Color Box
		Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	  0,	  0,	 75,	 40);
		Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 80,	  0,	 75,	 40);
		Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	160,	  0,	 75,	 40);
		Ili9341_FilledRect(g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	240,	  0,	 75,	 40);

		// Upper Side Text(Name)
		Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+0]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	  8,	  0,	 60,	 24);
		Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+1]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 88,	  0,	 60,	 24);
		Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+2]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	168,	  0,	 60,	 24);
		Ili9341_Ascii((int8_t*)(g_arMainUppTxtAsc[g_deMainBlockStt+3]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	248,	  0,	 60,	 24);

		// Upper Side Text(Value)
		Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+0]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+0]],	 15,	 18,	 48,	 24);
		Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+1]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+1]],	 95,	 18,	 48,	 24);
		Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+2]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+2]],	175,	 18,	 48,	 24);
		Ili9341_Ascii((int8_t*)(g_arMainUppDatAsc[g_deMainBlockStt+3]),	 24,	BLACK,	g_arBgColor[g_arMainColorStt[g_deMainBlockStt+3]],	255,	 18,	 48,	 24);

		// Center Color Box
		Ili9341_FilledRect((g_deMainModeAlarmNum == 1)?(g_arBgColor[g_arMainBgClrStt[g_deMainSelStt]]):(g_arBgColor[0]),	 60,	 65,	255,	150);	//
		g_arMainBgClrStt_k[g_deMainSelStt] = g_arMainBgClrStt[g_deMainSelStt];

		g_deBkgdStt_k = g_deBkgdStt;
	}
}

void LCD_SettingDisplay(void)	// 9 pcs
{
	// Background Setting Page
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		if(g_deSetSelStt == g_deSetSelStt_k)	// After Second Time
		{
			asm(" NOP");
		}
		else									// First time Only(Moved Point of Cursor)
		{
			if(g_deSetListMax > 5)
			{
				if(g_deSetSelStt < 3)
				{
					g_deSetPageStt = 1;
					Ili9341_Ascii((int8_t*)" ",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[g_deSetSelStt_k],	18, 36);
					Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[g_deSetSelStt],	18, 36);
				}
				else if(g_deSetSelStt > g_deSetListMax-2)
				{
					g_deSetPageStt = g_deSetListMax-4;
					Ili9341_Ascii((int8_t*)" ",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[g_deSetSelStt_k-(g_deSetListMax-5)],	18, 36);
					Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[g_deSetSelStt-(g_deSetListMax-5)],	18, 36);
				}
				else if(g_deSetSelStt > g_deSetSelStt_k)
				{
					if(g_deSetSelStt == g_deSetPageStt+2)
					{
						Ili9341_Ascii((int8_t*)" ",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[2],	18, 36);
						Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[3],	18, 36);
					}
					else
					{
						g_deSetPageStt = g_deSetSelStt-3;
						Ili9341_Ascii((int8_t*)" ",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[3],	18, 36);
						Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[4],	18, 36);
					}
				}
				else
				{
					if(g_deSetSelStt == g_deSetPageStt+2)
					{
						Ili9341_Ascii((int8_t*)" ",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[4],	18, 36);
						Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[3],	18, 36);
					}
					else
					{
						g_deSetPageStt = g_deSetSelStt-1;
						Ili9341_Ascii((int8_t*)" ",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[3],	18, 36);
						Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[2],	18, 36);
					}
				}

				Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+0],	36, BLACK, WHITE,	 56,	g_arSetCursorY[1],	 36,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+0][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[1],	162,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+1],	36, BLACK, WHITE,	 56,	g_arSetCursorY[2],	 36,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+1][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[2],	162,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+2],	36, BLACK, WHITE,	 56,	g_arSetCursorY[3],	 36,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+2][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[3],	162,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+3],	36, BLACK, WHITE,	 56,	g_arSetCursorY[4],	 36,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+3][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[4],	162,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+4],	36, BLACK, WHITE,	 56,	g_arSetCursorY[5],	 36,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+4][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[5],	162,	 36);
			}
			else
			{
				Ili9341_Ascii((int8_t*)" ",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[g_deSetSelStt_k],	18, 36);
				Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[g_deSetSelStt],	18, 36);

				Ili9341_Ascii((int8_t*)"1.",	36, BLACK, WHITE,	 56,	g_arSetCursorY[1],	 36,	 36);
				Ili9341_Ascii((int8_t*)"ALARM REF",	36, BLACK, WHITE,	 92,	g_arSetCursorY[1],	162,	 36);
				Ili9341_Ascii((int8_t*)"2.",	36, BLACK, WHITE,	 56,	g_arSetCursorY[2],	 36,	 36);
				Ili9341_Ascii((int8_t*)"DATE,TIME",	36, BLACK, WHITE,	 92,	g_arSetCursorY[2],	162,	 36);
				Ili9341_Ascii((int8_t*)"3.",	36, BLACK, WHITE,	 56,	g_arSetCursorY[3],	 36,	 36);
				Ili9341_Ascii((int8_t*)"MODE",	36, BLACK, WHITE,	 92,	g_arSetCursorY[3],	162,	 36);
				Ili9341_Ascii((int8_t*)"4.",	36, BLACK, WHITE,	 56,	g_arSetCursorY[4],	 36,	 36);
				Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+3][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[4],	162,	 36);
				if(g_deSetListMax == 5)
				{
					Ili9341_Ascii((int8_t*)"5.",	36, BLACK, WHITE,	 56,	g_arSetCursorY[5],	 36,	 36);
					Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+4][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[5],	162,	 36);
				}
			}

			g_deSetSelStt_k = g_deSetSelStt;
		}
	}
	else								// First Time Only
	{
		// Display Clear, Main to Setting - All Clear/Detail to Setting - Partial Clear
		if(g_deBkgdStt_k == 1)			// Background is Changed from Main Page
		{
			Ili9341_Background(WHITE);

			Ili9341_Ascii((int8_t*)"SETTINGS",		36, BLACK, WHITE, 20, 14, 500, 500);

			g_deSetSelStt = 1;
		}
		else							// Background is Changed from Detail Page
		{
			Ili9341_Ascii((int8_t*)"SETTINGS         ",	36, BLACK, WHITE, 20, 14, 500, 500);

			Ili9341_FilledRect(WHITE, 0, 50, 320, 190);

			g_deSetSelStt = g_deSetSelStt_k;
		}

		// Menu Order Status
		Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+0],	36, BLACK, WHITE,	 56,	g_arSetCursorY[1],	 36,	 36);
		Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+0][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[1],	162,	 36);
		Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+1],	36, BLACK, WHITE,	 56,	g_arSetCursorY[2],	 36,	 36);
		Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+1][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[2],	162,	 36);
		Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+2],	36, BLACK, WHITE,	 56,	g_arSetCursorY[3],	 36,	 36);
		Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+2][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[3],	162,	 36);
		Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+3],	36, BLACK, WHITE,	 56,	g_arSetCursorY[4],	 36,	 36);
		Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+3][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[4],	162,	 36);
		if(g_deSetListMax >= 5)	// if Except List is 4(All Modes are Turned Off)
		{
			Ili9341_Ascii((int8_t*)g_arSetListNumAsc[g_deSetPageStt+4],	36, BLACK, WHITE,	 56,	g_arSetCursorY[5],	 36,	 36);
			Ili9341_Ascii((int8_t*)g_arSetListTxtAsc[g_arSetOrdrStt[g_deSetPageStt+4][g_deSetListStt]],	36, BLACK, WHITE,	 92,	g_arSetCursorY[5],	162,	 36);
		}

		// Cursor Point Status
		switch(g_deSetSelStt-g_deSetPageStt)
		{
			case 0 :
				Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[1],	18, 36);
				break;
			case 1 :
				Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[2],	18, 36);
				break;
			case 2 :
				Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[3],	18, 36);
				break;
			case 3 :
				Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[4],	18, 36);
				break;
			case 4 :
				Ili9341_Ascii((int8_t*)">",	36,	BLACK,	WHITE,	38,	g_arSetCursorY[5],	18, 36);
				break;
			default :
				break;
		}

		g_deBkgdStt_k = g_deBkgdStt;
	}
}

void LCD_DetailsAlarmDisplay(void)
{
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		// Renewal Alarm Reference Data
		Ili9341_Ascii(g_arSetAl003Asc,	24, BLACK, WHITE, g_arCharX[13], g_arCharY[1], g_arCharSizeX[4], g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetAl005Asc,	24, BLACK, WHITE, g_arCharX[13], g_arCharY[2], g_arCharSizeX[4], g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetAl010Asc,	24, BLACK, WHITE, g_arCharX[13], g_arCharY[3], g_arCharSizeX[4], g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetAl025Asc,	24, BLACK, WHITE, g_arCharX[13], g_arCharY[4], g_arCharSizeX[4], g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetAl050Asc,	24, BLACK, WHITE, g_arCharX[13], g_arCharY[5], g_arCharSizeX[4], g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetAl100Asc,	24, BLACK, WHITE, g_arCharX[13], g_arCharY[6], g_arCharSizeX[4], g_arCharSizeY[1]);

		if(g_deChSelStt != g_deChSelStt_k)	// Moved Point of Character Cursor
		{
			if(g_deChSelStt_k != 0)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[g_deDetSelStt], 10, 2);
			}
			g_deChSelStt_k = g_deChSelStt;
		}
		else
		{
			if(g_deChSelStt == 0)
			{
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"*", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
				Ili9341_FilledRect(g_deChCursor, g_arCharCursorX[g_deChSelStt], g_arCharCursorY[g_deDetSelStt], 10, 2);
			}
		}

		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], g_arCharSizeX[1], g_arCharSizeY[1]);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			g_deDetSelStt_k = g_deDetSelStt;
		}
	}
	else								// First Time Only(Changed to Details)
	{
		Ili9341_Ascii((int8_t*)"ALARM REF ",		36, BLACK, WHITE, 20, 14, 500, 500);

		Ili9341_FilledRect(WHITE, 0, 50, 320, 190);
		Ili9341_HallowRect(BLACK, 1, 51, 318, 188);

		Ili9341_Ascii((int8_t*)"1)0.3 um :      Pc(s)/L",	24, BLACK, WHITE, g_arCharX[2], g_arDetCursorY[1], g_arCharSizeX[23], g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"2)0.5 um :      Pc(s)/L",	24, BLACK, WHITE, g_arCharX[2], g_arDetCursorY[2], g_arCharSizeX[23], g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"3)1.0 um :      Pc(s)/L",	24, BLACK, WHITE, g_arCharX[2], g_arDetCursorY[3], g_arCharSizeX[23], g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"4)2.5 um :      Pc(s)/L",	24, BLACK, WHITE, g_arCharX[2], g_arDetCursorY[4], g_arCharSizeX[23], g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"5)5.0 um :      Pc(s)/L",	24, BLACK, WHITE, g_arCharX[2], g_arDetCursorY[5], g_arCharSizeX[23], g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"6) 10 um :      Pc(s)/L",	24, BLACK, WHITE, g_arCharX[2], g_arDetCursorY[6], g_arCharSizeX[23], g_arCharSizeY[1]);

		Ili9341_Ascii((int8_t*)"[SAVE&QUIT]",	24, BLACK, WHITE, g_arCharX[2], g_arDetCursorY[0], g_arCharSizeX[11], g_arCharSizeY[1]);

		g_deDetSelStt = 1;
		g_deDetSelStt_k = 255;
		g_deChSelStt = 0;
		g_deChSelStt_k = 10;

		g_deBkgdStt_k = g_deBkgdStt;
	}
}
void LCD_DetailsDatetimeDisplay(void)
{
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		Ili9341_Ascii(g_arSetDateAsc,								24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[2],	g_arCharSizeX[8],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetTimeAsc,								24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[4],	g_arCharSizeX[8],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetUpTimeAsc,								24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[6],	g_arCharSizeX[6],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetLifeTimeAsc,							24, BLACK, WHITE,	g_arCharX[11],	g_arCharY[6],	g_arCharSizeX[6],	g_arCharSizeY[1]);

		if(g_deChSelStt != g_deChSelStt_k)	// Moved Point of Character Cursor
		{
			if(g_deChSelStt_k != 0)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[g_deDetSelStt+1], 10, 2);
			}
			g_deChSelStt_k = g_deChSelStt;
		}
		else
		{
			if(g_deChSelStt == 0)
			{
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"*", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
				Ili9341_FilledRect(g_deChCursor, g_arCharCursorX[g_deChSelStt], g_arCharCursorY[g_deDetSelStt+1], 10, 2);
			}
		}

		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], g_arCharSizeX[1], g_arCharSizeY[1]);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			g_deDetSelStt_k = g_deDetSelStt;
		}
	}
	else								// First Time Only(Changed to Details)
	{
		Ili9341_Ascii((int8_t*)"DATE,TIME ",		36, BLACK, WHITE, 20, 14, 500, 500);

		Ili9341_FilledRect(WHITE, 0, 50, 320, 190);
		Ili9341_HallowRect(BLACK, 1, 51, 318, 188);

		Ili9341_Ascii((int8_t*)"1)Date(YY-MM-DD)",					24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[1],	g_arCharSizeX[16],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"2)Time(HH:MM:SS)",					24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[16],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"3)UpTime",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[8],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"/", 								24, BLACK, WHITE,	g_arCharX[10],	g_arCharY[6],	g_arCharSizeX[1],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"Hours", 							24, BLACK, WHITE,	g_arCharX[18],	g_arCharY[6],	g_arCharSizeX[5],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"[SAVE&QUIT]",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[11],	g_arCharSizeY[1]);

		g_deDetSelStt = 1;
		g_deDetSelStt_k = 255;
		g_deChSelStt = 0;
		g_deChSelStt_k = 10;

		g_deBkgdStt_k = g_deBkgdStt;
	}
}
void LCD_DetailsModeDisplay(void)
{
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			if(PAGE_1_OF_2)
			{
				Ili9341_FilledRect(WHITE, 2, 52, 316, 186);

				Ili9341_Ascii((int8_t*)"1)Alarm    :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[1],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"2)Ext.I/O  :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"3)RS485    :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"4)Network  :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[4],	g_arCharSizeX[12],	g_arCharSizeY[1]);
//				Ili9341_Ascii((int8_t*)"5)Wireless :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				// TH Sensor is Reserved
//				Ili9341_Ascii((int8_t*)"5)Temp,Humi:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"5)Reserved :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"                 ( 1/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
			}
			if(PAGE_2_OF_2)
			{
				Ili9341_FilledRect(WHITE, 2, 52, 316, 186);

				Ili9341_Ascii((int8_t*)"6)SMASample:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[7],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"7)Defreeze :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[8],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"[SAVE&QUIT]      ( 2/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
			}
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], 12, 24);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
			g_deDetSelStt_k = g_deDetSelStt;
		}

		if((g_deDetSelStt >= 1) && (g_deDetSelStt <= 6))
		{
			Ili9341_Ascii(g_arSetModeAlarmAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[1],	g_arCharSizeX[8],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetModeIoAsc,								24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[2],	g_arCharSizeX[8],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetMode485Asc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[3],	g_arCharSizeX[8],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetModeLanAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[4],	g_arCharSizeX[8],	g_arCharSizeY[1]);
			// TH Sensor is Reserved
//			Ili9341_Ascii(g_arSetModeThAsc,								24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[5],	g_arCharSizeX[8],	g_arCharSizeY[1]);
			Ili9341_Ascii((int8_t*)"None    ",								24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[5],	g_arCharSizeX[8],	g_arCharSizeY[1]);
		}
		if(((g_deDetSelStt >= 7) && (g_deDetSelStt <= 12)) || (g_deDetSelStt == 0))
		{
			Ili9341_Ascii(g_arSetModeSmaAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[7],	g_arCharSizeX[2],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetModeDefAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[8],	g_arCharSizeX[8],	g_arCharSizeY[1]);
		}
		if(g_deChSelStt != g_deChSelStt_k)	// Moved Point of Character Cursor
		{
			if(g_deChSelStt_k != 0)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[g_deDetSelStt], (g_deDetSelStt == 7)?22:((g_deDetSelStt == 4)?94:70), 2);
			}
			g_deChSelStt_k = g_deChSelStt;
		}
		else
		{
			if(g_deChSelStt == 0)
			{
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"*", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
				Ili9341_FilledRect(g_deChCursor, g_arCharCursorX[g_deChSelStt], g_arCharCursorY[g_deDetSelStt], (g_deDetSelStt == 7)?22:((g_deDetSelStt == 4)?94:70), 2);
			}
		}

		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], g_arCharSizeX[1], g_arCharSizeY[1]);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			g_deDetSelStt_k = g_deDetSelStt;
		}
	}
	else								// First Time Only(Changed to Details)
	{
		Ili9341_Ascii((int8_t*)"MODE      ",		36, BLACK, WHITE, 20, 14, 500, 500);

		Ili9341_FilledRect(WHITE, 0, 50, 320, 190);
		Ili9341_HallowRect(BLACK, 1, 51, 318, 188);

		Ili9341_Ascii((int8_t*)"1)Alarm    :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[1],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"2)Ext.I/O  :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"3)RS485    :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"4)Network  :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[4],	g_arCharSizeX[12],	g_arCharSizeY[1]);
//		Ili9341_Ascii((int8_t*)"5)Wireless :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		// TH Sensor is Reserved
//		Ili9341_Ascii((int8_t*)"5)Temp,Humi:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"5)Reserved :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"                 ( 1/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);

		g_deDetSelStt = 1;
		g_deDetSelStt_k = 1;
		g_deChSelStt = 0;
		g_deChSelStt_k = 10;

		g_deBkgdStt_k = g_deBkgdStt;
	}
}
void LCD_DetailsSerialDisplay(void)
{
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			if(PAGE_1_OF_2)
			{
				Ili9341_FilledRect(WHITE, 2, 52, 316, 186);

				Ili9341_Ascii((int8_t*)"1)Baud Rate:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[1],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"2)Data Bits:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"3)Parity   :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"4)Stop Bits:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[4],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"5)Data Flow:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"                 ( 1/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
			}
			if(PAGE_2_OF_2)
			{
				Ili9341_FilledRect(WHITE, 2, 52, 316, 186);

				Ili9341_Ascii((int8_t*)"6)Module ID:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[7],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"7)Dest. ID :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[8],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"8)Protocol :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[9],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				if(g_deSetLanPtclNum == 1)
					Ili9341_Ascii((int8_t*)"9)Device   :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[10],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"[SAVE&RESET]     ( 2/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
			}
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], 12, 24);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
			g_deDetSelStt_k = g_deDetSelStt;
		}
		if((g_deDetSelStt >= 1) && (g_deDetSelStt <= 6))
		{
			Ili9341_Ascii(g_arSet485BaudAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[1],	g_arCharSizeX[6],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSet485DataAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[2],	g_arCharSizeX[1],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSet485PrtyAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[3],	g_arCharSizeX[4],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSet485StopAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[4],	g_arCharSizeX[1],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSet485FlowAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[5],	g_arCharSizeX[4],	g_arCharSizeY[1]);
		}
		if(((g_deDetSelStt >= 7) && (g_deDetSelStt <= 12)) || (g_deDetSelStt == 0))
		{
			Ili9341_Ascii(g_arSet485ModAsc,								24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[7],	g_arCharSizeX[2],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSet485DestAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[8],	g_arCharSizeX[2],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSet485PtclAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[9],	g_arCharSizeX[8],	g_arCharSizeY[1]);
			if(g_deSet485PtclNum == 1)
			{
				Ili9341_Ascii((int8_t*)"9)Device   :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[10],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii(g_arSet485DvcdAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[10],	g_arCharSizeX[1],	g_arCharSizeY[1]);
				Ili9341_Ascii(g_arSet485DvldAsc,							24, BLACK, WHITE,	g_arCharX[15],	g_arCharY[10],	g_arCharSizeX[5],	g_arCharSizeY[1]);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"            ",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[10],	g_arCharSizeX[12],	g_arCharSizeY[1]);
				Ili9341_Ascii((int8_t*)"      ",							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[10],	g_arCharSizeX[6],	g_arCharSizeY[1]);
			}
		}
		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], 12, 24);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
			g_deDetSelStt_k = g_deDetSelStt;
		}
		if(g_deChSelStt != g_deChSelStt_k)	// Moved Point of Character Cursor
		{
			if(g_deChSelStt_k != 0)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[g_deDetSelStt], (g_deDetSelStt == 9)?94:((g_deDetSelStt == 1)?70:(((g_deDetSelStt == 3) || (g_deDetSelStt == 5))?46:(((g_deDetSelStt == 7) || (g_deDetSelStt == 8))?22:10))), 2);
			}
			g_deChSelStt_k = g_deChSelStt;
		}
		else
		{
			if(g_deChSelStt == 0)
			{
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"*", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
				Ili9341_FilledRect(g_deChCursor, g_arCharCursorX[g_deChSelStt], g_arCharCursorY[g_deDetSelStt],  (g_deDetSelStt == 9)?94:((g_deDetSelStt == 1)?70:(((g_deDetSelStt == 3) || (g_deDetSelStt == 5))?46:(((g_deDetSelStt == 7) || (g_deDetSelStt == 8))?22:10))), 2);
			}
		}
	}
	else								// First Time Only(Changed to Details)
	{
		Ili9341_Ascii((int8_t*)"SERIAL    ",		36, BLACK, WHITE, 20, 14, 500, 500);

		Ili9341_FilledRect(WHITE, 0, 50, 320, 190);
		Ili9341_HallowRect(BLACK, 1, 51, 318, 188);

		Ili9341_Ascii((int8_t*)"1)Baud Rate:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[1],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"2)Data Bits:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"3)Parity   :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"4)Stop Bits:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[4],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"5)Data Flow:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"                 ( 1/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);

		g_deDetSelStt = 1;
		g_deDetSelStt_k = 1;
		g_deChSelStt = 0;
		g_deChSelStt_k = 0;

		g_deBkgdStt_k = g_deBkgdStt;
	}
}
void LCD_DetailsNetworkDisplay(void)
{
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		if((g_deMainModeLanNum & 2) == 2)	// Include Client
		{
			if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
			{
				if(PAGE_1_OF_2)
				{
					Ili9341_FilledRect(WHITE, 2, 52, 316, 186);

					Ili9341_Ascii((int8_t*)"Module Address",					24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[1],	g_arCharSizeX[14],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"1)IP  :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[7],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"2)SUB :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[7],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"3)GW  :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[4],	g_arCharSizeX[7],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"4)PORT:",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[7],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"5)MAC :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[6],	g_arCharSizeX[7],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"                 ( 1/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
				}
				if(PAGE_2_OF_2)
				{
					Ili9341_FilledRect(WHITE, 2, 52, 316, 186);

					Ili9341_Ascii((int8_t*)"Dest. Address",						24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[7],	g_arCharSizeX[13],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"6)IP  :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[8],	g_arCharSizeX[7],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"7)PORT:",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[9],	g_arCharSizeX[7],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"8)Protocol :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[10],	g_arCharSizeX[12],	g_arCharSizeY[1]);
					if(g_deSetLanPtclNum == 1)
						Ili9341_Ascii((int8_t*)"9)Device   :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[11],	g_arCharSizeX[12],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"[SAVE&RESET]     ( 2/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
				}
				Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], 12, 24);
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
				g_deDetSelStt_k = g_deDetSelStt;
			}

			if((g_deDetSelStt >= 1) && (g_deDetSelStt <= 6))
			{
				Ili9341_Ascii(g_arSetLanModIpAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[2],	g_arCharSizeX[15],	g_arCharSizeY[1]);
				Ili9341_Ascii(g_arSetLanModSubAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[3],	g_arCharSizeX[15],	g_arCharSizeY[1]);
				Ili9341_Ascii(g_arSetLanModGwAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[4],	g_arCharSizeX[15],	g_arCharSizeY[1]);
				Ili9341_Ascii(g_arSetLanModPortAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[5],	g_arCharSizeX[5],	g_arCharSizeY[1]);
				Ili9341_Ascii(g_arSetLanModMacAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[6],	g_arCharSizeX[17],	g_arCharSizeY[1]);
			}
			if(((g_deDetSelStt >= 7) && (g_deDetSelStt <= 12)) || (g_deDetSelStt == 0))
			{
				Ili9341_Ascii(g_arSetLanDestIpAsc,								24, BLACK, WHITE, g_arCharX[9], g_arCharY[8], g_arCharSizeX[15], 24);
				Ili9341_Ascii(g_arSetLanDestPortAsc,							24, BLACK, WHITE, g_arCharX[9], g_arCharY[9], g_arCharSizeX[5], 24);
				Ili9341_Ascii(g_arSetLanPtclAsc,								24, BLACK, WHITE, g_arCharX[14], g_arCharY[10], g_arCharSizeX[5], 24);
				if(g_deSetLanPtclNum == 1)
				{
					Ili9341_Ascii((int8_t*)"9)Device   :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[11],	g_arCharSizeX[12],	g_arCharSizeY[1]);
					Ili9341_Ascii(g_arSetLanDvcdAsc,							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[11],	g_arCharSizeX[1],	g_arCharSizeY[1]);
					Ili9341_Ascii(g_arSetLanDvldAsc,							24, BLACK, WHITE,	g_arCharX[15],	g_arCharY[11],	g_arCharSizeX[5],	g_arCharSizeY[1]);
				}
				else
				{
					Ili9341_Ascii((int8_t*)"            ",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[11],	g_arCharSizeX[12],	g_arCharSizeY[1]);
					Ili9341_Ascii((int8_t*)"      ",							24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[11],	g_arCharSizeX[6],	g_arCharSizeY[1]);
				}
			}
		}
		else								// Exclude Client
		{
			if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
			{
				Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], 12, 24);
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
				g_deDetSelStt_k = g_deDetSelStt;
			}
			Ili9341_Ascii(g_arSetLanModIpAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[2],	g_arCharSizeX[15],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetLanModSubAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[3],	g_arCharSizeX[15],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetLanModGwAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[4],	g_arCharSizeX[15],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetLanModPortAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[5],	g_arCharSizeX[5],	g_arCharSizeY[1]);
			Ili9341_Ascii(g_arSetLanModMacAsc,								24, BLACK, WHITE,	g_arCharX[9],	g_arCharY[6],	g_arCharSizeX[17],	g_arCharSizeY[1]);
		}
		if(g_deChSelStt != g_deChSelStt_k)	// Moved Point of Character Cursor
		{
			if(g_deChSelStt_k != 0)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[g_deDetSelStt], ((g_deDetSelStt == 10)?94:10), 2);
			}
			g_deChSelStt_k = g_deChSelStt;
		}
		else
		{
			if(g_deChSelStt == 0)
			{
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"*", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
				Ili9341_FilledRect(g_deChCursor, g_arCharCursorX[g_deChSelStt], g_arCharCursorY[g_deDetSelStt], ((g_deDetSelStt == 10)?94:10), 2);
			}
		}
	}
	else								// First Time Only(Changed to Details)
	{
		Ili9341_Ascii((int8_t*)"NETWORK   ",		36, BLACK, WHITE, 20, 14, 500, 500);

		Ili9341_FilledRect(WHITE, 0, 50, 320, 190);
		Ili9341_HallowRect(BLACK, 1, 51, 318, 188);

		Ili9341_Ascii((int8_t*)"Module Address",					24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[1],	g_arCharSizeX[14],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"1)IP  :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[7],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"2)SUB :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[7],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"3)GW  :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[4],	g_arCharSizeX[7],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"4)PORT:",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[5],	g_arCharSizeX[7],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"5)MAC :",							24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[6],	g_arCharSizeX[7],	g_arCharSizeY[1]);
		if((g_deMainModeLanNum & 2) == 2)
			Ili9341_Ascii((int8_t*)"                 ( 1/ 2)",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
		else
			Ili9341_Ascii((int8_t*)"[SAVE&RESET]            ",			24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[24],	g_arCharSizeY[1]);
		g_deDetSelStt = 2;
		g_deDetSelStt_k = 2;
		g_deChSelStt = 0;
		g_deChSelStt_k = 0;

		g_deBkgdStt_k = g_deBkgdStt;
	}
}
void LCD_DetailsWirelessDisplay(void)
{


}
void LCD_DetailsTemphumiDisplay(void)
{
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{

		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], 12, 24);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
			g_deDetSelStt_k = g_deDetSelStt;
		}
		Ili9341_Ascii(g_arSetThOtWrnAsc,							24, BLACK, WHITE,	g_arCharX[17],	g_arCharY[1],	g_arCharSizeX[2],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetThOtErrAsc,							24, BLACK, WHITE,	g_arCharX[17],	g_arCharY[2],	g_arCharSizeX[2],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetThOhErrAsc,							24, BLACK, WHITE,	g_arCharX[17],	g_arCharY[3],	g_arCharSizeX[2],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetThTempSlpAsc,							24, BLACK, WHITE,	g_arCharX[11],	g_arCharY[5],	g_arCharSizeX[4],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetThTempOffAsc,							24, BLACK, WHITE,	g_arCharX[18],	g_arCharY[5],	g_arCharSizeX[6],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetThHumiSlpAsc,							24, BLACK, WHITE,	g_arCharX[11],	g_arCharY[6],	g_arCharSizeX[4],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetThHumiOffAsc,							24, BLACK, WHITE,	g_arCharX[18],	g_arCharY[6],	g_arCharSizeX[6],	g_arCharSizeY[1]);

		if(g_deChSelStt != g_deChSelStt_k)	// Moved Point of Character Cursor
		{
			if(g_deChSelStt_k != 0)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[g_deDetSelStt], ((g_deDetSelStt <= 3)?22:10), 2);
			}
			if(g_deDetSelStt >= 4)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[5], 10, 2);
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[6], 10, 2);
			}
			g_deChSelStt_k = g_deChSelStt;
		}
		else
		{
			if(g_deChSelStt == 0)
			{
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"*", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], 12, 24);
				Ili9341_FilledRect(g_deChCursor, g_arCharCursorX[g_deChSelStt], g_arCharCursorY[g_deDetSelStt], ((g_deDetSelStt <= 3)?22:10), 2);
			}
		}
	}
	else								// First Time Only(Changed to Details)
	{
		Ili9341_Ascii((int8_t*)"TEMP,HUMI ",		36, BLACK, WHITE, 20, 14, 500, 500);

		Ili9341_FilledRect(WHITE, 0, 50, 320, 190);
		Ili9341_HallowRect(BLACK, 1, 51, 318, 188);

		Ili9341_Ascii((int8_t*)"1)OverTempWarn:  c",				24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[1],	g_arCharSizeX[18],	g_arCharSizeY[1]);
		Ili9341_Point(BLACK, 3, 228, 65);
		Ili9341_Ascii((int8_t*)"2)OverTempErr :  c",				24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[18],	g_arCharSizeY[1]);
		Ili9341_Point(BLACK, 3, 228, 89);
		Ili9341_Ascii((int8_t*)"3)OverHumiErr :  %",				24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[3],	g_arCharSizeX[18],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"4)Compensation",					24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[4],	g_arCharSizeX[14],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"Temp S:     O:",					24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[5],	g_arCharSizeX[14],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"Humi S:     O:",					24, BLACK, WHITE,	g_arCharX[4],	g_arCharY[6],	g_arCharSizeX[14],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"[SAVE&RESET]",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[12],	g_arCharSizeY[1]);

		g_deDetSelStt = 1;
		g_deDetSelStt_k = 1;
		g_deChSelStt = 0;
		g_deChSelStt_k = 0;

		g_deBkgdStt_k = g_deBkgdStt;
	}
}
void LCD_DetailsSoundDisplay(void)
{
	if(g_deBkgdStt == g_deBkgdStt_k)	// After Second Time
	{
		// Renewal Alarm Reference Data
		Ili9341_Ascii(g_arSetSndTypAsc,								24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[1],	g_arCharSizeX[6],	g_arCharSizeY[1]);
		Ili9341_Ascii(g_arSetSndVlmAsc,								24, BLACK, WHITE,	g_arCharX[14],	g_arCharY[2],	g_arCharSizeX[4],	g_arCharSizeY[1]);

		if(g_deChSelStt != g_deChSelStt_k)	// Moved Point of Character Cursor
		{
			if(g_deChSelStt_k != 0)
			{
				Ili9341_FilledRect(WHITE, g_arCharCursorX[g_deChSelStt_k], g_arCharCursorY[g_deDetSelStt], (g_deDetSelStt == 1)?70:46, 2);
			}
			g_deChSelStt_k = g_deChSelStt;
		}
		else
		{
			if(g_deChSelStt == 0)
			{
				Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			}
			else
			{
				Ili9341_Ascii((int8_t*)"*", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
				Ili9341_FilledRect(g_deChCursor, g_arCharCursorX[g_deChSelStt], g_arCharCursorY[g_deDetSelStt], (g_deDetSelStt == 1)?70:46, 2);
			}
		}

		if(g_deDetSelStt != g_deDetSelStt_k)	// Moved Point of Details Cursor
		{
			Ili9341_Ascii((int8_t*)" ", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt_k], g_arCharSizeX[1], g_arCharSizeY[1]);
			Ili9341_Ascii((int8_t*)">", 24, BLACK, WHITE, g_arDetCursorX[1], g_arDetCursorY[g_deDetSelStt], g_arCharSizeX[1], g_arCharSizeY[1]);
			g_deDetSelStt_k = g_deDetSelStt;
		}
	}
	else								// First Time Only(Changed to Details)
	{
		Ili9341_Ascii((int8_t*)"SOUND     ",		36, BLACK, WHITE, 20, 14, 500, 500);

		Ili9341_FilledRect(WHITE, 0, 50, 320, 190);
		Ili9341_HallowRect(BLACK, 1, 51, 318, 188);

		Ili9341_Ascii((int8_t*)"1)AlarmType:",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[1],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"2)Volume   :",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[2],	g_arCharSizeX[12],	g_arCharSizeY[1]);
		Ili9341_Ascii((int8_t*)"[SAVE&QUIT]",						24, BLACK, WHITE,	g_arCharX[2],	g_arCharY[0],	g_arCharSizeX[11],	g_arCharSizeY[1]);

		g_deDetSelStt = 1;
		g_deDetSelStt_k = 10;
		g_deChSelStt = 0;
		g_deChSelStt_k = 10;

		g_deBkgdStt_k = g_deBkgdStt;
	}
}

void LCD_MainStatus(void)
{
	// Inputs Main Page
	if(g_deSw1ShortFlg == 1)		// Change to Smaller Particle Selection
	{
		if(g_deMainSelStt == 2)
		{
			g_deMainSelStt = 2;
		}
		else
		{
			g_deMainSelStt--;
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Bigger Particle Selection
	{
		if(g_deMainSelStt == 12)
		{
			g_deMainSelStt = 12;
		}
		else
		{
			g_deMainSelStt++;
		}
		g_deSw2ShortFlg = 0;
	}

	if((g_deSw3LongFlg == 1) && (g_deSw4LongFlg == 0))			// Change to Setting Screen
	{
		g_deBkgdStt = SETTING_STATE;
		g_deSetSelStt = 1;
	}

	if(g_deSw4LongFlg == 1)			//
	{
		if(g_deSwCmdFlg == 1)
		{
			if(g_deSw3LongFlg == 1)	// Time-Log Clear(4, 1-2-1-1-2-1-2-3)
			{
				CPU_I2C.ClearFlag = 0;

				I2C_INIT();
				HAL_Delay(10);

				NVIC_SystemReset();
			}
		}
		if(g_deSwCmdFlg == 3)
		{

		}
	}

	// Color of Upper Box(0.5um, 1.0um, 2.5um, 5.0um, 10um)
	for(uint8_t i = 1; i <= 6; i++)
	{
		if(((uint32_t)*(&PM_UART.Cnt.Pm003+i-1)) >= g_arMainAlarmNum[i])
			g_arMainColorStt[i] = 1;
		else if(((uint32_t)*(&PM_UART.Cnt.Pm003+i-1)) >= ((g_arMainAlarmNum[i] >> 1)+(g_arMainAlarmNum[i] >> 2)))
			g_arMainColorStt[i] = 2;
		else if(((uint32_t)*(&PM_UART.Cnt.Pm003+i-1)) >= (g_arMainAlarmNum[i] >> 1))
			g_arMainColorStt[i] = 3;
		else if(((uint32_t)*(&PM_UART.Cnt.Pm003+i-1)) >= (g_arMainAlarmNum[i] >> 2))
			g_arMainColorStt[i] = 4;
		else
			g_arMainColorStt[i] = 5;
	}

	// Color of Upper Box(Error, Log-1, Log-2, Log-3)
	((BError.bit.FanAsErr == 1) || (BError.bit.PmConnErr == 1))?(g_arMainColorStt[7] = 1):(g_arMainColorStt[7] = 5);		// if Only no Err is O.K. else is Err
	(CPU_I2C.LogData[(CPU_I2C.LogCnt)&0x7f][1] != 1)?(g_arMainColorStt[8] = 4):(g_arMainColorStt[8] = 2);
	(CPU_I2C.LogData[(CPU_I2C.LogCnt-4)&0x7f][1] != 1)?(g_arMainColorStt[9] = 4):(g_arMainColorStt[9] = 2);
	(CPU_I2C.LogData[(CPU_I2C.LogCnt-8)&0x7f][1] != 1)?(g_arMainColorStt[10] = 4):(g_arMainColorStt[10] = 2);

	if(g_deMainSelStt <= 6)
	{
		// Color of Left Side Arrow
		(((uint32_t)*(&PM_UART.Cnt.Pm003+g_deMainSelStt-1)) >= g_arMainAlarmNum[g_deMainSelStt])
				?(g_arMainBgClrStt[g_deMainSelStt] = 1)
						:((uint32_t)*(&PM_UART.Cnt.Pm003+g_deMainSelStt-1) < (g_arMainAlarmNum[g_deMainSelStt]*0.95))?(g_arMainBgClrStt[g_deMainSelStt] = 0):0;
		if(((uint32_t)*(&PM_UART.Cnt.Pm003+g_deMainSelStt-1)) >= g_arMainAlarmNum[g_deMainSelStt])
			g_deMainLvClrStt = 1;
		else if(((uint32_t)*(&PM_UART.Cnt.Pm003+g_deMainSelStt-1)) >= ((g_arMainAlarmNum[g_deMainSelStt] >> 1)+(g_arMainAlarmNum[g_deMainSelStt] >> 2)))
			g_deMainLvClrStt = 2;
		else if(((uint32_t)*(&PM_UART.Cnt.Pm003+g_deMainSelStt-1)) >= (g_arMainAlarmNum[g_deMainSelStt] >> 1))
			g_deMainLvClrStt = 3;
		else if(((uint32_t)*(&PM_UART.Cnt.Pm003+g_deMainSelStt-1)) >= (g_arMainAlarmNum[g_deMainSelStt] >> 2))
			g_deMainLvClrStt = 4;
		else
			g_deMainLvClrStt = 5;
	}
	else
	{
		// Renewal Log Data(10 Lists, Date/Time/Code)
		for(uint8_t i = 0; i < 10; i++)
		{
			if(g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][1] == 1)
			{
				g_arMainLogDateAsc[i][0] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][2]/10)+'0';
				g_arMainLogDateAsc[i][1] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][2]%10)+'0';
				g_arMainLogDateAsc[i][2] = '-';
				g_arMainLogDateAsc[i][3] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][3]/10)+'0';
				g_arMainLogDateAsc[i][4] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][3]%10)+'0';
				g_arMainLogDateAsc[i][5] = '-';
				g_arMainLogDateAsc[i][6] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][4]/10)+'0';
				g_arMainLogDateAsc[i][7] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][4]%10)+'0';

				g_arMainLogTimeAsc[i][0] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][5]/10)+'0';
				g_arMainLogTimeAsc[i][1] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][5]%10)+'0';
				g_arMainLogTimeAsc[i][2] = ':';
				g_arMainLogTimeAsc[i][3] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][6]/10)+'0';
				g_arMainLogTimeAsc[i][4] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][6]%10)+'0';
				g_arMainLogTimeAsc[i][5] = ':';
				g_arMainLogTimeAsc[i][6] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][7]/10)+'0';
				g_arMainLogTimeAsc[i][7] = (g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][7]%10)+'0';

				switch(g_arMainLogDataNum[(g_deMainLogCntNum-i) & 0x7f][0])
				{
					// Check Event for Alarm(Odd : Start, Even : Clear)
					case 0x4B :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"S100", 4);
						break;
					case 0x4A :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"E100", 4);
						break;
					case 0x49 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"S050", 4);
						break;
					case 0x48 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"E050", 4);
						break;
					case 0x47 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"S025", 4);
						break;
					case 0x46 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"E025", 4);
						break;
					case 0x45 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"S010", 4);
						break;
					case 0x44 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"E010", 4);
						break;
					case 0x43 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"S005", 4);
						break;
					case 0x42 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"E005", 4);
						break;
					// Check Event for Error(Odd : Start, Even : Clear)
					case 47 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Sohm", 4);
						break;
					case 46 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Eohm", 4);
						break;
					case 45 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Sotm", 4);
						break;
					case 44 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Eotm", 4);
						break;
//					case 13 :
//						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Swtm", 4);
//						break;
//					case 12 :
//						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Ewtm", 4);
//						break;
					case 0x61 :
						((PM_UART.Alarm & 1) == 1) ? (strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Sfos", 4)) : (strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Sfus", 4));
						break;
					case 0x60 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Efas", 4);
						break;
//					case 41 :
//						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Sthc", 4);
//						break;
//					case 40 :
//						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Ethc", 4);
//						break;
					case 0x65 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Spmc", 4);
						break;
					case 0x64 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Epmc", 4);
						break;
//					case 37 :
//						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Swlc", 4);
//						break;
//					case 36 :
//						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Ewlc", 4);
//						break;
					case 0x67 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Ssdc", 4);
						break;
					case 0x66 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Esdc", 4);
						break;
					case 0x63 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Serc", 4);
						break;
					case 0x62 :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Eerc", 4);
						break;
					// Undefined(Wrong Code)
					default :
						strncpy((char*)g_arMainLogCodeAsc[i], (char*)"Undf", 4);
						break;
				}
			}
			else
			{
				strncpy((char*)g_arMainLogCodeAsc[i], (char*)"    ", 4);
				memset((void*)g_arMainLogDateAsc[i], ' ', 8);
				memset((void*)g_arMainLogTimeAsc[i], ' ', 8);
			}
		}

		// Renewal Temperature, Humidity
		ConvertNum2AscDec((int16_t)(TH_I2C.Temperature*10), g_arMainTempAsc, 3);
		ConvertNum2AscDec((int16_t)(TH_I2C.Humidity*10), g_arMainHumiAsc, 3);
		g_arMainTempAsc[3] = g_arMainTempAsc[2];
		g_arMainTempAsc[2] = '.';
		g_arMainHumiAsc[3] = g_arMainHumiAsc[2];
		g_arMainHumiAsc[2] = '.';
	}
}

void LCD_SettingStatus(void)
{
	// Inputs Setting Page
	if(g_deSw1ShortFlg == 1)		// Change to Lower Setting List Selection
	{
		if(g_deSetSelStt == 1)
			g_deSetSelStt = 1;
		else
			g_deSetSelStt--;
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Setting List Selection
	{
		if(g_deSetSelStt == g_deSetListMax)
			g_deSetSelStt = g_deSetListMax;
		else
			g_deSetSelStt++;
		g_deSw2ShortFlg = 0;
	}

	if((g_deSw3ShortFlg == 1) && (g_deSw3LongFlg == 0) && (g_deSw4ShortFlg == 0))		// Change to Set Details Screen
	{
		switch(g_arSetOrdrStt[g_deSetSelStt][g_deSetListStt])
		{
			case 1 :	// Load Alarm Reference Data
				g_deSetAl003Num = g_arMainAlarmNum[1];
				g_deSetAl005Num = g_arMainAlarmNum[2];
				g_deSetAl010Num = g_arMainAlarmNum[3];
				g_deSetAl025Num = g_arMainAlarmNum[4];
				g_deSetAl050Num = g_arMainAlarmNum[5];
				g_deSetAl100Num = g_arMainAlarmNum[6];

				g_deSetAl003Num_k = g_deSetAl003Num;
				g_deSetAl005Num_k = g_deSetAl005Num;
				g_deSetAl010Num_k = g_deSetAl010Num;
				g_deSetAl025Num_k = g_deSetAl025Num;
				g_deSetAl050Num_k = g_deSetAl050Num;
				g_deSetAl100Num_k = g_deSetAl100Num;

				ConvertNum2AscDec(g_deSetAl003Num, g_arSetAl003Asc, 4);
				ConvertNum2AscDec(g_deSetAl005Num, g_arSetAl005Asc, 4);
				ConvertNum2AscDec(g_deSetAl010Num, g_arSetAl010Asc, 4);
				ConvertNum2AscDec(g_deSetAl025Num, g_arSetAl025Asc, 4);
				ConvertNum2AscDec(g_deSetAl050Num, g_arSetAl050Asc, 4);
				ConvertNum2AscDec(g_deSetAl100Num, g_arSetAl100Asc, 4);
				break;
			case 2 :	// Load Date,Time Data(Date, Time, UpTime)
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				g_arSetDateNum[0] = sDate.Year;
				g_arSetDateNum[1] = sDate.Month;
				g_arSetDateNum[2] = sDate.Date;
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				g_arSetTimeNum[0] = sTime.Hours;
				g_arSetTimeNum[1] = sTime.Minutes;
				g_arSetTimeNum[2] = sTime.Seconds;
				g_deSetUptimeNum = ((uint32_t)g_arRtcUptime[0]*24)+(uint32_t)g_arRtcUptime[1];
				g_deSetLifeNum = g_deMainLifetimeNum;

				memcpy(g_arSetDateNum_k, g_arSetDateNum, 3);
				memcpy(g_arSetTimeNum_k, g_arSetTimeNum, 3);
				g_deSetLifeNum_k = g_deSetLifeNum;

				g_arSetDateAsc[0] = (g_arSetDateNum[0]/10)%10+'0';
				g_arSetDateAsc[1] = g_arSetDateNum[0]%10+'0';
				ConvertNum2AscDec(g_arSetDateNum[1], g_arSetDateAsc+3, 2);
				ConvertNum2AscDec(g_arSetDateNum[2], g_arSetDateAsc+6, 2);
				ConvertNum2AscDec(g_arSetTimeNum[0], g_arSetTimeAsc, 2);
				ConvertNum2AscDec(g_arSetTimeNum[1], g_arSetTimeAsc+3, 2);
				ConvertNum2AscDec(g_arSetTimeNum[2], g_arSetTimeAsc+6, 2);
				ConvertNum2AscDec(g_deSetUptimeNum, g_arSetUpTimeAsc, 6);
				ConvertNum2AscDec(g_deSetLifeNum, g_arSetLifeTimeAsc, 6);
				break;
			case 3 :	// Load Mode Data(Alarm On/off, Network, Simple Moving Average Sample)
				g_deSetModeAlarmNum = g_deMainModeAlarmNum;		// 0-Unused, 1-Used
				g_deSetModeIoNum = g_deMainModeIoNum;			// 0-Unused, 1-Used
				g_deSetMode485Num = g_deMainMode485Num;			// 0-Unused, 1-Used
				g_deSetModeLanNum = g_deMainModeLanNum;			// 0-Unused, 1-Server, 2-Client, 3-Dual
				g_deSetModeWlssNum = g_deMainModeWlssNum;		// 0-Unused, 1-Wifi, 2-Ble, 3-Dual(server only)
				g_deSetModeThNum = g_deMainModeThNum;			// 0-Unused, 1-Used
				g_deSetModeSmaNum = g_deMainModeSmaNum;			// 1~63
				g_deSetModeDefNum = g_deMainModeDefNum;			// 0-Unused, 1-Used

				g_deSetModeAlarmNum_k = g_deSetModeAlarmNum;
				g_deSetModeIoNum_k = g_deSetModeIoNum;
				g_deSetMode485Num_k = g_deSetMode485Num;
				g_deSetModeLanNum_k = g_deSetModeLanNum;
				g_deSetModeWlssNum_k = g_deSetModeWlssNum;
				g_deSetModeThNum_k = g_deSetModeThNum;
				g_deSetModeDefNum_k = g_deSetModeDefNum;
				g_deSetModeSmaNum_k = g_deSetModeSmaNum;

				(g_deSetModeAlarmNum == 1) ? (strncpy((char*)g_arSetModeAlarmAsc, (char*)"Used    ", 8)) : (strncpy((char*)g_arSetModeAlarmAsc, (char*)"Unused  ", 8));
				(g_deSetModeIoNum == 1) ? (strncpy((char*)g_arSetModeIoAsc, (char*)"Used    ", 8)) : (strncpy((char*)g_arSetModeIoAsc, (char*)"Unused  ", 8));
				(g_deSetMode485Num == 1) ? (strncpy((char*)g_arSetMode485Asc, (char*)"Used    ", 8)) : (strncpy((char*)g_arSetMode485Asc, (char*)"Unused  ", 8));
				switch(g_deSetModeLanNum)
				{
					case 0 :
						strncpy((char*)g_arSetModeLanAsc, (char*)"Unused  ", 8);
						break;
					case 1 :
						strncpy((char*)g_arSetModeLanAsc, (char*)"Server  ", 8);
						break;
					case 2 :
						strncpy((char*)g_arSetModeLanAsc, (char*)"Client  ", 8);
						break;
					case 3 :
						strncpy((char*)g_arSetModeLanAsc, (char*)"Clnt&Srv", 8);
						break;
				}
				switch(g_deSetModeWlssNum)
				{
					case 0 :
						strncpy((char*)g_arSetModeWlssAsc, (char*)"Unused  ", 8);
						break;
					case 1 :
						strncpy((char*)g_arSetModeWlssAsc, (char*)"WIFI    ", 8);
						break;
					case 2 :
						strncpy((char*)g_arSetModeWlssAsc, (char*)"BLE     ", 8);
						break;
					case 3 :
						strncpy((char*)g_arSetModeWlssAsc, (char*)"WIFI&BLE", 8);
						break;
				}
				(g_deSetModeThNum == 1) ? (strncpy((char*)g_arSetModeThAsc, (char*)"Used    ", 8)) : (strncpy((char*)g_arSetModeThAsc, (char*)"Unused  ", 8));
				ConvertNum2AscDec(g_deSetModeSmaNum, g_arSetModeSmaAsc, 2);
				while(g_arSetModeSmaAsc[0] == ' ')
				{
					g_arSetModeSmaAsc[0] = g_arSetModeSmaAsc[1];
					g_arSetModeSmaAsc[1] = ' ';
				}
				(g_deSetModeDefNum == 1) ? (strncpy((char*)g_arSetModeDefAsc, (char*)"Used    ", 8)) : (strncpy((char*)g_arSetModeDefAsc, (char*)"Unused  ", 8));
			case 4 :	// Load Serial Data
				g_deSet485BaudNum = g_deMain485BaudNum;
				g_deSet485DataNum = g_deMain485DataNum;
				g_deSet485PrtyNum = g_deMain485PrtyNum;
				g_deSet485StopNum = g_deMain485StopNum;
				g_deSet485FlowNum = g_deMain485FlowNum;
				g_deSet485ModNum = g_deMain485ModNum;
				g_deSet485DestNum = g_deMain485DestNum;
				g_deSet485PtclNum = g_deMain485PtclNum;
				g_deSet485DvcdNum = g_deMain485DvcdNum;
				g_deSet485DvldNum = g_deMain485DvldNum;

				g_deSet485BaudNum_k = g_deSet485BaudNum;
				g_deSet485DataNum_k = g_deSet485DataNum;
				g_deSet485PrtyNum_k = g_deSet485PrtyNum;
				g_deSet485StopNum_k = g_deSet485StopNum;
				g_deSet485FlowNum_k = g_deSet485FlowNum;
				g_deSet485ModNum_k = g_deSet485ModNum;
				g_deSet485DestNum_k = g_deSet485DestNum;
				g_deSet485PtclNum_k = g_deSet485PtclNum;
				g_deSet485DvcdNum_k = g_deSet485DvcdNum;
				g_deSet485DvldNum_k = g_deSet485DvldNum;

				ConvertNum2AscDec(g_deSet485BaudNum, g_arSet485BaudAsc, 6);
				while(g_arSet485BaudAsc[0] == ' ')
				{
					g_arSet485BaudAsc[0] = g_arSet485BaudAsc[1];
					g_arSet485BaudAsc[1] = g_arSet485BaudAsc[2];
					g_arSet485BaudAsc[2] = g_arSet485BaudAsc[3];
					g_arSet485BaudAsc[3] = g_arSet485BaudAsc[4];
					g_arSet485BaudAsc[4] = g_arSet485BaudAsc[5];
					g_arSet485BaudAsc[5] = ' ';
				}
				g_arSet485DataAsc[0] = g_deSet485DataNum+'0';
				switch(g_deSet485PrtyNum)
				{
					case 0:
						strncpy((char*)g_arSet485PrtyAsc, (char*)"None", 4);
						break;
					case 1 :
						strncpy((char*)g_arSet485PrtyAsc, (char*)"Odd ", 4);
						break;
					case 2 :
						strncpy((char*)g_arSet485PrtyAsc, (char*)"Even", 4);
						break;
				}
				g_arSet485StopAsc[0] = g_deSet485StopNum+'0';
				switch(g_deSet485FlowNum)
				{
					case 0 :
						strncpy((char*)g_arSet485FlowAsc, (char*)"None", 4);
						break;
					case 1 :
						strncpy((char*)g_arSet485FlowAsc, (char*)"Soft", 4);
						break;
				}
				ConvertNum2AscDec(g_deSet485ModNum, g_arSet485ModAsc, 2);
				while(g_arSet485ModAsc[0] == ' ')
				{
					g_arSet485ModAsc[0] = g_arSet485ModAsc[1];
					g_arSet485ModAsc[1] = ' ';
				}
				ConvertNum2AscDec(g_deSet485DestNum, g_arSet485DestAsc, 2);
				while(g_arSet485DestAsc[0] == ' ')
				{
					g_arSet485DestAsc[0] = g_arSet485DestAsc[1];
					g_arSet485DestAsc[1] = ' ';
				}
				switch(g_deSet485PtclNum)
				{
					case 0 :
						strncpy((char*)g_arSet485PtclAsc, (char*)"ACT     ", 8);
						break;
					case 1 :
						strncpy((char*)g_arSet485PtclAsc, (char*)"MC Fmt5 ", 8);
						break;
				}
				switch(g_deSet485DvcdNum)
				{
					case 0xa0 :
						strncpy((char*)g_arSet485DvcdAsc, (char*)"B", 1);
						break;
					case 0xa8 :
						strncpy((char*)g_arSet485DvcdAsc, (char*)"D", 1);
						break;
					case 0xaf :
						strncpy((char*)g_arSet485DvcdAsc, (char*)"R", 1);
						break;
					case 0xb0 :
						strncpy((char*)g_arSet485DvcdAsc, (char*)"W", 1);
						break;
					default :
						break;
				}
				g_arSet485DvldAsc[0] = (g_deSet485DvldNum/10000)%10+'0';
				g_arSet485DvldAsc[1] = (g_deSet485DvldNum/1000)%10+'0';
				g_arSet485DvldAsc[2] = (g_deSet485DvldNum/100)%10+'0';
				g_arSet485DvldAsc[3] = (g_deSet485DvldNum/10)%10+'0';
				g_arSet485DvldAsc[4] = g_deSet485DvldNum%10+'0';
				break;
			case 5 :	// Load Network Data(UserIp, UserSubnetMask, UserGateway, UserPort, UserMac, ServerIp, ServerPort)
				memcpy(g_arSetLanModIpNum, g_arMainLanModIpNum, 4);
				memcpy(g_arSetLanModSubNum, g_arMainLanModSubNum, 4);
				memcpy(g_arSetLanModGwNum, g_arMainLanModGwNum, 4);
				memcpy(g_arSetLanModMacNum, g_arMainLanModMacNum, 6);
				g_deSetLanModPortNum = g_deMainLanModPortNum;
				memcpy(g_arSetLanDestIpNum, g_arMainLanDestIpNum, 4);
				g_deSetLanDestPortNum = g_deMainLanDestPortNum;
				g_deSetLanPtclNum = g_deMainLanPtclNum;
				g_deSetLanDvcdNum = g_deMainLanDvcdNum;
				g_deSetLanDvldNum = g_deMainLanDvldNum;

				memcpy(g_arSetLanModIpNum_k, g_arSetLanModIpNum, 4);
				memcpy(g_arSetLanModSubNum_k, g_arSetLanModSubNum, 4);
				memcpy(g_arSetLanModGwNum_k, g_arSetLanModGwNum, 4);
				memcpy(g_arSetLanModMacNum_k, g_arSetLanModMacNum, 6);
				g_deSetLanModPortNum_k = g_deSetLanModPortNum;
				memcpy(g_arSetLanDestIpNum_k, &g_arSetLanDestIpNum, 4);
				g_deSetLanDestPortNum_k = g_deSetLanDestPortNum;
				g_deSetLanPtclNum_k = g_deSetLanPtclNum;
				g_deSetLanDvcdNum_k = g_deSetLanDvcdNum;
				g_deSetLanDvldNum_k = g_deSetLanDvldNum;

				ConvertNum2AscDec(g_arSetLanModIpNum[0], g_arSetLanModIpAsc, 3);
				ConvertNum2AscDec(g_arSetLanModIpNum[1], g_arSetLanModIpAsc+4, 3);
				ConvertNum2AscDec(g_arSetLanModIpNum[2], g_arSetLanModIpAsc+8, 3);
				ConvertNum2AscDec(g_arSetLanModIpNum[3], g_arSetLanModIpAsc+12, 3);
				ConvertNum2AscDec(g_arSetLanModSubNum[0], g_arSetLanModSubAsc, 3);
				ConvertNum2AscDec(g_arSetLanModSubNum[1], g_arSetLanModSubAsc+4, 3);
				ConvertNum2AscDec(g_arSetLanModSubNum[2], g_arSetLanModSubAsc+8, 3);
				ConvertNum2AscDec(g_arSetLanModSubNum[3], g_arSetLanModSubAsc+12, 3);
				ConvertNum2AscDec(g_arSetLanModGwNum[0], g_arSetLanModGwAsc, 3);
				ConvertNum2AscDec(g_arSetLanModGwNum[1], g_arSetLanModGwAsc+4, 3);
				ConvertNum2AscDec(g_arSetLanModGwNum[2], g_arSetLanModGwAsc+8, 3);
				ConvertNum2AscDec(g_arSetLanModGwNum[3], g_arSetLanModGwAsc+12, 3);
				ConvertNum2AscHex(g_arSetLanModMacNum[0], g_arSetLanModMacAsc, 2);
				ConvertNum2AscHex(g_arSetLanModMacNum[1], g_arSetLanModMacAsc+3, 2);
				ConvertNum2AscHex(g_arSetLanModMacNum[2], g_arSetLanModMacAsc+6, 2);
				ConvertNum2AscHex(g_arSetLanModMacNum[3], g_arSetLanModMacAsc+9, 2);
				ConvertNum2AscHex(g_arSetLanModMacNum[4], g_arSetLanModMacAsc+12, 2);
				ConvertNum2AscHex(g_arSetLanModMacNum[5], g_arSetLanModMacAsc+15, 2);
				ConvertNum2AscDec(g_deSetLanModPortNum, g_arSetLanModPortAsc, 5);
				ConvertNum2AscDec(g_arSetLanDestIpNum[0], g_arSetLanDestIpAsc, 3);
				ConvertNum2AscDec(g_arSetLanDestIpNum[1], g_arSetLanDestIpAsc+4, 3);
				ConvertNum2AscDec(g_arSetLanDestIpNum[2], g_arSetLanDestIpAsc+8, 3);
				ConvertNum2AscDec(g_arSetLanDestIpNum[3], g_arSetLanDestIpAsc+12, 3);
				ConvertNum2AscDec(g_deSetLanDestPortNum, g_arSetLanDestPortAsc, 5);
				switch(g_deSetLanPtclNum)
				{
					case 0 :
						strncpy((char*)g_arSetLanPtclAsc, (char*)"ACT     ", 8);
						break;
					case 1 :
						strncpy((char*)g_arSetLanPtclAsc, (char*)"MC      ", 8);
						break;
				}
				switch(g_deSetLanDvcdNum)
				{
					case 0xa8 :
						strncpy((char*)g_arSetLanDvcdAsc, (char*)"D", 1);
						break;
					case 0xb4 :
						strncpy((char*)g_arSetLanDvcdAsc, (char*)"W", 1);
						break;
					default :
						break;
				}
				g_arSetLanDvldAsc[0] = (g_deSetLanDvldNum/10000)%10+'0';
				g_arSetLanDvldAsc[1] = (g_deSetLanDvldNum/1000)%10+'0';
				g_arSetLanDvldAsc[2] = (g_deSetLanDvldNum/100)%10+'0';
				g_arSetLanDvldAsc[3] = (g_deSetLanDvldNum/10)%10+'0';
				g_arSetLanDvldAsc[4] = g_deSetLanDvldNum%10+'0';
				break;
			case 6 :	// Load Wireless Data
				asm(" NOP");
				break;
			case 7 :	// Load Temperature, Humidity Data
				g_deSetThOtWrnNum = g_deMainThOtWrnNum;
				g_deSetThOtErrNum = g_deMainThOtErrNum;
				g_deSetThOhErrNum = g_deMainThOhErrNum;
				g_deSetThTempSlpNum = g_deMainThTempSlpNum;
				g_deSetThTempOffNum = g_deMainThTempOffNum;
				g_deSetThHumiSlpNum = g_deMainThHumiSlpNum;
				g_deSetThHumiOffNum = g_deMainThHumiOffNum;

				g_deSetThOtWrnNum_k = g_deSetThOtWrnNum;
				g_deSetThOtErrNum_k = g_deSetThOtErrNum;
				g_deSetThOhErrNum_k = g_deSetThOhErrNum;
				g_deSetThTempSlpNum_k = g_deSetThTempSlpNum;
				g_deSetThTempOffNum_k = g_deSetThTempOffNum;
				g_deSetThHumiSlpNum_k = g_deSetThHumiSlpNum;
				g_deSetThHumiOffNum_k = g_deSetThHumiOffNum;

				ConvertNum2AscDec(g_deSetThOtWrnNum, g_arSetThOtWrnAsc, 2);
				ConvertNum2AscDec(g_deSetThOtErrNum, g_arSetThOtErrAsc, 2);
				ConvertNum2AscDec(g_deSetThOhErrNum, g_arSetThOhErrAsc, 2);
				g_arSetThTempSlpAsc[0] = (g_deSetThTempSlpNum/100)%10+'0';
				g_arSetThTempSlpAsc[2] = (g_deSetThTempSlpNum/10)%10+'0';
				g_arSetThTempSlpAsc[3] = g_deSetThTempSlpNum%10+'0';
				g_arSetThTempOffAsc[0] = (g_deSetThTempOffNum < 0)?'-':'+';
				g_arSetThTempOffAsc[1] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/1000)%10+'0';
				g_arSetThTempOffAsc[2] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/100)%10+'0';
				g_arSetThTempOffAsc[4] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/10)%10+'0';
				g_arSetThTempOffAsc[5] = ((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))%10+'0';

				g_arSetThHumiSlpAsc[0] = (g_deSetThHumiSlpNum/100)%10+'0';
				g_arSetThHumiSlpAsc[2] = (g_deSetThHumiSlpNum/10)%10+'0';
				g_arSetThHumiSlpAsc[3] = g_deSetThHumiSlpNum%10+'0';
				g_arSetThHumiOffAsc[0] = (g_deSetThHumiOffNum < 0)?'-':'+';
				g_arSetThHumiOffAsc[1] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/1000)%10+'0';
				g_arSetThHumiOffAsc[2] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/100)%10+'0';
				g_arSetThHumiOffAsc[4] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/10)%10+'0';
				g_arSetThHumiOffAsc[5] = ((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))%10+'0';
				break;
			case 8 :	// Load Sound Data
				g_deSetSndTypNum = g_deMainSndTypNum;
				g_deSetSndVlmNum = g_deMainSndVlmNum;

				g_deSetSndTypNum_k = g_deSetSndTypNum;
				g_deSetSndVlmNum_k = g_deSetSndVlmNum;

				switch(g_deSetSndTypNum)
				{
					case 0:
						strncpy((char*)g_arSetSndTypAsc, (char*)"Beep  ", 6);
						break;
					case 1 :
						strncpy((char*)g_arSetSndTypAsc, (char*)"Melody", 6);
						break;
				}
				switch(g_deSetSndVlmNum)
				{
					case 0:
						strncpy((char*)g_arSetSndVlmAsc, (char*)"Off ", 4);
						break;
					case 1 :
						strncpy((char*)g_arSetSndVlmAsc, (char*)"Low ", 4);
						break;
					case 2 :
						strncpy((char*)g_arSetSndVlmAsc, (char*)"Mid ", 4);
						break;
					case 3 :
						strncpy((char*)g_arSetSndVlmAsc, (char*)"High", 4);
						break;
				}
				break;
			default :
				break;
		}

		g_deBkgdStt = DETAILS_STATE;
		g_deSw3ShortFlg = 0;
	}

	if((g_deSw3LongFlg == 0) && (g_deSw4LongFlg == 1))			// Change to Main Screen
	{
		g_deBkgdStt = MAIN_STATE;
		g_deMainSelStt = 2;
//		g_deSw4LongFlg = 0;
	}

	// Command Parameter Reset
	if((g_deSw3LongFlg == 1) && (g_deSw4LongFlg == 1))
	{
		if(g_deSwCmdFlg == 2)
		{
			if(g_deSw3LongFlg == 1)	// Parameter Reset
			{
				CPU_I2C.ResetFlag = 0;

				I2C_INIT();
				HAL_Delay(10);

				NVIC_SystemReset();
			}
		}
	}
}

void LCD_DetailsAlarmStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 0)	// if Detail(0.3um, 0.5um, 1.0um, 2.5um, 5.0um, 10.um) is Unselected,
				g_deDetSelStt = 6;
			else
				g_deDetSelStt--;
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected 0.3um
					if(g_deSetAl003Num < g_arSetAlMax[g_deChSelStt])
						g_deSetAl003Num += g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl003Num = 9999;
					ConvertNum2AscDec(g_deSetAl003Num, g_arSetAl003Asc, 4);
					break;
				case 2 :						// Selected 0.5um
					if(g_deSetAl005Num < g_arSetAlMax[g_deChSelStt])
						g_deSetAl005Num += g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl005Num = 9999;
					ConvertNum2AscDec(g_deSetAl005Num, g_arSetAl005Asc, 4);
					break;
				case 3 :						// Selected 1.0um
					if(g_deSetAl010Num < g_arSetAlMax[g_deChSelStt])
						g_deSetAl010Num += g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl010Num = 9999;
					ConvertNum2AscDec(g_deSetAl010Num, g_arSetAl010Asc, 4);
					break;
				case 4 :						// Selected 2.5um
					if(g_deSetAl025Num < g_arSetAlMax[g_deChSelStt])
						g_deSetAl025Num += g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl025Num = 9999;
					ConvertNum2AscDec(g_deSetAl025Num, g_arSetAl025Asc, 4);
					break;
				case 5 :						// Selected 5.0um
					if(g_deSetAl050Num < g_arSetAlMax[g_deChSelStt])
						g_deSetAl050Num += g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl050Num = 9999;
					ConvertNum2AscDec(g_deSetAl050Num, g_arSetAl050Asc, 4);
					break;
				case 6 :						// Selected  10um
					if(g_deSetAl100Num < g_arSetAlMax[g_deChSelStt])
						g_deSetAl100Num += g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl100Num = 9999;
					ConvertNum2AscDec(g_deSetAl100Num, g_arSetAl100Asc, 4);
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 6)	// if Detail(0.3um, 0.5um, 1.0um, 2.5um, 5.0um, 10.um) is Unselected,
				g_deDetSelStt = 0;
			else
				g_deDetSelStt++;
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected 0.3um
					if(g_deSetAl003Num > g_arSetAlMin[g_deChSelStt])
						g_deSetAl003Num -= g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl003Num = 0;
					ConvertNum2AscDec(g_deSetAl003Num, g_arSetAl003Asc, 4);
					break;
				case 2 :						// Selected 0.5um
					if(g_deSetAl005Num > g_arSetAlMin[g_deChSelStt])
						g_deSetAl005Num -= g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl005Num = 0;
					ConvertNum2AscDec(g_deSetAl005Num, g_arSetAl005Asc, 4);
					break;
				case 3 :						// Selected 1.0um
					if(g_deSetAl010Num > g_arSetAlMin[g_deChSelStt])
						g_deSetAl010Num -= g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl010Num = 0;
					ConvertNum2AscDec(g_deSetAl010Num, g_arSetAl010Asc, 4);
					break;
				case 4 :						// Selected 2.5um
					if(g_deSetAl025Num > g_arSetAlMin[g_deChSelStt])
						g_deSetAl025Num -= g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl025Num = 0;
					ConvertNum2AscDec(g_deSetAl025Num, g_arSetAl025Asc, 4);
					break;
				case 5 :						// Selected 5.0um
					if(g_deSetAl050Num > g_arSetAlMin[g_deChSelStt])
						g_deSetAl050Num -= g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl050Num = 0;
					ConvertNum2AscDec(g_deSetAl050Num, g_arSetAl050Asc, 4);
					break;
				case 6 :						// Selected  10um
					if(g_deSetAl100Num > g_arSetAlMin[g_deChSelStt])
						g_deSetAl100Num -= g_arSetAlSum[g_deChSelStt];
					else
						g_deSetAl100Num = 0;
					ConvertNum2AscDec(g_deSetAl100Num, g_arSetAl100Asc, 4);
					break;
				default :
					break;
			}
		}
		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			g_arMainAlarmNum[1] = g_deSetAl003Num;
			g_arMainAlarmNum[2] = g_deSetAl005Num;
			g_arMainAlarmNum[3] = g_deSetAl010Num;
			g_arMainAlarmNum[4] = g_deSetAl025Num;
			g_arMainAlarmNum[5] = g_deSetAl050Num;
			g_arMainAlarmNum[6] = g_deSetAl100Num;

			CPU_I2C.Alarm003 = g_arMainAlarmNum[1];
			CPU_I2C.Alarm005 = g_arMainAlarmNum[2];
			CPU_I2C.Alarm010 = g_arMainAlarmNum[3];
			CPU_I2C.Alarm025 = g_arMainAlarmNum[4];
			CPU_I2C.Alarm050 = g_arMainAlarmNum[5];
			CPU_I2C.Alarm100 = g_arMainAlarmNum[6];

			I2C1_WRITE();
			HAL_Delay(100);

			g_deBkgdStt = SETTING_STATE;
		}
		else						// if Select Other Details
		{
			// variable character range : 13~16
			if(g_deChSelStt == 0)
			{
				g_deChSelStt = 13;
			}
			else if(g_deChSelStt < 16)
			{
				g_deChSelStt++;
			}
			else
			{
				g_deChSelStt = 0;
			}
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
		}
		g_deSw4ShortFlg = 0;
	}
}
void LCD_DetailsDatetimeStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 0)	// if Detail(0.3um, 0.5um, 1.0um, 2.5um, 5.0um, 10.um) is Unselected,
				g_deDetSelStt = 5;
			else
			{
				g_deDetSelStt--;
				if((g_deDetSelStt == 2) || (g_deDetSelStt == 4))
					g_deDetSelStt--;
			}
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Date
					if(g_arSetDateNum[(g_deChSelStt-4)/3] < g_arSetDateMax[g_deChSelStt])
						g_arSetDateNum[(g_deChSelStt-4)/3] += g_arSetDateSum[g_deChSelStt];
					else
					{
						switch(g_deChSelStt)
						{
							case 4 :
							case 5 :
								g_arSetDateNum[0] = g_arSetDateMax[5];
								break;
							case 7 :
							case 8 :
								g_arSetDateNum[1] = g_arSetDateMax[8];
								break;
							case 10 :
							case 11 :
								g_arSetDateNum[2] = g_arSetDateMax[11];
								break;
						}
					}
					switch(g_arSetDateNum[1])
					{
						case 1 :
						case 3 :
						case 5 :
						case 7 :
						case 8 :
						case 10 :
						case 12 :
							g_arSetDateMax[10] = 22;
							g_arSetDateMax[11] = 31;
							break;
						case 4 :
						case 6 :
						case 9 :
						case 11 :
							g_arSetDateMax[10] = 21;
							g_arSetDateMax[11] = 30;
							break;
						case 2 :
							if((g_arSetDateNum[0] % 4) == 0)
							{
								g_arSetDateMax[10] = 20;
								g_arSetDateMax[11] = 29;
							}
							else
							{
								g_arSetDateMax[10] = 19;
								g_arSetDateMax[11] = 28;
							}
							break;
						default :
							break;
					}
					g_arSetDateAsc[0] = (g_arSetDateNum[0]/10)%10+'0';
					g_arSetDateAsc[1] = g_arSetDateNum[0]%10+'0';
					ConvertNum2AscDec(g_arSetDateNum[1], g_arSetDateAsc+3, 2);
					ConvertNum2AscDec(g_arSetDateNum[2], g_arSetDateAsc+6, 2);
					break;
				case 3 :						// Selected Time
					if(g_arSetTimeNum[(g_deChSelStt-4)/3] < g_arSetTimeMax[g_deChSelStt])
						g_arSetTimeNum[(g_deChSelStt-4)/3] += g_arSetTimeSum[g_deChSelStt];
					else
					{
						switch(g_deChSelStt)
						{
							case 4 :
							case 5 :
								g_arSetTimeNum[0] = g_arSetTimeMax[5];
								break;
							case 7 :
							case 8 :
								g_arSetTimeNum[1] = g_arSetTimeMax[8];
								break;
							case 10 :
							case 11 :
								g_arSetTimeNum[2] = g_arSetTimeMax[11];
								break;
						}
					}
					ConvertNum2AscDec(g_arSetTimeNum[0], g_arSetTimeAsc, 2);
					ConvertNum2AscDec(g_arSetTimeNum[1], g_arSetTimeAsc+3, 2);
					ConvertNum2AscDec(g_arSetTimeNum[2], g_arSetTimeAsc+6, 2);
					break;
				case 5 :
					if(g_deSetLifeNum < g_arSetLifeMax[g_deChSelStt])
						g_deSetLifeNum += g_arSetLifeSum[g_deChSelStt];
					else
						g_deSetLifeNum = 499999;
					ConvertNum2AscDec(g_deSetLifeNum, g_arSetLifeTimeAsc, 6);
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 5)	// if Detail(0.3um, 0.5um, 1.0um, 2.5um, 5.0um, 10.um) is Unselected,
				g_deDetSelStt = 0;
			else
			{
				g_deDetSelStt++;
				if((g_deDetSelStt == 2) || (g_deDetSelStt == 4))
					g_deDetSelStt++;
			}
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Date
					if(g_arSetDateNum[(g_deChSelStt-4)/3] > g_arSetDateMin[g_deChSelStt])
						g_arSetDateNum[(g_deChSelStt-4)/3] -= g_arSetDateSum[g_deChSelStt];
					else
					{
						switch(g_deChSelStt)
						{
							case 4 :
							case 5 :
								g_arSetDateNum[0] = g_arSetDateMin[5];
								break;
							case 7 :
							case 8 :
								g_arSetDateNum[1] = g_arSetDateMin[8];
								break;
							case 10 :
							case 11 :
								g_arSetDateNum[2] = g_arSetDateMin[11];
								break;
						}
					}
					switch(g_arSetDateNum[1])
					{
						case 1 :
						case 3 :
						case 5 :
						case 7 :
						case 8 :
						case 10 :
						case 12 :
							g_arSetDateMax[10] = 22;
							g_arSetDateMax[11] = 31;
							break;
						case 4 :
						case 6 :
						case 9 :
						case 11 :
							g_arSetDateMax[10] = 21;
							g_arSetDateMax[11] = 30;
							break;
						case 2 :
							if((g_arSetDateNum[0] % 4) == 0)
							{
								g_arSetDateMax[10] = 20;
								g_arSetDateMax[11] = 29;
							}
							else
							{
								g_arSetDateMax[10] = 19;
								g_arSetDateMax[11] = 28;
							}
							break;
						default :
							break;
					}
					g_arSetDateAsc[0] = (g_arSetDateNum[0]/10)%10+'0';
					g_arSetDateAsc[1] = g_arSetDateNum[0]%10+'0';
					ConvertNum2AscDec(g_arSetDateNum[1], g_arSetDateAsc+3, 2);
					ConvertNum2AscDec(g_arSetDateNum[2], g_arSetDateAsc+6, 2);
					break;
				case 3 :						// Selected Time
					if(g_arSetTimeNum[(g_deChSelStt-4)/3] > g_arSetTimeMin[g_deChSelStt])
						g_arSetTimeNum[(g_deChSelStt-4)/3] -= g_arSetTimeSum[g_deChSelStt];
					else
					{
						switch(g_deChSelStt)
						{
							case 4 :
							case 5 :
								g_arSetTimeNum[0] = g_arSetTimeMin[5];
								break;
							case 7 :
							case 8 :
								g_arSetTimeNum[1] = g_arSetTimeMin[8];
								break;
							case 10 :
							case 11 :
								g_arSetTimeNum[2] = g_arSetTimeMin[11];
								break;
						}
					}
					ConvertNum2AscDec(g_arSetTimeNum[0], g_arSetTimeAsc, 2);
					ConvertNum2AscDec(g_arSetTimeNum[1], g_arSetTimeAsc+3, 2);
					ConvertNum2AscDec(g_arSetTimeNum[2], g_arSetTimeAsc+6, 2);
					break;
				case 5 :
					if(g_deSetLifeNum > g_arSetLifeMin[g_deChSelStt])
						g_deSetLifeNum -= g_arSetLifeSum[g_deChSelStt];
					else
						g_deSetLifeNum = 0;
					ConvertNum2AscDec(g_deSetLifeNum, g_arSetLifeTimeAsc, 6);
					break;
				default :
					break;
			}
		}
		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			sDate.Year = g_arSetDateNum[0];
			sDate.Month = g_arSetDateNum[1];
			sDate.Date = g_arSetDateNum[2];
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			sTime.Hours = g_arSetTimeNum[0];
			sTime.Minutes = g_arSetTimeNum[1];
			sTime.Seconds = g_arSetTimeNum[2];
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

			g_deMainLifetimeNum = g_deSetLifeNum;
			CPU_I2C.AlarmLifetime = g_deMainLifetimeNum;

			I2C1_WRITE();
			HAL_Delay(100);
			g_deBkgdStt = SETTING_STATE;
		}
		else if(g_deDetSelStt != 5)	// if Selected 'Date' or 'Time'
		{
			if(g_deChSelStt == 0)
			{
				g_deChSelStt = 4;
			}
			else if(g_deChSelStt < 11)
			{
				g_deChSelStt++;
				if((g_deChSelStt == 6) || (g_deChSelStt == 9))
					g_deChSelStt++;
			}
			else
			{
				g_deChSelStt = 0;
			}
		}
		else						// if Selected 'UpTime', It Can be Reset Using Only Command
		{
			if(g_deChSelStt == 0)
			{
				g_deChSelStt = 11;
			}
			else if(g_deChSelStt < 16)
			{
				g_deChSelStt++;
			}
			else
			{
				g_deChSelStt = 0;
			}
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
		}
		g_deSw4ShortFlg = 0;
	}
}
void LCD_DetailsModeStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 0)
				g_deDetSelStt = 8;
			else if(g_deDetSelStt == 7)
				g_deDetSelStt = 5;
			else
				g_deDetSelStt --;
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected External Input Ouput
					g_deSetModeAlarmNum = 1;
					strncpy((char*)g_arSetModeAlarmAsc, (char*)"Used    ", 8);
					break;
				case 2 :						// Selected Recommended Standard 485
					g_deSetModeIoNum = 1;
					strncpy((char*)g_arSetModeIoAsc, (char*)"Used    ", 8);
					break;
				case 3 :						// Selected Local Area Network
					g_deSetMode485Num = 1;
					strncpy((char*)g_arSetMode485Asc, (char*)"Used    ", 8);
					break;
				case 4 :
					if(g_deSetModeLanNum < 3)
						g_deSetModeLanNum++;
					else
						g_deSetModeLanNum = 3;
					switch(g_deSetModeLanNum)
					{
						case 1 :
							strncpy((char*)g_arSetModeLanAsc, (char*)"Server  ", 8);
							break;
						case 2 :
							strncpy((char*)g_arSetModeLanAsc, (char*)"Client  ", 8);
							break;
						case 3 :
							strncpy((char*)g_arSetModeLanAsc, (char*)"Clnt&Srv", 8);
							break;
						default :
							break;
					}
					break;
				case 5 :
					// TH Sensor is Reserved
//					g_deSetModeThNum = 1;
					strncpy((char*)g_arSetModeThAsc, (char*)"Used    ", 8);
					break;
				case 7 :						// Simple Moving Average
					if(g_deSetModeSmaNum < 60)
						g_deSetModeSmaNum++;
					else
						g_deSetModeSmaNum = 60;
					ConvertNum2AscDec(g_deSetModeSmaNum, g_arSetModeSmaAsc, 2);
					while(g_arSetModeSmaAsc[0] == ' ')
					{
						g_arSetModeSmaAsc[0] = g_arSetModeSmaAsc[1];
						g_arSetModeSmaAsc[1] = ' ';
					}
					break;
				case 8 :						// Selected Defreezing
					g_deSetModeDefNum = 1;
					strncpy((char*)g_arSetModeDefAsc, (char*)"Used    ", 8);
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 8)
				g_deDetSelStt = 0;
			else if(g_deDetSelStt == 5)
				g_deDetSelStt = 7;
			else
				g_deDetSelStt++;
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :
					g_deSetModeAlarmNum = 0;
					strncpy((char*)g_arSetModeAlarmAsc, (char*)"Unused  ", 8);
					break;
				case 2 :						// Selected External Input Ouput
					g_deSetModeIoNum = 0;
					strncpy((char*)g_arSetModeIoAsc, (char*)"Unused  ", 8);
					break;
				case 3 :						// Selected Recommended Standard 485
					g_deSetMode485Num = 0;
					strncpy((char*)g_arSetMode485Asc, (char*)"Unused  ", 8);
					break;
				case 4 :						// Selected Local Area Network
					if(g_deSetModeLanNum > 0)
						g_deSetModeLanNum--;
					else
						g_deSetModeLanNum = 0;
					switch(g_deSetModeLanNum)
					{
						case 0:
							strncpy((char*)g_arSetModeLanAsc, (char*)"Unused  ", 8);
							break;
						case 1 :
							strncpy((char*)g_arSetModeLanAsc, (char*)"Server  ", 8);
							break;
						case 2 :
							strncpy((char*)g_arSetModeLanAsc, (char*)"Client  ", 8);
							break;
						default :
							break;
					}
					break;
				case 5 :
					// TH Sensor is Reserved
//					g_deSetModeThNum = 0;
					strncpy((char*)g_arSetModeThAsc, (char*)"Unused  ", 8);
					break;
				case 7 :						// Simple Moving Average
					if(g_deSetModeSmaNum > 1)
						g_deSetModeSmaNum--;
					else
						g_deSetModeSmaNum = 1;
					ConvertNum2AscDec(g_deSetModeSmaNum, g_arSetModeSmaAsc, 2);
					while(g_arSetModeSmaAsc[0] == ' ')
					{
						g_arSetModeSmaAsc[0] = g_arSetModeSmaAsc[1];
						g_arSetModeSmaAsc[1] = ' ';
					}
					break;
				case 8 :						// Defreezing
					g_deSetModeDefNum = 0;
					strncpy((char*)g_arSetModeDefAsc, (char*)"Unused  ", 8);
					break;
				default :
					break;
			}
		}
		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			g_deMainModeAlarmNum = g_deSetModeAlarmNum;
			g_deMainModeIoNum = g_deSetModeIoNum;
			g_deMainMode485Num = g_deSetMode485Num;
			g_deMainModeLanNum = g_deSetModeLanNum;
			g_deMainModeWlssNum = g_deSetModeWlssNum;
			g_deMainModeThNum = g_deSetModeThNum;
			g_deMainModeSmaNum = g_deSetModeSmaNum;
			g_deMainModeDefNum = g_deSetModeDefNum;
			CPU_I2C.ModeAlarm = g_deMainModeAlarmNum;
			CPU_I2C.ModeIo = g_deMainModeIoNum;
			CPU_I2C.Mode485 = g_deMainMode485Num;
			CPU_I2C.ModeLan = g_deMainModeLanNum;
			CPU_I2C.ModeWlss = g_deMainModeWlssNum;
			CPU_I2C.ModeTh = g_deMainModeThNum;
			CPU_I2C.ModeSma = g_deMainModeSmaNum;
			CPU_I2C.ModeDef = g_deMainModeDefNum;

			I2C1_WRITE();
			HAL_Delay(10);

			g_deSetListStt = ((!!g_deMainModeThNum) << 3) | ((!!g_deMainModeWlssNum) << 2) | ((!!g_deMainModeLanNum) << 1) | ((!!g_deMainMode485Num) << 0);
			g_deSetListMax = 4+(!!g_deMainModeThNum)+(!!g_deMainModeWlssNum)+(!!g_deMainModeLanNum)+(!!g_deMainMode485Num);

			g_deSetPageStt = 1;
			g_deBkgdStt = SETTING_STATE;
		}
		else						// if Select Other Details
		{
			if(g_deChSelStt == 0)
			{
				g_deChSelStt = 14;
			}
			else
			{
				g_deChSelStt = 0;
			}
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
		}
		g_deSw4ShortFlg = 0;
	}
}
void LCD_DetailsSerialStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if(g_deDetSelStt == 0)
				g_deDetSelStt = ((g_deSet485PtclNum == 1) ? 10 : 9);
			else if(g_deDetSelStt == 7)
				g_deDetSelStt = 5;
			else
				g_deDetSelStt --;
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Baud Rate
					if(g_deSet485BaudNum == 38400)
						g_deSet485BaudNum = 57600;
					else if(g_deSet485BaudNum < 921600)
						g_deSet485BaudNum <<= 1;
					else
						g_deSet485BaudNum = 921600;
					ConvertNum2AscDec(g_deSet485BaudNum, g_arSet485BaudAsc, 6);
					while(g_arSet485BaudAsc[0] == ' ')
					{
						g_arSet485BaudAsc[0] = g_arSet485BaudAsc[1];
						g_arSet485BaudAsc[1] = g_arSet485BaudAsc[2];
						g_arSet485BaudAsc[2] = g_arSet485BaudAsc[3];
						g_arSet485BaudAsc[3] = g_arSet485BaudAsc[4];
						g_arSet485BaudAsc[4] = g_arSet485BaudAsc[5];
						g_arSet485BaudAsc[5] = ' ';
					}
					break;
				case 2 :						// Selected Data Bits
					g_deSet485DataNum = 8;
					g_arSet485DataAsc[0] = '8';
					break;
				case 3 :						// Selected Parity Bits
					if(g_deSet485PrtyNum < 2)
						g_deSet485PrtyNum++;
					else
						g_deSet485PrtyNum = 2;
					switch(g_deSet485PrtyNum)
					{
						case 1 :
							strncpy((char*)g_arSet485PrtyAsc, (char*)"Odd ", 4);
							break;
						case 2 :
							strncpy((char*)g_arSet485PrtyAsc, (char*)"Even", 4);
							break;
						default :
							break;
					}
					break;
				case 4 :						// Selected Stop Bits
					g_deSet485StopNum = 2;
					g_arSet485StopAsc[0] = '2';
					break;
				case 5 :						// Selected Data Flow
					g_deSet485FlowNum = 1;
					strncpy((char*)g_arSet485FlowAsc, (char*)"Soft", 4);
					break;
				case 7 :						// Selected Module ID
					if(g_deSet485ModNum < 63)
						g_deSet485ModNum++;
					else
						g_deSet485ModNum = 63;
					ConvertNum2AscDec(g_deSet485ModNum, g_arSet485ModAsc, 2);
					while(g_arSet485ModAsc[0] == ' ')
					{
						g_arSet485ModAsc[0] = g_arSet485ModAsc[1];
						g_arSet485ModAsc[1] = ' ';
					}
					break;
				case 8 :						// Selected Destination ID
					if(g_deSet485DestNum < 63)
						g_deSet485DestNum++;
					else
						g_deSet485DestNum = 63;
					ConvertNum2AscDec(g_deSet485DestNum, g_arSet485DestAsc, 2);
					while(g_arSet485DestAsc[0] == ' ')
					{
						g_arSet485DestAsc[0] = g_arSet485DestAsc[1];
						g_arSet485DestAsc[1] = ' ';
					}
					break;
				case 9 :						// Selected Protocol
					g_deSet485PtclNum = 1;
					strncpy((char*)g_arSet485PtclAsc, (char*)"MC Fmt5 ", 8);
					break;
				case 10 :						// Selected Device Code/Leader
					if(g_deChSelStt == 14)
					{
						g_deSet485DvcdNum = 0xb4;
						strncpy((char*)g_arSet485DvcdAsc, (char*)"W", 1);
					}
					else
					{
						if(g_deSet485DvldNum < g_arSetDvMax[g_deChSelStt])
							g_deSet485DvldNum += g_arSetDvSum[g_deChSelStt];
						else
							g_deSet485DvldNum = 49999;
						g_arSet485DvldAsc[0] = (g_deSet485DvldNum/10000)%10+'0';
						g_arSet485DvldAsc[1] = (g_deSet485DvldNum/1000)%10+'0';
						g_arSet485DvldAsc[2] = (g_deSet485DvldNum/100)%10+'0';
						g_arSet485DvldAsc[3] = (g_deSet485DvldNum/10)%10+'0';
						g_arSet485DvldAsc[4] = g_deSet485DvldNum%10+'0';
					}
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if(g_deDetSelStt == ((g_deSet485PtclNum == 1) ? 10 : 9))
				g_deDetSelStt = 0;
			else if(g_deDetSelStt == 5)
				g_deDetSelStt = 7;
			else
				g_deDetSelStt++;
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Baud Rate
					if(g_deSet485BaudNum == 57600)
						g_deSet485BaudNum = 38400;
					else if(g_deSet485BaudNum > 2400)
						g_deSet485BaudNum >>= 1;
					else
						g_deSet485BaudNum = 2400;
					ConvertNum2AscDec(g_deSet485BaudNum, g_arSet485BaudAsc, 6);
					while(g_arSet485BaudAsc[0] == ' ')
					{
						g_arSet485BaudAsc[0] = g_arSet485BaudAsc[1];
						g_arSet485BaudAsc[1] = g_arSet485BaudAsc[2];
						g_arSet485BaudAsc[2] = g_arSet485BaudAsc[3];
						g_arSet485BaudAsc[3] = g_arSet485BaudAsc[4];
						g_arSet485BaudAsc[4] = g_arSet485BaudAsc[5];
						g_arSet485BaudAsc[5] = ' ';
					}
					break;
				case 2 :						// Selected Data Bits
					g_deSet485DataNum = 7;
					g_arSet485DataAsc[0] = '7';
					break;
				case 3 :						// Selected Parity Bits
					if(g_deSet485PrtyNum > 0)
						g_deSet485PrtyNum--;
					else
						g_deSet485PrtyNum = 0;
					switch(g_deSet485PrtyNum)
					{
						case 0 :
							strncpy((char*)g_arSet485PrtyAsc, (char*)"None", 4);
							break;
						case 1 :
							strncpy((char*)g_arSet485PrtyAsc, (char*)"Odd ", 4);
							break;
						default :
							break;
					}
					break;
				case 4 :						// Selected Stop Bits
					g_deSet485StopNum = 1;
					g_arSet485StopAsc[0] = '1';
					break;
				case 5 :						// Selected Data Flow
					g_deSet485FlowNum = 0;
					strncpy((char*)g_arSet485FlowAsc, (char*)"None", 4);
					break;
				case 7 :						// Selected Module ID
					if(g_deSet485ModNum > 0)
						g_deSet485ModNum--;
					else
						g_deSet485ModNum = 0;
					ConvertNum2AscDec(g_deSet485ModNum, g_arSet485ModAsc, 2);
					while(g_arSet485ModAsc[0] == ' ')
					{
						g_arSet485ModAsc[0] = g_arSet485ModAsc[1];
						g_arSet485ModAsc[1] = ' ';
					}
					break;
				case 8 :						// Selected Destination ID
					if(g_deSet485DestNum > 0)
						g_deSet485DestNum--;
					else
						g_deSet485DestNum = 0;
					ConvertNum2AscDec(g_deSet485DestNum, g_arSet485DestAsc, 2);
					while(g_arSet485DestAsc[0] == ' ')
					{
						g_arSet485DestAsc[0] = g_arSet485DestAsc[1];
						g_arSet485DestAsc[1] = ' ';
					}
					break;
				case 9 :						// Selected Protocol
					g_deSet485PtclNum = 0;
					strncpy((char*)g_arSet485PtclAsc, (char*)"ACT     ", 8);
					break;
				case 10 :
					if(g_deChSelStt == 14)
					{
						g_deSet485DvcdNum = 0xa8;
						strncpy((char*)g_arSet485DvcdAsc, (char*)"D", 1);
					}
					else
					{
						if(g_deSet485DvldNum > g_arSetDvMin[g_deChSelStt])
							g_deSet485DvldNum -= g_arSetDvSum[g_deChSelStt];
						else
							g_deSet485DvldNum = 0;
						g_arSet485DvldAsc[0] = (g_deSet485DvldNum/10000)%10+'0';
						g_arSet485DvldAsc[1] = (g_deSet485DvldNum/1000)%10+'0';
						g_arSet485DvldAsc[2] = (g_deSet485DvldNum/100)%10+'0';
						g_arSet485DvldAsc[3] = (g_deSet485DvldNum/10)%10+'0';
						g_arSet485DvldAsc[4] = g_deSet485DvldNum%10+'0';
					}
					break;
				default :
					break;
			}
		}
		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			g_deMain485BaudNum = g_deSet485BaudNum;
			g_deMain485DataNum = g_deSet485DataNum;
			g_deMain485PrtyNum = g_deSet485PrtyNum;
			g_deMain485StopNum = g_deSet485StopNum;
			g_deMain485FlowNum = g_deSet485FlowNum;
			g_deMain485ModNum = g_deSet485ModNum;
			g_deMain485DestNum = g_deSet485DestNum;
			g_deMain485PtclNum = g_deSet485PtclNum;
			g_deMain485DvcdNum = g_deSet485DvcdNum;
			g_deMain485DvldNum = g_deSet485DvldNum;

			CPU_I2C.Rs485Baud = g_deMain485BaudNum;
			CPU_I2C.Rs485Data = g_deMain485DataNum;
			CPU_I2C.Rs485Prty = g_deMain485PrtyNum;
			CPU_I2C.Rs485Stop = g_deMain485StopNum;
			CPU_I2C.Rs485Flow = g_deMain485FlowNum;
			CPU_I2C.Rs485Mod = g_deMain485ModNum;
			CPU_I2C.Rs485Dest = g_deMain485DestNum;
			CPU_I2C.Rs485Ptcl = g_deMain485PtclNum;
			CPU_I2C.Rs485Dvcd = g_deMain485DvcdNum;
			CPU_I2C.Rs485Dvld = g_deMain485DvldNum;

			I2C1_WRITE();
			HAL_Delay(10);

			g_deBkgdStt = SETTING_STATE;

			NVIC_SystemReset();
		}
		else						// if Select Other Details
		{
			if(g_deChSelStt == 0)
			{
				g_deChSelStt = 14;
			}
			else
			{
				if(g_deDetSelStt == 10)
				{
					g_deChSelStt++;
					if(g_deChSelStt == 20)
						g_deChSelStt = 0;
				}
				else
					g_deChSelStt = 0;
			}
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
		}

		g_deSw4ShortFlg = 0;
	}
}
void LCD_DetailsNetworkStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if((g_deMainModeLanNum & 2) == 2)	// if Include Client Mode, 2 Pages
			{
				if(g_deDetSelStt == 0)
					g_deDetSelStt = ((g_deSetLanPtclNum == 1) ? 11 : 10);
				else if(g_deDetSelStt == 8)
					g_deDetSelStt = 6;
				else if(g_deDetSelStt == 2)
					g_deDetSelStt = 0;
				else
					g_deDetSelStt --;
			}
			else								// if Exclude Client Mode, 1 Page
			{
				if(g_deDetSelStt == 0)
					g_deDetSelStt = 6;
				else if(g_deDetSelStt == 2)
					g_deDetSelStt = 0;
				else
					g_deDetSelStt --;
			}
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 2 :						// Selected ModIp
					if(g_arSetLanModIpNum[(g_deChSelStt-9)/4] < g_arSetAddrMax[g_deChSelStt])
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] += g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] = 255;
					ConvertNum2AscDec(g_arSetLanModIpNum[0], g_arSetLanModIpAsc, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[1], g_arSetLanModIpAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[2], g_arSetLanModIpAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[3], g_arSetLanModIpAsc+12, 3);
					break;
				case 3 :						// Selected ModSubnet
					if(g_arSetLanModSubNum[(g_deChSelStt-9)/4] < g_arSetAddrMax[g_deChSelStt])
						g_arSetLanModSubNum[(g_deChSelStt-9)/4] += g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModSubNum[(g_deChSelStt-9)/4] = 255;
					ConvertNum2AscDec(g_arSetLanModSubNum[0], g_arSetLanModSubAsc, 3);
					ConvertNum2AscDec(g_arSetLanModSubNum[1], g_arSetLanModSubAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModSubNum[2], g_arSetLanModSubAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModSubNum[3], g_arSetLanModSubAsc+12, 3);
					break;
				case 4 :						// Selected ModGateway
					if(g_arSetLanModGwNum[(g_deChSelStt-9)/4] < g_arSetAddrMax[g_deChSelStt])
						g_arSetLanModGwNum[(g_deChSelStt-9)/4] += g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModGwNum[(g_deChSelStt-9)/4] = 255;
					ConvertNum2AscDec(g_arSetLanModGwNum[0], g_arSetLanModGwAsc, 3);
					ConvertNum2AscDec(g_arSetLanModGwNum[1], g_arSetLanModGwAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModGwNum[2], g_arSetLanModGwAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModGwNum[3], g_arSetLanModGwAsc+12, 3);
					break;
				case 5 :						// Selected ModPort
					if(g_deSetLanModPortNum < g_arSetPortMax[g_deChSelStt])
						g_deSetLanModPortNum += g_arSetPortSum[g_deChSelStt];
					else
						g_deSetLanModPortNum = 65535;
					ConvertNum2AscDec(g_deSetLanModPortNum, g_arSetLanModPortAsc, 5);
					break;
				case 6 :						// Selected ModMac
					if(g_arSetLanModMacNum[(g_deChSelStt-9)/3] < g_arSetMacMax[g_deChSelStt])
						g_arSetLanModMacNum[(g_deChSelStt-9)/3] += g_arSetMacSum[g_deChSelStt];
					else
						g_arSetLanModMacNum[(g_deChSelStt-9)/3] = 255;
					ConvertNum2AscHex(g_arSetLanModMacNum[0], g_arSetLanModMacAsc, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[1], g_arSetLanModMacAsc+3, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[2], g_arSetLanModMacAsc+6, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[3], g_arSetLanModMacAsc+9, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[4], g_arSetLanModMacAsc+12, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[5], g_arSetLanModMacAsc+15, 2);
					break;
				case 8 :						// Selected DestIp
					if(g_arSetLanDestIpNum[(g_deChSelStt-9)/4] < g_arSetAddrMax[g_deChSelStt])
						g_arSetLanDestIpNum[(g_deChSelStt-9)/4] += g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanDestIpNum[(g_deChSelStt-9)/4] = 255;
					ConvertNum2AscDec(g_arSetLanDestIpNum[0], g_arSetLanDestIpAsc, 3);
					ConvertNum2AscDec(g_arSetLanDestIpNum[1], g_arSetLanDestIpAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanDestIpNum[2], g_arSetLanDestIpAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanDestIpNum[3], g_arSetLanDestIpAsc+12, 3);
					break;
				case 9 :						// Selected DestPort
					if(g_deSetLanDestPortNum < g_arSetPortMax[g_deChSelStt])
						g_deSetLanDestPortNum += g_arSetPortSum[g_deChSelStt];
					else
						g_deSetLanDestPortNum = 65535;
					ConvertNum2AscDec(g_deSetLanDestPortNum, g_arSetLanDestPortAsc, 5);
					break;
				case 10 :						// Selected ServerPort
					g_deSetLanPtclNum = 1;
					strncpy((char*)g_arSetLanPtclAsc, (char*)"MC      ", 8);
					break;
				case 11 :						// Selected Device Code/Leader
					if(g_deChSelStt == 14)
					{
						g_deSetLanDvcdNum = 0xb4;
						strncpy((char*)g_arSetLanDvcdAsc, (char*)"W", 1);
					}
					else
					{
						if(g_deSetLanDvldNum < g_arSetDvMax[g_deChSelStt])
							g_deSetLanDvldNum += g_arSetDvSum[g_deChSelStt];
						else
							g_deSetLanDvldNum = 49999;
						g_arSetLanDvldAsc[0] = (g_deSetLanDvldNum/10000)%10+'0';
						g_arSetLanDvldAsc[1] = (g_deSetLanDvldNum/1000)%10+'0';
						g_arSetLanDvldAsc[2] = (g_deSetLanDvldNum/100)%10+'0';
						g_arSetLanDvldAsc[3] = (g_deSetLanDvldNum/10)%10+'0';
						g_arSetLanDvldAsc[4] = g_deSetLanDvldNum%10+'0';
					}
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if((g_deMainModeLanNum & 2) == 2)	// if Include Client Mode, 2 Pages
			{
				if(g_deDetSelStt == ((g_deSetLanPtclNum == 1) ? 11 : 10))
					g_deDetSelStt = 0;
				else if(g_deDetSelStt == 0)
					g_deDetSelStt = 2;
				else if(g_deDetSelStt == 6)
					g_deDetSelStt = 8;
				else
					g_deDetSelStt++;
			}
			else								// if Exclude Client Mode, 1 Page
			{
				if(g_deDetSelStt == 6)
					g_deDetSelStt = 0;
				else if(g_deDetSelStt == 0)
					g_deDetSelStt = 2;
				else
					g_deDetSelStt++;
			}
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 2 :						// Selected SensorIp
					if(g_arSetLanModIpNum[(g_deChSelStt-9)/4] > g_arSetAddrMin[g_deChSelStt])
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] -= g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] = 0;
					ConvertNum2AscDec(g_arSetLanModIpNum[0], g_arSetLanModIpAsc, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[1], g_arSetLanModIpAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[2], g_arSetLanModIpAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[3], g_arSetLanModIpAsc+12, 3);
					break;
				case 3 :						// Selected SensorSubnet
					if(g_arSetLanModSubNum[(g_deChSelStt-9)/4] > g_arSetAddrMin[g_deChSelStt])
						g_arSetLanModSubNum[(g_deChSelStt-9)/4] -= g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModSubNum[(g_deChSelStt-9)/4] = 0;
					ConvertNum2AscDec(g_arSetLanModSubNum[0], g_arSetLanModSubAsc, 3);
					ConvertNum2AscDec(g_arSetLanModSubNum[1], g_arSetLanModSubAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModSubNum[2], g_arSetLanModSubAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModSubNum[3], g_arSetLanModSubAsc+12, 3);
					break;
				case 4 :						// Selected SensorGateway
					if(g_arSetLanModGwNum[(g_deChSelStt-9)/4] > g_arSetAddrMin[g_deChSelStt])
						g_arSetLanModGwNum[(g_deChSelStt-9)/4] -= g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModGwNum[(g_deChSelStt-9)/4] = 0;
					ConvertNum2AscDec(g_arSetLanModGwNum[0], g_arSetLanModGwAsc, 3);
					ConvertNum2AscDec(g_arSetLanModGwNum[1], g_arSetLanModGwAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModGwNum[2], g_arSetLanModGwAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModGwNum[3], g_arSetLanModGwAsc+12, 3);
					break;
				case 5 :						// Selected SensorPort
					if(g_deSetLanModPortNum > g_arSetPortMin[g_deChSelStt])
						g_deSetLanModPortNum -= g_arSetPortSum[g_deChSelStt];
					else
						g_deSetLanModPortNum = 0;
					ConvertNum2AscDec(g_deSetLanModPortNum, g_arSetLanModPortAsc, 5);
					break;
				case 6 :						// Selected ModMac
					if(g_arSetLanModMacNum[(g_deChSelStt-9)/3] > g_arSetMacMin[g_deChSelStt])
						g_arSetLanModMacNum[(g_deChSelStt-9)/3] -= g_arSetMacSum[g_deChSelStt];
					else
						g_arSetLanModMacNum[(g_deChSelStt-9)/3] = 0;
					ConvertNum2AscHex(g_arSetLanModMacNum[0], g_arSetLanModMacAsc, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[1], g_arSetLanModMacAsc+3, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[2], g_arSetLanModMacAsc+6, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[3], g_arSetLanModMacAsc+9, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[4], g_arSetLanModMacAsc+12, 2);
					ConvertNum2AscHex(g_arSetLanModMacNum[5], g_arSetLanModMacAsc+15, 2);
					break;
				case 8 :						// Selected ServerIp
					if(g_arSetLanDestIpNum[(g_deChSelStt-9)/4] > g_arSetAddrMin[g_deChSelStt])
						g_arSetLanDestIpNum[(g_deChSelStt-9)/4] -= g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanDestIpNum[(g_deChSelStt-9)/4] = 0;
					ConvertNum2AscDec(g_arSetLanDestIpNum[0], g_arSetLanDestIpAsc, 3);
					ConvertNum2AscDec(g_arSetLanDestIpNum[1], g_arSetLanDestIpAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanDestIpNum[2], g_arSetLanDestIpAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanDestIpNum[3], g_arSetLanDestIpAsc+12, 3);
					break;
				case 9 :						// Selected ServerPort
					if(g_deSetLanDestPortNum > g_arSetPortMin[g_deChSelStt])
						g_deSetLanDestPortNum -= g_arSetPortSum[g_deChSelStt];
					else
						g_deSetLanDestPortNum = 0;
					ConvertNum2AscDec(g_deSetLanDestPortNum, g_arSetLanDestPortAsc, 5);
					break;
				case 10 :						// Selected ServerPort
					g_deSetLanPtclNum = 0;
					strncpy((char*)g_arSetLanPtclAsc, (char*)"ACT     ", 8);
					break;
				case 11 :
					if(g_deChSelStt == 14)
					{
						g_deSetLanDvcdNum = 0xa8;
						strncpy((char*)g_arSetLanDvcdAsc, (char*)"D", 1);
					}
					else
					{
						if(g_deSetLanDvldNum > g_arSetDvMin[g_deChSelStt])
							g_deSetLanDvldNum -= g_arSetDvSum[g_deChSelStt];
						else
							g_deSetLanDvldNum = 0;
						g_arSetLanDvldAsc[0] = (g_deSetLanDvldNum/10000)%10+'0';
						g_arSetLanDvldAsc[1] = (g_deSetLanDvldNum/1000)%10+'0';
						g_arSetLanDvldAsc[2] = (g_deSetLanDvldNum/100)%10+'0';
						g_arSetLanDvldAsc[3] = (g_deSetLanDvldNum/10)%10+'0';
						g_arSetLanDvldAsc[4] = g_deSetLanDvldNum%10+'0';
					}
					break;
				default :
					break;
			}

		}

		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			memcpy(g_arMainLanModIpNum, g_arSetLanModIpNum, 4);
			memcpy(g_arMainLanModSubNum, g_arSetLanModSubNum, 4);
			memcpy(g_arMainLanModGwNum, g_arSetLanModGwNum, 4);
			memcpy(g_arMainLanModMacNum, g_arSetLanModMacNum, 6);
			g_deMainLanModPortNum = g_deSetLanModPortNum;
			memcpy(g_arMainLanDestIpNum, g_arSetLanDestIpNum, 4);
			g_deMainLanDestPortNum = g_deSetLanDestPortNum;
			g_deMainLanPtclNum = g_deSetLanPtclNum;
			g_deMainLanDvcdNum = g_deSetLanDvcdNum;
			g_deMainLanDvldNum = g_deSetLanDvldNum;

			memcpy(&CPU_I2C.LanModIp, g_arMainLanModIpNum, 4);
			memcpy(&CPU_I2C.LanModSub, g_arMainLanModSubNum, 4);
			memcpy(&CPU_I2C.LanModGw, g_arMainLanModGwNum, 4);
			memcpy(&CPU_I2C.LanModMac, g_arMainLanModMacNum, 6);
			CPU_I2C.LanModPort= g_deMainLanModPortNum;
			memcpy(&CPU_I2C.LanDestIp, g_arMainLanDestIpNum, 4);
			CPU_I2C.LanDestPort = g_deMainLanDestPortNum;
			CPU_I2C.LanPtcl = g_deMainLanPtclNum;
			CPU_I2C.LanDvcd = g_deMainLanDvcdNum;
			CPU_I2C.LanDvld = g_deMainLanDvldNum;

			I2C1_WRITE();
			HAL_Delay(10);

			g_deBkgdStt = SETTING_STATE;

			NVIC_SystemReset();
		}
		else						// if Select Other Details
		{
			if(g_deChSelStt == 0)
			{
				if((g_deDetSelStt == 10) || (g_deDetSelStt == 11))
					g_deChSelStt = 14;
				else
					g_deChSelStt = 9;
			}
			else if(g_deChSelStt < 25)
			{
				g_deChSelStt++;

				switch(g_deDetSelStt)
				{
					case 2 :
					case 3 :
					case 4 :
					case 8 :
						if((g_deChSelStt == 12) || (g_deChSelStt == 16) || (g_deChSelStt == 20))
							g_deChSelStt++;
						if(g_deChSelStt == 24)
							g_deChSelStt = 0;
						break;
					case 5 :
					case 9 :
						if(g_deChSelStt == 14)
							g_deChSelStt = 0;
						break;
					case 10 :
						g_deChSelStt = 0;
						break;
					case 11 :
						if(g_deChSelStt == 20)
							g_deChSelStt = 0;
						break;
					case 6 :
						if((g_deChSelStt == 11) || (g_deChSelStt == 14) || (g_deChSelStt == 17) || (g_deChSelStt == 20) || (g_deChSelStt == 23))
							g_deChSelStt++;
						break;
					default :
						break;
				}
			}
			else
			{
				g_deChSelStt = 0;
			}
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
		}

		g_deSw4ShortFlg = 0;
	}
}

void LCD_DetailsWirelessStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if((g_deMainModeLanNum & 2) == 2)	// if Include Client Mode, 2 Pages
			{
				if(g_deDetSelStt == 0)
					g_deDetSelStt = 9;
				else if(g_deDetSelStt == 8)
					g_deDetSelStt = 6;
				else if(g_deDetSelStt == 2)
					g_deDetSelStt = 0;
				else
					g_deDetSelStt --;
			}
			else								// if Exclude Client Mode, 1 Page
			{
				if(g_deDetSelStt == 0)
					g_deDetSelStt = 6;
				else if(g_deDetSelStt == 2)
					g_deDetSelStt = 0;
				else
					g_deDetSelStt --;
			}
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 2 :						// Selected ModIp
					if(g_arSetLanModIpNum[(g_deChSelStt-9)/4] < g_arSetAddrMax[g_deChSelStt])
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] += g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] = 255;
					ConvertNum2AscDec(g_arSetLanModIpNum[0], g_arSetLanModIpAsc, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[1], g_arSetLanModIpAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[2], g_arSetLanModIpAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[3], g_arSetLanModIpAsc+12, 3);
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if((g_deMainModeLanNum & 2) == 2)	// if Include Client Mode, 2 Pages
			{
				if(g_deDetSelStt == 9)
					g_deDetSelStt = 0;
				else if(g_deDetSelStt == 0)
					g_deDetSelStt = 2;
				else if(g_deDetSelStt == 6)
					g_deDetSelStt = 8;
				else
					g_deDetSelStt++;
			}
			else								// if Exclude Client Mode, 1 Page
			{
				if(g_deDetSelStt == 6)
					g_deDetSelStt = 0;
				else if(g_deDetSelStt == 0)
					g_deDetSelStt = 2;
				else
					g_deDetSelStt++;
			}
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 2 :						// Selected SensorIp
					if(g_arSetLanModIpNum[(g_deChSelStt-9)/4] > g_arSetAddrMin[g_deChSelStt])
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] -= g_arSetAddrSum[g_deChSelStt];
					else
						g_arSetLanModIpNum[(g_deChSelStt-9)/4] = 0;
					ConvertNum2AscDec(g_arSetLanModIpNum[0], g_arSetLanModIpAsc, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[1], g_arSetLanModIpAsc+4, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[2], g_arSetLanModIpAsc+8, 3);
					ConvertNum2AscDec(g_arSetLanModIpNum[3], g_arSetLanModIpAsc+12, 3);
					break;
				default :
					break;
			}

		}

		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			memcpy(&CPU_I2C.LanModIp, g_arSetLanModIpNum, 4);
			memcpy(&CPU_I2C.LanModSub, g_arSetLanModSubNum, 4);
			memcpy(&CPU_I2C.LanModGw, g_arSetLanModGwNum, 4);
			memcpy(&CPU_I2C.LanModPort, &g_deSetLanModPortNum, 2);
			memcpy(&CPU_I2C.LanModMac, g_arSetLanModMacNum, 6);
			memcpy(&CPU_I2C.LanDestIp, g_arSetLanDestIpNum, 4);
			memcpy(&CPU_I2C.LanDestPort, &g_deSetLanDestPortNum, 2);

			I2C1_WRITE();
			HAL_Delay(10);

			g_deBkgdStt = SETTING_STATE;

			NVIC_SystemReset();
		}
		else						// if Select Other Details
		{
			if(g_deChSelStt == 0)
			{
				g_deChSelStt = 9;
			}
			else if(g_deChSelStt < 25)
			{
			}
			else
			{
				g_deChSelStt = 0;
			}
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
		}

		g_deSw4ShortFlg = 0;
	}
}
void LCD_DetailsTemphumiStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if(g_deDetSelStt == 0)
				g_deDetSelStt = 4;
			else
				g_deDetSelStt --;
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Over Temperature Error
					if(g_deSetThOtWrnNum < 99)
						g_deSetThOtWrnNum++;
					else
						g_deSetThOtWrnNum = 99;
					ConvertNum2AscDec(g_deSetThOtWrnNum, g_arSetThOtWrnAsc, 2);
					break;
				case 2 :						// Selected Over Temperature Warning
					if(g_deSetThOtErrNum < 99)
						g_deSetThOtErrNum++;
					else
						g_deSetThOtErrNum = 99;
					ConvertNum2AscDec(g_deSetThOtErrNum, g_arSetThOtErrAsc, 2);
					break;
				case 3 :						// Selected Over Humidity Error
					if(g_deSetThOhErrNum < 99)
						g_deSetThOhErrNum++;
					else
						g_deSetThOhErrNum = 99;
					ConvertNum2AscDec(g_deSetThOhErrNum, g_arSetThOhErrAsc, 2);
					break;
				case 5 :
					if(g_deChSelStt < 18)
					{
						if(g_deSetThTempSlpNum < g_arSetSlpOffMax[g_deChSelStt])
							g_deSetThTempSlpNum += g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThTempSlpNum = 499;
						g_arSetThTempSlpAsc[0] = (g_deSetThTempSlpNum/100)%10+'0';
						g_arSetThTempSlpAsc[2] = (g_deSetThTempSlpNum/10)%10+'0';
						g_arSetThTempSlpAsc[3] = g_deSetThTempSlpNum%10+'0';
					}
					else
					{
						if(g_deSetThTempOffNum < g_arSetSlpOffMax[g_deChSelStt])
							g_deSetThTempOffNum += g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThTempOffNum = 4999;
						g_arSetThTempOffAsc[0] = ((g_deSetThTempOffNum < 0)?('-'):('+'));
						g_arSetThTempOffAsc[1] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/1000)%10+'0';
						g_arSetThTempOffAsc[2] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/100)%10+'0';
						g_arSetThTempOffAsc[4] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/10)%10+'0';
						g_arSetThTempOffAsc[5] = ((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))%10+'0';
					}
					break;
				case 6 :
					if(g_deChSelStt < 18)
					{
						if(g_deSetThHumiSlpNum < g_arSetSlpOffMax[g_deChSelStt])
							g_deSetThHumiSlpNum += g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThHumiSlpNum = 499;
						g_arSetThHumiSlpAsc[0] = (g_deSetThHumiSlpNum/100)%10+'0';
						g_arSetThHumiSlpAsc[2] = (g_deSetThHumiSlpNum/10)%10+'0';
						g_arSetThHumiSlpAsc[3] = g_deSetThHumiSlpNum%10+'0';
					}
					else
					{
						if(g_deSetThHumiOffNum < g_arSetSlpOffMax[g_deChSelStt])
							g_deSetThHumiOffNum += g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThHumiOffNum = 4999;
						g_arSetThHumiOffAsc[0] = ((g_deSetThHumiOffNum < 0)?('-'):('+'));
						g_arSetThHumiOffAsc[1] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/1000)%10+'0';
						g_arSetThHumiOffAsc[2] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/100)%10+'0';
						g_arSetThHumiOffAsc[4] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/10)%10+'0';
						g_arSetThHumiOffAsc[5] = ((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))%10+'0';
					}
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)
		{
			if(g_deDetSelStt == 4)
				g_deDetSelStt = 0;
			else
				g_deDetSelStt++;
		}
		else
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Over Temperature Error
					if(g_deSetThOtWrnNum > 0)
						g_deSetThOtWrnNum--;
					else
						g_deSetThOtWrnNum = 0;
					ConvertNum2AscDec(g_deSetThOtWrnNum, g_arSetThOtWrnAsc, 2);
					break;
				case 2 :						// Selected Over Temperature Warning
					if(g_deSetThOtErrNum > 0)
						g_deSetThOtErrNum--;
					else
						g_deSetThOtErrNum = 0;
					ConvertNum2AscDec(g_deSetThOtErrNum, g_arSetThOtErrAsc, 2);
					break;
				case 3 :						// Selected Over Humidity Error
					if(g_deSetThOhErrNum > 0)
						g_deSetThOhErrNum--;
					else
						g_deSetThOhErrNum = 0;
					ConvertNum2AscDec(g_deSetThOhErrNum, g_arSetThOhErrAsc, 2);
					break;
				case 5 :
					if(g_deChSelStt < 18)
					{
						if(g_deSetThTempSlpNum > g_arSetSlpOffMin[g_deChSelStt])
							g_deSetThTempSlpNum -= g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThTempSlpNum = 1;
						g_arSetThTempSlpAsc[0] = (g_deSetThTempSlpNum/100)%10+'0';
						g_arSetThTempSlpAsc[2] = (g_deSetThTempSlpNum/10)%10+'0';
						g_arSetThTempSlpAsc[3] = g_deSetThTempSlpNum%10+'0';
					}
					else
					{
						if(g_deSetThTempOffNum > g_arSetSlpOffMin[g_deChSelStt])
							g_deSetThTempOffNum -= g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThTempOffNum = -4999;
						g_arSetThTempOffAsc[0] = ((g_deSetThTempOffNum < 0)?('-'):('+'));
						g_arSetThTempOffAsc[1] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/1000)%10+'0';
						g_arSetThTempOffAsc[2] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/100)%10+'0';
						g_arSetThTempOffAsc[4] = (((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))/10)%10+'0';
						g_arSetThTempOffAsc[5] = ((g_deSetThTempOffNum < 0)?(g_deSetThTempOffNum*-1):(g_deSetThTempOffNum))%10+'0';
					}
					break;
				case 6 :
					if(g_deChSelStt < 18)
					{
						if(g_deSetThHumiSlpNum > g_arSetSlpOffMin[g_deChSelStt])
							g_deSetThHumiSlpNum -= g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThHumiSlpNum = 1;
						g_arSetThHumiSlpAsc[0] = (g_deSetThHumiSlpNum/100)%10+'0';
						g_arSetThHumiSlpAsc[2] = (g_deSetThHumiSlpNum/10)%10+'0';
						g_arSetThHumiSlpAsc[3] = g_deSetThHumiSlpNum%10+'0';
					}
					else
					{
						if(g_deSetThHumiOffNum > g_arSetSlpOffMin[g_deChSelStt])
							g_deSetThHumiOffNum -= g_arSetSlpOffSum[g_deChSelStt];
						else
							g_deSetThHumiOffNum = -4999;
						g_arSetThHumiOffAsc[0] = ((g_deSetThHumiOffNum < 0)?'-':'+');
						g_arSetThHumiOffAsc[1] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/1000)%10+'0';
						g_arSetThHumiOffAsc[2] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/100)%10+'0';
						g_arSetThHumiOffAsc[4] = (((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))/10)%10+'0';
						g_arSetThHumiOffAsc[5] = ((g_deSetThHumiOffNum < 0)?(g_deSetThHumiOffNum*-1):(g_deSetThHumiOffNum))%10+'0';
					}
					break;
				default :
					break;
			}

		}
		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			g_deMainThOtWrnNum = g_deSetThOtWrnNum;
			g_deMainThOtErrNum = g_deSetThOtErrNum;
			g_deMainThOhErrNum = g_deSetThOhErrNum;
			g_deMainThTempSlpNum = g_deSetThTempSlpNum;
			g_deMainThTempOffNum = g_deSetThTempOffNum;
			g_deMainThHumiSlpNum = g_deSetThHumiSlpNum;
			g_deMainThHumiOffNum = g_deSetThHumiOffNum;

			CPU_I2C.AlarmTempWrn = g_deMainThOtWrnNum;
			CPU_I2C.AlarmTempErr = g_deMainThOtErrNum;
			CPU_I2C.AlarmHumiErr = g_deMainThOhErrNum;
			CPU_I2C.TempSlope = g_deMainThTempSlpNum;
			CPU_I2C.TempOffset = g_deMainThTempOffNum;
			CPU_I2C.HumiSlope = g_deMainThHumiSlpNum;
			CPU_I2C.HumiOffset = g_deMainThHumiOffNum;

			I2C1_WRITE();
			HAL_Delay(10);

			g_deBkgdStt = SETTING_STATE;

			NVIC_SystemReset();
		}
		else if(g_deDetSelStt >= 4)	// if Select Other Details
		{
			if(g_deDetSelStt == 4)
			{
				g_deDetSelStt++;
				g_deChSelStt = 11;
			}
			else
			{
				g_deChSelStt++;
				if((g_deChSelStt == 12) || (g_deChSelStt == 21))
					g_deChSelStt++;
				else if(g_deChSelStt == 15)
					g_deChSelStt = 19;
				else if(g_deChSelStt == 24)
				{
					g_deDetSelStt++;
					g_deChSelStt = 11;
				}

				if(g_deDetSelStt == 7)
				{
					g_deDetSelStt = 4;
					g_deChSelStt = 0;
				}
			}
		}
		else
		{
			if(g_deChSelStt == 0)
				g_deChSelStt = 17;
			else
				g_deChSelStt = 0;
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
			if(g_deDetSelStt > 4)
				g_deDetSelStt = 4;
		}

		g_deSw4ShortFlg = 0;
	}
}
void LCD_DetailsSoundStatus(void)
{
	if(g_deSw1ShortFlg == 1)		// Change to Lower Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 0)	// if Detail(Type, Volume) is Unselected,
				g_deDetSelStt = 2;
			else
				g_deDetSelStt--;
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Alarm Type
					g_deSetSndTypNum = 1;
					strncpy((char*)g_arSetSndTypAsc, (char*)"Melody", 6);
					break;
				case 2 :						// Selected Volume
					if(g_deSetSndVlmNum < 3)
						g_deSetSndVlmNum++;
					else
						g_deSetSndVlmNum = 3;
					switch(g_deSetSndVlmNum)
					{
						case 1 :
							strncpy((char*)g_arSetSndVlmAsc, (char*)"Low ", 4);
							break;
						case 2 :
							strncpy((char*)g_arSetSndVlmAsc, (char*)"Mid ", 4);
							break;
						case 3 :
							strncpy((char*)g_arSetSndVlmAsc, (char*)"High", 4);
							break;
						default :
							break;
					}
					break;
				default :
					break;
			}
		}
		g_deSw1ShortFlg = 0;
	}

	if(g_deSw2ShortFlg == 1)		// Change to Upper Details List Selection
	{
		if(g_deChSelStt == 0)		// if Character is  Unselected,
		{
			if(g_deDetSelStt == 2)	// if Detail(Type, Volume) is Unselected,
				g_deDetSelStt = 0;
			else
				g_deDetSelStt++;
		}
		else						// if Character is Selected
		{
			switch(g_deDetSelStt)
			{
				case 1 :						// Selected Alarm Type
					g_deSetSndTypNum = 0;
					strncpy((char*)g_arSetSndTypAsc, (char*)"Beep  ", 6);
					break;
				case 2 :						// Selected Volume
					if(g_deSetSndVlmNum > 0)
						g_deSetSndVlmNum--;
					else
						g_deSetSndVlmNum = 0;
					switch(g_deSetSndVlmNum)
					{
						case 0:
							strncpy((char*)g_arSetSndVlmAsc, (char*)"Off ", 4);
							break;
						case 1 :
							strncpy((char*)g_arSetSndVlmAsc, (char*)"Low ", 4);
							break;
						case 2 :
							strncpy((char*)g_arSetSndVlmAsc, (char*)"Mid ", 4);
							break;
						default :
							break;
					}
					break;
				default :
					break;
			}
		}
		g_deSw2ShortFlg = 0;
	}

	if(g_deSw3ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deDetSelStt == 0)		// if Select 'SAVE' Details
		{
			// Save
			g_deMainSndTypNum = g_deSetSndTypNum;
			g_deMainSndVlmNum = g_deSetSndVlmNum;

			CPU_I2C.SndTyp = g_deMainSndTypNum;
			CPU_I2C.SndVlm = g_deMainSndVlmNum;

			if(g_deMainSndVlmNum == 0)
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
			else
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

			I2C1_WRITE();
			HAL_Delay(100);

			g_deBkgdStt = SETTING_STATE;
		}
		else						// if Select Other Details
		{
			if(g_deChSelStt == 0)
			{
				g_deChSelStt = 14;
			}
			else
			{
				g_deChSelStt = 0;
			}
		}
		g_deSw3ShortFlg = 0;
	}
	if(g_deSw4ShortFlg == 1)		// Change to Details List Select Screen
	{
		if(g_deChSelStt == 0)
		{
			g_deBkgdStt = SETTING_STATE;
		}
		else
		{
			g_deChSelStt = 0;
		}
		g_deSw4ShortFlg = 0;
	}
}
