//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	eth.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef ETH_H_
#define ETH_H_
// Includes



// Defines
#define STX_HEAD							(0x53)

#define TYPE_PARTICLE						(0x02)

// DAQ_ICD01(����)
#define CMD_ID_MSG_CTRL_REP					(0x04)				// ������ ����
#define CMD_ID_MSG_CTRL_DATA				(0x05)				// ������ ��û ���� 
#define CMD_ID_MSG_CTRL_PARA				(0x06)				// ������ ����

#define ACK_CODE_1							(0x64)				// ��ƼŬ������� ������ ��û ����
#define ACK_CODE_2							(0x65)				// ��ƼŬ������� ���� ������ ����
#define ACK_CODE_3							(0x66)				// ������ ACK
#define ACK_CODE_4							(0x67)				// �̺�Ʈ�α� ACK
#define ACK_CODE_5							(0x68)				// �Ϸù�ȣ ACK
#define ACK_CODE_6							(0x69)				// �������� ACK


// DAQ_ICD01(���)
#define CMD_ID_MSG_DATA_REQ					(0x01)				// ������ ��û
#define CMD_ID_MSG_CONFIG_REQ				(0x02)				// ���� ��û
#define CMD_ID_MSG_SYS_CMD					(0x03)				// ������

#define CMD_CODE_1							(0x01)				// ��ƼŬ ������ ��û
#define CMD_CODE_2							(0x02)				// ���� ������ ��û
#define CMD_CODE_3							(0x03)				// ��� �����
#define CMD_CODE_4							(0x04)				// �̺�Ʈ �α� �ʱ�ȭ
#define CMD_CODE_5							(0x05)				// ��� ����
#define CMD_CODE_6							(0x06)				// �̹����� ���� ����
#define CMD_CODE_7							(0x07)				// �Ҹ� ����
#define CMD_CODE_8							(0x08)				// �����ð� ���� ����
#define CMD_CODE_9							(0x09)				// RS485 ��� ����
#define CMD_CODE_10							(0x0A)				// LAN ��� SERVER ����
#define CMD_CODE_11							(0x0B)				// LAN ��� MAC/CLIENT ����
#define CMD_CODE_12							(0x0C)				// WLAN ��� SSID ����
#define CMD_CODE_13							(0x0D)				// WLAN ��� PASS ����
#define CMD_CODE_14							(0x0E)				// WLAN ��� SERVER ����
#define CMD_CODE_15							(0x0F)				// WLAN ��� MAC/CLIENT ����
#define CMD_CODE_16							(0x10)				// BLE ��� SERVICE UUID ����
#define CMD_CODE_17							(0x11)				// BLE ��� CHAR. UUID ����
#define CMD_CODE_18							(0x12)				// BLE ��� SPARE UUID ����
#define CMD_CODE_19							(0x13)				// BLE ��� PIN CODE ����
#define CMD_CODE_20							(0x14)				// �½������� ���� ����
#define CMD_CODE_21							(0x15)				// �½��� ����ġ ����
#define CMD_CODE_22							(0x16)				// �̺�Ʈ �α� ��û
#define CMD_CODE_23							(0x17)				// �Ϸù�ȣ ��û
#define CMD_CODE_24							(0x18)				// �������� ��û
#define CMD_CODE_25							(0x19)				// �������� ��û

// üũ�� value
#define CHECK_SUM_ERROR						(0)
#define CHECK_SUM_OK						(1)


// Structures
/**************************************** DAQ_ICD01(����) ****************************************/


// ��ƼŬ������� ������ ��û ����

