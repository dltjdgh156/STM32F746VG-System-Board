//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME				:
//	PROJECT	NAME				:
//	FILE	NAME				:	icd.h
//	MAKE	BY					:
//	DESCRIPTION					:	interface control document structures
//	CREATED DATE				:	0000.00.00
//	----------------------------------------------------------------------------------------------------


#ifndef ICD_H_
#define ICD_H_
// Includes



// Defines



// Variables



// Structures
typedef struct
{
	float BatteryVolt;
	uint32_t ModuleUptime;
}
PM_MOD;
extern PM_MOD MOD_DATA;

typedef struct
{
	struct
	{
		uint32_t Pm003;
		uint32_t Pm005;
		uint32_t Pm010;
		uint32_t Pm025;
		uint32_t Pm050;
		uint32_t Pm100;
	}
	Cnt;
	uint32_t Alarm;
	uint8_t Status;
	uint8_t Version[13];
	uint16_t Serial[5];
}
PMtoCPU_t;
extern PMtoCPU_t PM_UART;

typedef struct
{
	float Temperature;
	float Humidity;
}
THtoCPU_t;
extern THtoCPU_t TH_I2C;

typedef struct
{
	// 1st 64 = (1+1)+(2+2+2)+(2+2+2+2+2+2)+(2)+(1+1+1+1+1+1+1+1)+(1+1)+(4+4+4+4+4+4+4+4)
	// 2 = 1+1
	uint8_t ResetFlag;				// [0](Parameter Reset Flag)
	uint8_t ClearFlag;				// [1](Time-log Clear Flag)
	// 6 = 2+2+2
	uint16_t AccDay;				// [2]~[3]
	uint16_t AccHour;				// [4]~[5]
	uint16_t AccMin;				// [6]~[7]
	// 12 = 2+2+2+2+2+2
	uint16_t Alarm003;				// [8]~[9]
	uint16_t Alarm005;				// [10]~[11]
	uint16_t Alarm010;				// [12]~[13]
	uint16_t Alarm025;				// [14]~[15]
	uint16_t Alarm050;				// [16]~[17]
	uint16_t Alarm100;				// [18]~[19]
	// 1
	int8_t LogCnt;					// [20]
	// 9 = 1+1+1+1+1+1+1+1+1
	uint8_t ModeAlarm;				// [21]
	uint8_t ModeIo;					// [22]
	uint8_t Mode485;				// [23]
	uint8_t ModeLan;				// [24]
	uint8_t ModeWlss;				// [25]
	uint8_t ModeTh;					// [26]
	uint8_t ModeSma;				// [27]
	uint8_t ModeDef;				// [28]
	uint8_t ModeRsvd;				// [29]
	// 2 = 1+1
	uint8_t SndTyp;					// [30]
	uint8_t SndVlm;					// [31]
	// 32 = 4+4+4+4+4+4+4+4
	uint32_t AlarmLifetime;			// [32]~[35]
	uint8_t AlarmTempWrn;			// [36]
	uint8_t AlarmTempErr;			// [37]
	uint8_t AlarmHumiErr;			// [38]
	uint8_t AlarmRsvd;				// [39]
	int16_t TempSlope;				// [40]~[41]
	int16_t TempOffset;				// [42]~[43]
	int16_t HumiSlope;				// [44]~[45]
	int16_t HumiOffset;				// [46]~[47]
	uint8_t reserved1[16];			// [48]~[63]

	// 2nd 64 = (4+1+1+1+1+1+1+1+1+2)+(4+4+4+2+6+4+2+1+1+2)+rsvd[20]
	// 14 = 4+1+1+1+1+1+1+1+1+2
	uint32_t Rs485Baud;				// [64]~[67]
	uint8_t Rs485Data;				// [68]
	uint8_t Rs485Prty;				// [69]
	uint8_t Rs485Stop;				// [70]
	uint8_t Rs485Flow;				// [71]
	uint8_t Rs485Mod;				// [72]
	uint8_t Rs485Dest;				// [73]
	uint8_t Rs485Ptcl;				// [74]
	uint8_t Rs485Dvcd;				// [75]
	uint16_t Rs485Dvld;				// [76]~[77]

	// 30 = 4+4+4+6+2+4+2+1+1+2
	uint8_t LanModIp[4];			// [78]~[89]
	uint8_t LanModSub[4];			// [82]~[85]
	uint8_t LanModGw[4];			// [86]~[89]
	uint16_t LanModPort;			// [90]~[91]
	uint8_t LanModMac[6];			// [92]~[97]
	uint8_t LanDestIp[4];			// [98]~[101]
	uint16_t LanDestPort;			// [102]~[103]
	uint8_t LanPtcl;				// [104]
	uint8_t LanDvcd;				// [105]
	uint16_t LanDvld;				// [106]~[107]
	// reserved
	uint8_t reserved2[20];			// [108]~[127]

	// 3th 64 = (16+16+4+4+4+2+6+4+2+1+1+2)+rsvd[2]
	// 62 = 16+16+4+4+4+2+6+4+2+1+1+2
	int8_t WifiSsid[16];			// [128]~[143]
	int8_t WifiPswd[16];			// [144]~[159]
	uint8_t WifiModIp[4];			// [160]~[163]
	uint8_t WifiModSub[4];			// [164]~[167]
	uint8_t WifiModGw[4];			// [168]~[171]
	uint16_t WifiModPort;			// [178]~[179]
	uint8_t WifiModMac[6];			// [172]~[177]
	uint8_t WifiDestIp[4];			// [180]~[183]
	uint16_t WifiDestPort;			// [184]~[185]
	uint8_t WifiPtcl;				// [186]
	uint8_t WifiDvcd;				// [187]
	uint16_t WifiDvld;				// [188]~[189]
	// reserved
	uint8_t reserved3[2];			// [190]~[191]

	// 4th 64 = 16+16+16+16
	uint8_t BleServUuid[16];		// [192]~[207]
	uint8_t BleCharUuid[16];		// [208]~[223]
	uint8_t BleSpareUuid[16];		// [224]~[191]
	uint8_t BlePincode[16];			// [240]~[255]

	// 5th~20th 64*16 = 64+64+64+64+64+64+64+64+64+64+64+64+64+64+64+64
	uint8_t LogData[128][8]; 		// [256]~[1279]
}
CPUtoEEPROM_t;
extern CPUtoEEPROM_t CPU_I2C;