/* =======================  Status  ======================= */
/* =======================(Bit Field)====================== */
typedef union{
	uint16_t u2t_status;
	struct {
		uint8_t BatteryUv		:	1;		// ���� ���͸� ������
		uint8_t UptimeOt		:	1;		// �����ð� ���� �ʰ�
		uint8_t LanDisconn		:	1;		// LAN ��� ��������
		uint8_t Rs485Disconn	:	1;		// RS485 ��� ��������
		uint8_t Reserved1		:	1;		// 
		uint8_t Reserved2		:	1;		// 
		uint8_t Reserved3		:	1;		// 
		uint8_t Reserved4		:	1;		// 
		uint8_t Reserved5		:	1;		// 
		uint8_t Reserved6		:	1;		// 
		uint8_t Reserved7		:	1;		// 
		uint8_t Reserved8		:	1;		// 
		uint8_t Reserved9		:	1;		// 
		uint8_t Reserved10		:	1;		// 
		uint8_t Reserved11		:	1;		// 
		uint8_t Reserved12		:	1;		// 
	}Bit;
}bit_field_pm_status;


/* ========================= Alarm ======================== */
/* =======================(Bit Field)====================== */
typedef union{
	uint16_t u2t_alarm;
	struct {
		uint8_t Pm003OpAlm		:	1;		// 0.3um �˶����� �ʰ� ���
		uint8_t Pm005OpAlm		:	1;		// 0.5um �˶����� �ʰ� ���
		uint8_t Pm010OpAlm		:	1;		// 1.0um �˶����� �ʰ� ���
		uint8_t Pm025OpAlm		:	1;		// 2.5um �˶����� �ʰ� ���
		uint8_t Pm050OpAlm		:	1;		// 5.0um �˶����� �ʰ� ���
		uint8_t Pm100OpAlm		:	1;		// 10um �˶����� �ʰ� ���
		uint8_t Reserved1		:	1;		// 
		uint8_t Reserved2		:	1;		// 
		uint8_t Reserved3		:	1;		// 
		uint8_t Reserved4		:	1;		// 
		uint8_t Reserved5		:	1;		// 
		uint8_t Reserved6		:	1;		// 
		uint8_t Reserved7		:	1;		// 
		uint8_t Reserved8		:	1;		// 
		uint8_t Reserved9		:	1;		// 
		uint8_t Reserved10		:	1;		// 
	}Bit;
}bit_field_pm_alarm;

/* ======================    Error   ====================== */
/* =======================(Bit Field)====================== */
typedef union{
	uint16_t u2t_error;
	struct {
		uint8_t FanAsErr		:	1;		// FAN ����(Fanos + Fanus)
		uint8_t EepromConnErr	:	1;		// EEPROM ����(��� ������)
		uint8_t PmConnErr		:	1;		// PM Sensor ����(��� ������)
		uint8_t SdConnErr		:	1;		// SD Card ����(��� ������)
		uint8_t Reserved1		:	1;		// 
		uint8_t Reserved2		:	1;		// 
		uint8_t Reserved3		:	1;		// 
		uint8_t Reserved4		:	1;		// 
		uint8_t Reserved5		:	1;		// 
		uint8_t Reserved6		:	1;		// 
		uint8_t Reserved7		:	1;		// 
		uint8_t Reserved8		:	1;		// 
		uint8_t Reserved9		:	1;		// 
		uint8_t Reserved10		:	1;		// 
		uint8_t Reserved11		:	1;		// 
		uint8_t Reserved12		:	1;		// 
	}Bit;       
}bit_field_error;
                

/* ======================== PM_Dio ======================== */
/* =======================(Bit Field)====================== */
typedef union{
	uint16_t u2t_dio;
	struct {
		uint8_t In1AlarmEnable	:	1;		// ���� �Է�1 ����	//	�˶� ��� ����
		uint8_t In2AlarmReset	:	1;		// ���� �Է�2 ����	//	�˶� ����
		uint8_t In3Reserved		:	1;		// ���� �Է�3 ����	//	�̻��
		uint8_t In4Reserved		:	1;		// ���� �Է�4 ����	//	�̻��
		uint8_t Reserved1		:	1;		// 	
		uint8_t Reserved2		:	1;		// 	
		uint8_t Reserved3		:	1;		// 	
		uint8_t Reserved4		:	1;		// 	
		uint8_t Out1Running		:	1;		// ���� ���1 ����	//	������
		uint8_t Out1PmAlarm		:	1;		// ���� ���2 ����	//	�̹����� �����ʰ� �˶�
		uint8_t Out3ModError	:	1;		// ���� ���3 ����	//	��⵿�� ����
		uint8_t Out4Reserved	:	1;		// ���� ���4 ����	//	�̻��
		uint8_t Reserved5		:	1;		// 
		uint8_t Reserved6		:	1;		// 
		uint8_t Reserved7		:	1;		// 
		uint8_t Reserved8		:	1;		// 
	}Bit;       
}bit_field_dio;


// ũ�� : 58 Byte

/* ========= Date ========= */
/* [0]		Year(Lower)		*/
/* [1]		Month			*/
/* [2]		Date			*/
/* ======================== */

/* ========== Time ========  */
/* [0]		Hour			 */
/* [1]		Minute			 */
/* [2]		Second			 */
/* [3]		TenMiliSecond*10 */
/* ========================  */

/* ====================== Uptime ====================== */
/* [0]		Days			���� ���� �Ⱓ(��)			*/
/* [1]		Hours			���� �⵿ �ð�(�ð�)		*/
/* [2]		Minutes*10		���� �⵿ �ð�(��*10)		*/
/* [3]		Reserved									*/
/* ==================================================== */

/* ================== PM_Data ================= */
/* [0]	0.3um ��ƼŬ ����		����: Number	*/
/* [1]	0.5um ��ƼŬ ����		����: Number	*/
/* [2]	1.0um ��ƼŬ ����		����: Number	*/
/* [3]	2.5um ��ƼŬ ����		����: Number	*/
/* [4]	5.0um ��ƼŬ ����		����: Number	*/
/* [5]	10.0um ��ƼŬ ����		����: Number	*/
/* ============================================ */

typedef struct
{
	uint8_t					Id[1];
	uint8_t					Date[3];
	uint8_t					Time[4];
	uint16_t				Uptime[4];
	uint32_t				Pm[6];
	bit_field_pm_status		Status;
	bit_field_pm_alarm		Alarm;
	bit_field_error			Error;
	bit_field_dio			Dio;
	uint16_t				Reserved[5];
}ACK_DATA_REQ;


// ��ƼŬ������� ���� ������ ��û ����

// DAQ_ICD01(��������û_��)
typedef struct
{
	uint8_t		Mode_AlarmEnable;			// AlarmEnable
	uint8_t		Mode_IoEnable;				// ExtI/OEnable
	uint8_t		Mode_485Enable;				// RS485Enable
	uint8_t		Mode_LanEnable;				// NetworkEnable
	uint8_t		Mode_WlssEnable;			// WirelessEnable
	uint8_t		Mode_ThEnable;				// Temp,HumiEnable
	uint8_t		Mode_SmaEnable;				// SimpleMovingAverageSamples
	uint8_t		Mode_DefEnable;				// DefreezeEnable

	uint16_t	Pm_Alarm003Refer;			// OverPaticleMatterRef.of0.3um
	uint16_t	Pm_Alarm005Refer;			// OverPaticleMatterRef.of0.5um
	uint16_t	Pm_Alarm010Refer;			// OverPaticleMatterRef.of1.0um
	uint16_t	Pm_Alarm025Refer;			// OverPaticleMatterRef.of2.5um
	uint16_t	Pm_Alarm050Refer;			// OverPaticleMatterRef.of5.0um
	uint16_t	Pm_Alarm100Refer;			// OverPaticleMatterRef.of10um

	uint8_t		Sound_Type;					// TypeofSound
	uint8_t		Sound_Volume;				// VolumeofSound

	uint32_t	Dt_AlarmUptimeRefer;		// OverUptimeRef.ofModule

	uint32_t	Rs485_BaudRate;				// BaudRateofRS485
	uint8_t		Rs485_DataSize;				// DataSizeofRS485Packet
	uint8_t		Rs485_Paritybit;			// ParityBitsofRS485Packet
	uint8_t		Rs485_Stopbit;				// StopBitsofRS485Packet
	uint8_t		Rs485_Flowctrl;				// FlowControlofRS485Packet
	uint8_t		Rs485_Protocol;				// ProtocolofRS485
	uint8_t		Rs485_Devcode;				// DeviceCodeofRS485
	uint16_t	Rs485_Leaddev;				// LeadingDeviceofRS485
	uint8_t		Rs485_ModID;				// ModuleIDofRS485
	uint8_t		Rs485_DestID;				// DestinationIDofRS485

	uint8_t		Lan_ModIpAddr[4];			// ModuleIPAddressofLan
	uint8_t		Lan_ModSubnet[4];			// ModuleSubnetMaskofLan
	uint8_t		Lan_ModGateway[4];			// ModuleGatewayofLan
	uint16_t	Lan_ModPort;				// ModulePortNumber
	uint8_t		Lan_ModMacAddr[6];			// ModuleMACAddressofLan
	uint8_t		Lan_DestIpAddr[4];			// DestinationIPAddressofLan
	uint16_t	Lan_DestPort;				// DestinationPortNumber
	uint8_t		Lan_Protocol;				// ProtocolofLAN
	uint8_t		Lan_Devcode;				// DeviceCodeofLAN
	uint16_t	Lan_Leaddev;				// LeadingDeviceofLAN

	int8_t		Wifi_Ssid[16];				// ServiceSetIDofWlan
	int8_t		Wifi_Password[16];			// PasswordofWlan
	uint8_t		Wifi_ModIpAddr[4];			// ModuleIPAddressofWlan
	uint8_t		Wifi_ModSubnet[4];			// ModuleSubnetMaskofWlan
	uint8_t		Wifi_ModGateway[4];			// ModuleGatewayofWlan
	uint16_t	Wifi_ModPort;				// ModulePortNumber
	uint8_t		Wifi_ModMacAddr[6];			// ModuleMACAddressofWlan
	uint8_t		Wifi_DestIpAddr[4];			// DestinationIPAddressofWlan
	uint16_t	Wifi_DestPort;				// DestinationPortNumber
	uint8_t		Wifi_Protocol;				// ProtocolofLAN
	uint8_t		Wifi_Devcode;				// DeviceCodeofLAN
	uint16_t	Wifi_Leaddev;				// LeadingDeviceofLAN

	uint8_t		Ble_ServUuid[16];			// ServiceUni.UniqueIDofBLE
	uint8_t		Ble_CharUuid[16];			// CharacteristicUni.UniqueIDofBLE
	uint8_t		Ble_SpareUuid[16];			// SpareUni.UniqueIDofBLE
	int8_t		Ble_Pincode[16];			// PincodeofBLE

	uint8_t		Th_AlarmOverTempWarnRefer;	// OverTemperatureRef.(Warning)
	uint8_t		Th_AlarmOverTempErrRefer;	// OverTemperatureRef.(Error)
	uint8_t		Th_AlarmOverHumiErrRefer;	// OverHumidityRef.(Error)
	uint8_t		Th_Reserved;				// Reserved
	uint16_t	Th_TempSlope;				// ScaleofTemperature
	uint16_t	Th_TempOffset;				// OffsetofTemperature
	uint16_t	Th_HumiSlope;				// ScaleofHumidity
	uint16_t	Th_HumiOffset;				// OffsetofHumidity
}ACK_PARA_REQ;


// �Ϸù�ȣ ��û ����
typedef struct
{
	int8_t		Sn_Module[10];				// SerialNumberofModule
	uint16_t	Sn_Sensor[5];				// SerialNumberofPMSensor
}ACK_PARA_SERIAL_NUM_REQ;



// ���� ���� ��û ����
typedef struct
{
	int8_t		Ver_ModuleSW[10];			// VersionofModuleSoftware
	int8_t		Ver_SensorSW[10];			// VersionofPMSensorSoftware
}ACK_PARA_VERSION_REQ;




// 22. �̺�Ʈ�α� ACK
typedef struct
{
	uint8_t log_num[128][8];
}LOG_DATA_128EA;


// ������ ACK
typedef struct
{
	uint8_t u1t_response_code;		// [0]
	uint8_t none_data[7];			// [1] ~ [7]
}ACK_CMD;