typedef struct
{

	int8_t Pm003[8];
	int8_t			: 1;
	int8_t Pm005[8];
	int8_t			: 1;
	int8_t Pm010[8];
	int8_t			: 1;
	int8_t Pm025[8];
	int8_t			: 1;
	int8_t Pm050[8];
	int8_t			: 1;
	int8_t Pm100[8];
	int8_t			: 1;
}
ParticleCount_t;
extern ParticleCount_t PCnt;

typedef struct
{
	int8_t Alarm[8];
	uint8_t : 1;
}
ParticleAlram_t;
extern ParticleAlram_t PAlarm;

typedef struct
{
	int8_t Version[13];
	uint8_t : 1;
}
ParticleVersion_t;
extern ParticleVersion_t PVersion;

//typedef struct
//{
//	uint8_t YearNum		: 8;
//	uint8_t MonthNum	: 8;
//	uint8_t DateNum		: 8;
//	uint8_t WeekNum		: 8;
//	uint8_t HourNum		: 8;
//	uint8_t MinuteNum	: 8;
//	uint8_t SecondNum	: 8;
//	uint8_t TenMiliNum	: 8;
//}
//RealTimeData_t;
//extern RealTimeData_t DNT_RTC;

//typedef union
//{
//	uint32_t all	: 32;
//	struct
//	{
//		uint32_t Pm003StatWarn	: 1;	// if Particle   3um is Over Reference(0x00000001)(code:0-1)
//		uint32_t Pm005StatWarn	: 1;	// if Particle   5um is Over Reference(0x00000002)(code:2-3)
//		uint32_t Pm010StatWarn 	: 1;	// if Particle  10um is Over Reference(0x00000004)(code:4-5)
//		uint32_t Pm025StatWarn	: 1;	// if Particle  25um is Over Reference(0x00000008)(code:6-7)
//		uint32_t Pm050StatWarn	: 1;	// if Particle  50um is Over Reference(0x00000010)(code:8-9)
//		uint32_t Pm100StatWarn	: 1;	// if Particle 100um is Over Reference(0x00000020)(code:10-11)
//		uint32_t TempStatWarn	: 1;	// if Temperature is Over Limit(0x00000040)(code:12-13)
//		uint32_t UptimeStatWarn	: 1;	// if Uptime is Under Limit(0x00000080)(code:14-15)
//		uint32_t BatStatWarn 	: 1;	// if Battery Voltage is Low(0x00000100)(code:16-17)
//		uint32_t SdmmcStatWarn	: 1;	// if Sd-card is Ejected(0x00000200)(code:18-19)
//		uint32_t EthConnWarn	: 1;	// if it can not receive over  5 second(0x00000400, Ethernet)(code:20-21)
//		uint32_t Uart7ConnWarn	: 1;	// if it can not receive over  5 second(0x00000800, RS485)(code:22-23)
//		uint32_t reserved1		: 4;
//
//		uint32_t I2C1ConnErr	: 1;	// if if can not receive over  3 second(0x00010000, EEPROM)(code:32-33)
//		uint32_t SdmmcConnErr	: 1;	// if it can not receive over  3 second(0x00040000, SDCard)(code:34-35)
//		uint32_t Uart1ConnErr	: 1;	// if it can not receive over  3 second(0x00080000, WIFI)(code:36-37)
//		uint32_t Uart2ConnErr	: 1;	// if it can not receive over 10 second(0x00100000, PMSensor)(code:38-39)
//		uint32_t I2C4ConnErr	: 1;	// if if can not receive over  3 second(0x00020000, THSensor)(code:40-41)
//		uint32_t FanStatErr		: 1;	// if if can not receive over 10 second(0x00200000, FanOS|FanUS)(code:42-43)
//		uint32_t TempStatErr	: 1;	// if Temperature is Over Limit(0x00400000)(code:44-45)
//		uint32_t HumiStatErr	: 1;	// if Humidity is Over Limit(0x00800000)(code:46-47)
//		uint32_t reserved2		: 8;
//	}
//	bit;
//}
//BitResult_t;
//extern BitResult_t BResult;
//extern BitResult_t BResult_k;