/**************************************** DAQ_ICD01(���) ****************************************/

// �������ʵ忡 �����Ͱ� ���� ����ü ������ ����
// CMD 1 ~ 4 / 22 ~ 24
typedef struct
{
	uint8_t u1f_none_data[16];
}CMD_NONE_DATA;



// CMD.1 : ��ƼŬ ������ ��û - ��� ����ü ������ ����
// �ð� ���� ����
typedef struct
{
	uint16_t	u2t_year;		// [0] ~ [1]
	uint8_t 	u1t_month;		// [2]
	uint8_t 	u1t_day;		// [3]
	uint8_t 	u1t_hour;		// [4]
	uint8_t 	u1t_minute;		// [5]
	uint8_t 	ut_second;		// [6]
}CMD1_DATA;


// CMD.5 : ��� ���� - ��� ����ü ������ ����
/* Lan/Wireless ���� : 	0-Disable, 1-Enable								*/
/* Lan : 				0-Disable, 1-Server, 	2-Client, 	3-Dual		*/
/* Wireless : 			0-Disable, 1-WIFI, 		2-BLE, 		3-Dual		*/

typedef struct
{
	uint8_t Mode_AlarmEnable;		// [0]
	uint8_t Mode_IOEnable;			// [1]
	uint8_t Mode_485Enable;			// [2]
	uint8_t Mode_LanEnable;			// [3]
	uint8_t Mode_WlssEnable;		// [4]
	uint8_t Mode_ThEnable;			// [5]
	uint8_t Mode_SmaEnable;			// [6]
	uint8_t Mode_DefEnable;			// [7]
	uint8_t none_data[8];			// [8 ~ 15]
}CMD5_DATA;




// CMD.6 : �̹����� ���� ���� - ��� ����ü ������ ����
/* �� ũ�⺰ �̹����� �˶� ���� ����(�� 0~9999 ��)	*/

typedef struct
{
	uint16_t Pm_Alarm003Refer;	// [0]~[1] 
	uint16_t Pm_Alarm005Refer;	// [2]~[3] 
	uint16_t Pm_Alarm010Refer;	// [4]~[5] 
	uint16_t Pm_Alarm025Refer;	// [6]~[7] 
	uint16_t Pm_Alarm050Refer;	// [8]~[9] 
	uint16_t Pm_Alarm100Refer;	// [10]~[11]
	uint16_t none_data[2];		// [12]~[15]
}CMD6_DATA;




// CMD.7 : �Ҹ� ���� - ��� ����ü ������ ����
/* �� ũ�⺰ �̹����� �˶� ���� ����(�� 0~9999 ��)	*/
/*		Type : 0-Beep, 1-Melody		*/
/*		Volume : 0-Off, 1-Low, 2-Middle, 3-High		*/

typedef struct
{
	uint8_t Sound_Type;			// [0] 
	uint8_t Sound_Volume;		// [1]
	uint8_t non_data[14];	// [2] ~ [15]
}CMD7_DATA;




// CMD.8 : �����ð� ���� ���� - ��� ����ü ������ ����
/* �����ð� �˶� ���� ����(0~499999 �ð�)			*/

typedef struct
{
	uint32_t Dt_AlarmUptimeRefer;			// [0] ~ [3] 
	uint32_t non_data[3];					// [4] ~ [15]
}CMD8_DATA;





// CMD.9 : RS485 ��� ���� - ��� ����ü ������ ����
/* BuadRate : 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 �� �� 1	*/
/* DataSize : 7-7 bits, 8-8 bits															*/
/* Paritybit : 0-No Parity, 1-Odd Parity, 2-Even Parity										*/
/* Stopbit : 1-1-Stop Bit, 2-2-Stop Bits													*/
/* Flowctrl : 0-None, 1-Software(No Effect)													*/
/* Protocol : 0-ACT, 1-MC Format5															*/
/* Devcode : ����̽���������(0xa8-D, 0xb4-W)													*/
/* Leaddev : ���� ����̽� ��ġ(0~49999)															*/
/* Module : ����� ID 0~31																		*/
/* Destination : ��ǥ�� ID 0~31																*/

typedef struct
{
	uint32_t	Rs485_BaudRate;		// [0]~[3]	
	uint8_t		Rs485_DataSize; 	// [4]		
	uint8_t		Rs485_Paritybit;	// [5]		
	uint8_t		Rs485_Stopbit;  	// [6]		
	uint8_t		Rs485_Flowctrl; 	// [7]		
	uint8_t		Rs485_Protocol; 	// [8]		
	uint8_t		Rs485_Devcode;  	// [9]		
	uint16_t	Rs485_Leaddev;  	// [10]~[11]
	uint8_t		Rs485_ModID;    	// [12]		
	uint8_t		Rs485_DestID;   	// [13]		
	uint8_t		non_data[2];    	// [14]~[15]
}CMD9_DATA;





// CMD.10 : LAN ��� SERVER ���� - ��� ����ü ������ ����
/* ModIpAddr : ����� IP�ּ�(192.168.210.110)			*/
/* ModSubnet : ����� ����ݸ���ũ(255.255.255.0)		*/
/* ModGateway : ����� ����Ʈ����(192.168.210.1)		*/
/* ModPort : ����� ��Ʈ��ȣ(0~65535)					*/

typedef struct
{
	uint8_t Lan_ModIpAddr[4];		// [0]~[3] :
	uint8_t Lan_ModSubnet[4];		// [4]~[7] :
	uint8_t Lan_ModGateway[4];		// [8]~[11] 
	uint16_t Lan_ModPort;			// [12]~[13]
	uint8_t non_data[2];    		// [14]~[15]
}CMD10_DATA;





// CMD.11 : LAN ��� MAC/CLIENT ���� - ��� ����ü ������ ����
/* ModMacAddr : ����� MAC�ּ�(0.1.2.3.4.5)				*/
/* DestIpAddr : ���Ӵ���� IP�ּ�(192.168.210.11)		*/
/* DestPort : ���Ӵ���� ��Ʈ��ȣ(0~65535)				*/
/* Protocol : 0-ACT, 1-MC								*/
/* Devcode : ����̽���������(0xa8-D, 0xb4-W)			*/
/* Leaddev : ���� ����̽� ��ġ(0~49999)				*/

typedef struct
{
	uint8_t  Lan_ModMacAddr[6];			// [0]~[5]
	uint8_t  Lan_DestIpAddr[4];			// [6]~[9] 
	uint16_t Lan_DestPort;				// [10]~[11]
	uint8_t  Lan_Protocol;				// [12]
	uint8_t  Lan_Devcode;				// [13]
	uint16_t Lan_Leaddev;				// [14]~[15]
}CMD11_DATA;





// CMD.12 : WLAN ��� SSID ���� - ��� ����ü ������ ����
/*	WIFI�� Service Set ID, Ascii("TP-Link_D57A")		*/

typedef struct
{
	int8_t Wifi_Ssid[16]; // [0]~[15]
}CMD12_DATA;





// CMD.13 : WLAN ��� PASS ���� - ��� ����ü ������ ����
/*	WIFI�� ��й�ȣ, Ascii("38663130")				*/

typedef struct
{
	int8_t Wifi_Password[16]; // [0]~[15]
}CMD13_DATA;





// CMD.14 : WLAN ��� SERVER ���� - ��� ����ü ������ ����
/* ModIpAddr : ����� IP�ּ�(192.168.210.110)			*/
/* ModSubnet : ����� ����ݸ���ũ(255.255.255.0)		*/
/* ModGateway : ����� ����Ʈ����(192.168.210.1)		*/
/* ModPort : ����� ��Ʈ��ȣ(0~65535)					*/

typedef struct
{
	uint8_t		Wifi_ModIpAddr[4];		// [0]~[3]
	uint8_t		Wifi_ModSubnet[4];		// [4]~[7]
	uint8_t		Wifi_ModGateway[4];		// [8]~[11]
	uint16_t	Wifi_ModPort;			// [12]~[13]
	uint8_t		non_data[2];			// [14]~[15]
}CMD14_DATA;