typedef union
{
	uint16_t all : 16;
	struct
	{
		uint16_t BatteryUv			: 1;
		uint16_t UptimeOt			: 1;
		uint16_t LanDisconn			: 1;
		uint16_t Rs485Disconn		: 1;
		uint16_t Reserved			: 12;
	}
	bit;
}
BitStatus_t;
extern BitStatus_t BStatus;

typedef union
{
	uint16_t all : 16;
	struct
	{
		uint16_t Pm003OpAlm		: 1;
		uint16_t Pm005OpAlm		: 1;
		uint16_t Pm010OpAlm		: 1;
		uint16_t Pm025OpAlm		: 1;
		uint16_t Pm050OpAlm		: 1;
		uint16_t Pm100OpAlm		: 1;
		uint16_t Reserved			: 10;
	}
	bit;
}
BitAlarm_t;
extern BitAlarm_t BAlarm;
extern BitAlarm_t BAlarm_k;

typedef union
{
	uint16_t all : 16;
	struct
	{
		uint16_t FanAsErr			: 1;
		uint16_t EepromConnErr		: 1;
		uint16_t PmConnErr			: 1;
		uint16_t SdConnErr			: 1;
		uint16_t Reserved			: 12;
	}
	bit;
}
BitError_t;
extern BitError_t BError;
extern BitError_t BError_k;

typedef union
{
	uint16_t all : 16;
	struct
	{
		uint16_t In1AlarmEnable		: 1;
		uint16_t In2AlarmReset 		: 1;
		uint16_t In3Reserved		: 1;
		uint16_t In4Reserved		: 1;
		uint16_t Reserved1			: 4;
		uint16_t Out1Running		: 1;
		uint16_t Out2PmAlarm 		: 1;
		uint16_t Out3ModError		: 1;
		uint16_t Out4Reserved		: 1;
		uint16_t Reserved2			: 4;
	}
	bit;
}
BitDio_t;
extern BitDio_t BDio;

// Functions



#endif /* ICD_H_ */