// CMD.15 : WLAN ��� MAC/CLIENT ���� - ��� ����ü ������ ����
/* ModMacAddr : ����� MAC�ּ�(0.1.2.3.4.5)			*/
/* DestIpAddr : ���Ӵ���� IP�ּ�(192.168.210.11)	*/
/* DestPort : ���Ӵ���� ��Ʈ��ȣ(0~65535)			*/
/* Protocol : 0-ACT, 1-MC							*/
/* Devcode : ����̽���������(0xa8-D, 0xb4-W)		*/
/* Leaddev : ���� ����̽� ��ġ(0~49999)			*/

typedef struct
{
	uint8_t  Wifi_ModMacAddr[6]; 		// [0]~[5]
	uint8_t  Wifi_DestIpAddr[4];		// [6]~[9]
	uint16_t Wifi_DestPort;				// [10]~[11]
	uint8_t  Wifi_Protocol;				// [12]
	uint8_t  Wifi_Devcode; 				// [13]
	uint16_t Wifi_Leaddev;				// [14]~[15]
}CMD15_DATA;





// CMD.16 : BLE ��� SERVICE UUID ���� - ��� ����ü ������ ����
/* Bluetooth�� Service Uni. Unique ID				*/
/* (550e8400-e29b-41d4-a716-446655440000)			*/

typedef struct
{
	uint8_t  Ble_ServUuid[16]; 			// [0]~[15] 
}CMD16_DATA;





// CMD.17 : BLE ��� CHAR. UUID ���� - ��� ����ü ������ ����
/* Bluetooth�� Characteristic Uni. Unique ID		*/

typedef struct
{
	uint8_t  Ble_CharUuid[16]; 			// [0]~[15] 
}CMD17_DATA;





// CMD.18 : BLE ��� SPARE UUID ���� - ��� ����ü ������ ����
/* Bluetooth�� Spare Uni. Unique ID					*/

typedef struct
{
	uint8_t  Ble_SpareUuid[16]; 		// [0]~[15] 
}CMD18_DATA;





// CMD.19 : BLE ��� PIN CODE ���� - ��� ����ü ������ ����
/* Bluetooth�� Pin Code, Ascii("38663130")			*/

typedef struct
{
	uint8_t  Ble_Pincode[16]; 			// [0]~[15] 
}CMD19_DATA;





// CMD.20 : �½������� ���� ���� - ��� ����ü ������ ����
/* �µ� �� ���� �˶� ���� ����(�� 0~99 ��(�Ǵ� %))	*/

typedef struct
{
	uint8_t  Th_AlarmOverTempWarnRefer; 		// [0]
	uint8_t  Th_AlarmOverTempErrRefer; 			// [1]
	uint8_t  Th_AlarmOverHumiErrRefer; 			// [2]
	uint8_t  non_data[13]; 						// [3] ~ [15]
}CMD20_DATA;





// CMD.21 : �½��� ����ġ ���� - ��� ����ü ������ ����
/* Slope : �µ� �� ���� �� ��������ġ ����			*/
/* Offset : �µ� �� ���� �� ���꺸��ġ ����			*/

typedef struct
{
	uint16_t  Th_TempSlope; 		// [0] ~ [1]
	uint16_t  Th_TempOffset; 		// [2] ~ [3]
	uint16_t  Th_HumiSlope; 		// [4] ~ [5]
	uint16_t  Th_HumiOffset; 		// [6] ~ [7]
	uint16_t  non_data[4]; 			// [8] ~ [15]
}CMD21_DATA;



// Variables
extern uint8_t g_arEthSrvrRxBuf[64], g_arEthSrvrTxBuf[128];
extern uint8_t g_deEthSrvrConnFlg, g_deEthSrvrConnCnt;
extern uint8_t g_deEthClntConnFlg, g_deEthClntConnCnt;


extern uint8_t u1f_bootjump_flag;


// Functions
extern void ETH_INIT(void);
extern void ETH_CLNT_RECV(void);
extern void ETH_CLNT_TRANS(void);

#endif /* ETH_H_ */
