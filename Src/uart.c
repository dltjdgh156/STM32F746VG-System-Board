//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	uart.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"


// Defines



// Variables
int8_t g_deSmaSampleCnt = 0;	// Counter Value of Simple Moving Average Samples(0~63)
uint8_t g_deSmaSampleRef = 1;	// Number of Simple Moving Average Samples(1~64)
uint8_t g_arUart1RxBuf[64] = {0, }, g_arUart1TxBuf[64] = {'A', 'b', 'C', 'd', 'E', 'f', '9', '0'};
uint8_t g_arUart2RxBuf[64] = {0, }, g_arUart2TxBuf[8] = {0, };
uint8_t g_arUart6RxBuf[32] = {0, }, g_arUart6TxBuf[64];
uint8_t g_arUart7RxBuf[32] = {0, }, g_arUart7TxBuf[128];
uint8_t g_deUart1ConnFlg = 0, g_deUart2ConnFlg = 0, g_deUart6ConnFlg = 0, g_deUart7ConnFlg = 0;
uint8_t g_deUart1ConnCnt = 0, g_deUart2ConnCnt = 0, g_deUart6ConnCnt = 0, g_deUart7ConnCnt = 0;
uint8_t g_deUart2ConnErrCnt = 0, g_deUart2ConnErrRef = 0;
uint16_t g_deUart1RxCpltCnt = 0, g_deUart1TxCpltCnt = 0;
uint16_t g_deUart2RxCpltCnt = 0, g_deUart2TxCpltCnt = 0;
uint16_t g_deUart6DataCmdFlg = 0;
uint16_t g_deUart7DataCmdFlg = 0;
uint32_t g_arSmaParticle[64][6] = {0, };

// Structures



// Functions
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void UART_INIT(void);
void UART1_RECV(void);
void UART2_RECV(void);
void UART6_RECV(void);
void UART7_RECV(void);
void UART1_TRANS(void);
void UART2_TRANS(void);
void UART6_TRANS(void);
void UART7_TRANS(void);

uint16_t u1f_rx1_cnt = 0;
uint8_t u1f_rx1_byte;
uint8_t u1f_rx1_timeout = 0;
uint8_t u1f_rx1_data[200];
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	if(huart->Instance == USART1)	// 115200 baud
//	{
//		UART1_RECV();
//		HAL_UART_Receive_IT(&huart1, g_arUart1RxBuf, 1);
//	}
//	if(huart->Instance == USART1)
//	{
//		u1f_rx1_timeout = 0;
//
//		u1f_rx1_data[u1f_rx1_cnt] = u1f_rx1_byte;
//
//		u1f_rx1_cnt++;
//
//		HAL_UART_Receive_IT(&huart1, &u1f_rx1_byte, 1);
//	}
	if(huart->Instance == USART2)	// 9600 baud
	{
		UART2_RECV();
		HAL_UART_Receive_IT(&huart2, g_arUart2RxBuf, 1);
	}
	if(huart->Instance == UART7)	// 9600 baud
	{
		if(HAL_GPIO_ReadPin(RS485_DE_GPIO_Port, RS485_DE_Pin) == 0)
		{
			UART7_RECV();
		}
		HAL_UART_Receive_IT(&huart7, g_arUart7RxBuf, 1);
	}
}

uint8_t u1f_tx1_init_data[100];

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == UART7)	// 9600 baud
	{
		HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 0);		// DE Low
	}
}
void AT_command_create(char u1t_array[], uint8_t u1t_size, uint8_t u1t_rn_add)
{
    for (int i = 0; i < u1t_size; i++)
    {
    	u1f_tx1_init_data[i] = (uint8_t)u1t_array[i];
    	u1t_array[i] = 0x00;
    }

	if(u1t_rn_add == 1)
	{
    	u1f_tx1_init_data[u1t_size] = 0x0D;
	    u1f_tx1_init_data[u1t_size+1] = 0x0A;


	    HAL_UART_Transmit_IT(&huart1, u1f_tx1_init_data, u1t_size+2);
	}
	else
	{
		HAL_UART_Transmit_IT(&huart1, u1f_tx1_init_data, u1t_size);
	}
}
void AT_period_send_data(uint8_t u1t_flag)
{
	char tmp[100] = {0};

	if(u1t_flag == 1)
	{
		strcpy(tmp, "Repeat transfer data");
		AT_command_create(tmp, strlen(tmp), 0);
	}
}

void UART_INIT(void)
{
	// Setup for RS485(Uart7)
	huart7.Init.BaudRate = g_deMain485BaudNum;
	huart7.Init.WordLength = ((g_deMain485DataNum+(!!g_deMain485PrtyNum)) == 8)?UART_WORDLENGTH_8B:(((g_deMain485DataNum+(!!g_deMain485PrtyNum)) == 7)?UART_WORDLENGTH_7B:UART_WORDLENGTH_9B);	// 7-bit ; (1<<28), 8-bit ; 0, 9-bit ; ?
	huart7.Init.StopBits = ((g_deMain485StopNum ==1)?UART_STOPBITS_1:UART_STOPBITS_2);	// 1-bit ; 0x0000, 2-bit ; 0x2000
	huart7.Init.Parity = ((g_deMain485PrtyNum == 0)?UART_PARITY_NONE:((g_deMain485PrtyNum == 1)?UART_PARITY_ODD:UART_PARITY_EVEN));	// none ; 0x0000, even ; 0x0400, odd ;0x0600

	if (HAL_RS485Ex_Init(&huart7, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
		Error_Handler();

	// Receive Interrupt Enable
//	HAL_UART_Receive_IT(&huart1, &u1f_rx1_byte, 1);
	HAL_UART_Receive_IT(&huart2, g_arUart2RxBuf, 1);
	HAL_UART_Receive_IT(&huart7, g_arUart7RxBuf, 1);

	// UART1_INIT
//	char tmp[100] = {0};


	// 전송 모드 탈출 명령
//		strcpy(tmp, "+++");
//		AT_command_create(tmp, strlen(tmp), 0);
//		HAL_Delay(500);
//	// 리셋 명령
//		strcpy(tmp, "AT+RST");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);
//	// 리셋 명령
//		strcpy(tmp, "AT+RST");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);
//	// CWMODE = 3 -> AP + STA 둘다 사용
//		strcpy(tmp, "AT+CWMODE=3");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(3000);
//		strcpy(tmp, "AT+CWJAP=\"TP-Link_D57A\",\"38663130\"");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(3000);
//	// 딜레이 용도
//		strcpy(tmp, "AT");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);
//	// 딜레이 용도
//		strcpy(tmp, "AT");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);
//	// 현재 IP + MAC 체크용 (STA 사용 / AP 사용 X)
//		strcpy(tmp, "AT+CIFSR");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);
//	// SERVER IP + PORT 에 접속 시도
//	strcpy(tmp, "AT+CIPSTART=\"TCP\",\"192.168.0.119\",49152");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);
//	// 전송 모드로 진입
//		strcpy(tmp, "AT+CIPMODE=1");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);
//	// 전송 시작 명령
//		strcpy(tmp, "AT+CIPSEND");
//		AT_command_create(tmp, strlen(tmp),1);
//		HAL_Delay(500);

	// UART2_INIT
	// Open Particle Measurement
	g_arUart2TxBuf[0] = 0x11;
	g_arUart2TxBuf[1] = 0x03;
	g_arUart2TxBuf[2] = 0x0c;
	g_arUart2TxBuf[3] = 0x02;
	g_arUart2TxBuf[4] = 0x1e;
	g_arUart2TxBuf[5] = 0xc0;
	HAL_UART_Transmit(&huart2, g_arUart2TxBuf, 6, 10);
	g_deUart2TxCpltCnt++;
	HAL_Delay(50);

	// Read Software Version Number
	g_arUart2TxBuf[0] = 0x11;
	g_arUart2TxBuf[1] = 0x01;
	g_arUart2TxBuf[2] = 0x1e;
	g_arUart2TxBuf[3] = 0xd0;
	HAL_UART_Transmit(&huart2, g_arUart2TxBuf, 4, 10);
	g_deUart2TxCpltCnt++;
	HAL_Delay(50);

	// Read Serial Number
	g_arUart2TxBuf[0] = 0x11;
	g_arUart2TxBuf[1] = 0x01;
	g_arUart2TxBuf[2] = 0x1f;
	g_arUart2TxBuf[3] = 0xcf;
	HAL_UART_Transmit(&huart2, g_arUart2TxBuf, 4, 10);
	g_deUart2TxCpltCnt++;
	HAL_Delay(50);

	// Read Particle Measurement Result(Only Check Operating Error or Not)
	g_arUart2TxBuf[0] = 0x11;
	g_arUart2TxBuf[1] = 0x02;
	g_arUart2TxBuf[2] = 0x0b;
	g_arUart2TxBuf[3] = 0x07;
	g_arUart2TxBuf[4] = 0xdb;
	HAL_UART_Transmit(&huart2, g_arUart2TxBuf, 5, 10);
	g_deUart2TxCpltCnt++;

	HAL_Delay(100);

	// UART7_INIT

}

void UART1_RECV(void)
{
	if(0)	// if Received Packet is Error
	{

	}
	else	// if Received Packet is Valid
	{

	}
	g_deUart1ConnFlg = 1;
	g_deUart1ConnCnt = 0;
}

void UART1_TRANS(void)
{


}

void UART2_RECV(void)
{
	uint8_t l_deTemp = 0;
	uint32_t l_dePm003Sum = 0, l_dePm005Sum = 0, l_dePm010Sum = 0, l_dePm025Sum = 0, l_dePm050Sum = 0, l_dePm100Sum = 0;
	static uint8_t l_deUart2PacketCnt = 0;
	static uint16_t l_deChksum = 0;

	if(0)	// if Received Packet is Error
	{

	}
	else	// if Received Packet is Valid
	{
		l_deTemp = g_arUart2RxBuf[0];

		if(l_deUart2PacketCnt == 0)						// if Only Uart2 is Ready
		{
			if(l_deTemp == 0x16)							// if Received Data is Stx
			{
				g_arUart2RxBuf[1] = 0;						// Clear Buffer for Length
				g_arUart2RxBuf[2] = 0;						// Clear Buffer for Command
				l_deUart2PacketCnt = 1;
				l_deChksum = l_deTemp;
			}
		}
		else														// if Uart2 is Running
		{
			if(l_deUart2PacketCnt < 57)							// if Only Counter is Not Over Limit(Maximum Packet)
			{
				if(l_deUart2PacketCnt == g_arUart2RxBuf[1]+2)		// if Received Data is Check Sum(Length No.+Stx+Checksum)
				{
					g_arUart2RxBuf[l_deUart2PacketCnt] = l_deTemp;
					if(l_deTemp == 256-(l_deChksum & 0xff))		// if 'Check Sum' and Summation are Matched
					{
						switch(g_arUart2RxBuf[2])
						{
							case 0x0b :						// Read Particle Measurement Result
								g_deSmaSampleCnt++;
								if (g_deSmaSampleCnt == 64)
									g_deSmaSampleCnt = 0;

								// phase 1 - gathering data
								l_dePm003Sum = 0;
								l_dePm005Sum = 0;
								l_dePm010Sum = 0;
								l_dePm025Sum = 0;
								l_dePm050Sum = 0;
								l_dePm100Sum = 0;

								memcpy(&g_arSmaParticle[g_deSmaSampleCnt], g_arUart2RxBuf+27, 24);	// Simple Moving Average Sample is 1 (Temporary Fixed)

								g_arSmaParticle[g_deSmaSampleCnt][0] = SWAPBYTE32(g_arSmaParticle[g_deSmaSampleCnt][0]);
								g_arSmaParticle[g_deSmaSampleCnt][1] = SWAPBYTE32(g_arSmaParticle[g_deSmaSampleCnt][1]);
								g_arSmaParticle[g_deSmaSampleCnt][2] = SWAPBYTE32(g_arSmaParticle[g_deSmaSampleCnt][2]);
								g_arSmaParticle[g_deSmaSampleCnt][3] = SWAPBYTE32(g_arSmaParticle[g_deSmaSampleCnt][3]);
								g_arSmaParticle[g_deSmaSampleCnt][4] = SWAPBYTE32(g_arSmaParticle[g_deSmaSampleCnt][4]);
								g_arSmaParticle[g_deSmaSampleCnt][5] = SWAPBYTE32(g_arSmaParticle[g_deSmaSampleCnt][5]);

								// phase 2 - Average Data
								if(g_deMainModeSmaNum == 1)
								{
									PM_UART.Cnt.Pm003 = g_arSmaParticle[g_deSmaSampleCnt][0];
									PM_UART.Cnt.Pm005 = g_arSmaParticle[g_deSmaSampleCnt][1];
									PM_UART.Cnt.Pm010 = g_arSmaParticle[g_deSmaSampleCnt][2];
									PM_UART.Cnt.Pm025 = g_arSmaParticle[g_deSmaSampleCnt][3];
									PM_UART.Cnt.Pm050 = g_arSmaParticle[g_deSmaSampleCnt][4];
									PM_UART.Cnt.Pm100 = g_arSmaParticle[g_deSmaSampleCnt][5];
								}
								else	// Reference is all range except 1
								{
									for(int Cnt = 0; Cnt < g_deMainModeSmaNum; Cnt++)
									{
										l_dePm003Sum += g_arSmaParticle[(g_deSmaSampleCnt-Cnt) & 63][0];
										l_dePm005Sum += g_arSmaParticle[(g_deSmaSampleCnt-Cnt) & 63][1];
										l_dePm010Sum += g_arSmaParticle[(g_deSmaSampleCnt-Cnt) & 63][2];
										l_dePm025Sum += g_arSmaParticle[(g_deSmaSampleCnt-Cnt) & 63][3];
										l_dePm050Sum += g_arSmaParticle[(g_deSmaSampleCnt-Cnt) & 63][4];
										l_dePm100Sum += g_arSmaParticle[(g_deSmaSampleCnt-Cnt) & 63][5];
									}
									switch(g_deMainModeSmaNum)
									{
										case 0 :
											break;
										case 2 :
											PM_UART.Cnt.Pm003 = l_dePm003Sum >> 1;
											PM_UART.Cnt.Pm005 = l_dePm005Sum >> 1;
											PM_UART.Cnt.Pm010 = l_dePm010Sum >> 1;
											PM_UART.Cnt.Pm025 = l_dePm025Sum >> 1;
											PM_UART.Cnt.Pm050 = l_dePm050Sum >> 1;
											PM_UART.Cnt.Pm100 = l_dePm100Sum >> 1;
											break;
										case 4 :
											PM_UART.Cnt.Pm003 = l_dePm003Sum >> 2;
											PM_UART.Cnt.Pm005 = l_dePm005Sum >> 2;
											PM_UART.Cnt.Pm010 = l_dePm010Sum >> 2;
											PM_UART.Cnt.Pm025 = l_dePm025Sum >> 2;
											PM_UART.Cnt.Pm050 = l_dePm050Sum >> 2;
											PM_UART.Cnt.Pm100 = l_dePm100Sum >> 2;
											break;
										case 8 :
											PM_UART.Cnt.Pm003 = l_dePm003Sum >> 3;
											PM_UART.Cnt.Pm005 = l_dePm005Sum >> 3;
											PM_UART.Cnt.Pm010 = l_dePm010Sum >> 3;
											PM_UART.Cnt.Pm025 = l_dePm025Sum >> 3;
											PM_UART.Cnt.Pm050 = l_dePm050Sum >> 3;
											PM_UART.Cnt.Pm100 = l_dePm100Sum >> 3;
											break;
										case 16 :
											PM_UART.Cnt.Pm003 = l_dePm003Sum >> 4;
											PM_UART.Cnt.Pm005 = l_dePm005Sum >> 4;
											PM_UART.Cnt.Pm010 = l_dePm010Sum >> 4;
											PM_UART.Cnt.Pm025 = l_dePm025Sum >> 4;
											PM_UART.Cnt.Pm050 = l_dePm050Sum >> 4;
											PM_UART.Cnt.Pm100 = l_dePm100Sum >> 4;
											break;
										case 32 :
											PM_UART.Cnt.Pm003 = l_dePm003Sum >> 5;
											PM_UART.Cnt.Pm005 = l_dePm005Sum >> 5;
											PM_UART.Cnt.Pm010 = l_dePm010Sum >> 5;
											PM_UART.Cnt.Pm025 = l_dePm025Sum >> 5;
											PM_UART.Cnt.Pm050 = l_dePm050Sum >> 5;
											PM_UART.Cnt.Pm100 = l_dePm100Sum >> 5;
											break;
										default :
											PM_UART.Cnt.Pm003 = l_dePm003Sum/g_deSmaSampleRef;
											PM_UART.Cnt.Pm005 = l_dePm005Sum/g_deSmaSampleRef;
											PM_UART.Cnt.Pm010 = l_dePm010Sum/g_deSmaSampleRef;
											PM_UART.Cnt.Pm025 = l_dePm025Sum/g_deSmaSampleRef;
											PM_UART.Cnt.Pm050 = l_dePm050Sum/g_deSmaSampleRef;
											PM_UART.Cnt.Pm100 = l_dePm100Sum/g_deSmaSampleRef;
											break;
									}
								}

								// phase 3 - convert to ascii
								ConvertNum2AscDec(PM_UART.Cnt.Pm003, PCnt.Pm003, 8);
								ConvertNum2AscDec(PM_UART.Cnt.Pm005, PCnt.Pm005, 8);
								ConvertNum2AscDec(PM_UART.Cnt.Pm010, PCnt.Pm010, 8);
								ConvertNum2AscDec(PM_UART.Cnt.Pm025, PCnt.Pm025, 8);
								ConvertNum2AscDec(PM_UART.Cnt.Pm050, PCnt.Pm050, 8);
								ConvertNum2AscDec(PM_UART.Cnt.Pm100, PCnt.Pm100, 8);

								// phase 4 - checking alarm data, convert to ascii
								memcpy(&PM_UART.Alarm, g_arUart2RxBuf+51, sizeof(PM_UART.Alarm));
								PM_UART.Alarm &= 3;													// Remove the Garbage Value Except Alarm Bits
								ConvertNum2AscDec(PM_UART.Alarm, PAlarm.Alarm, 8);

								// phase 5 - limitation of ascii(exceed 9999)
								(PCnt.Pm003[3] == ' ')?(memcpy(g_arMainUppDatAsc[1], PCnt.Pm003+4, 4)):(memset(g_arMainUppDatAsc[1], '9', 4));
								(PCnt.Pm005[3] == ' ')?(memcpy(g_arMainUppDatAsc[2], PCnt.Pm005+4, 4)):(memset(g_arMainUppDatAsc[2], '9', 4));
								(PCnt.Pm010[3] == ' ')?(memcpy(g_arMainUppDatAsc[3], PCnt.Pm010+4, 4)):(memset(g_arMainUppDatAsc[3], '9', 4));
								(PCnt.Pm025[3] == ' ')?(memcpy(g_arMainUppDatAsc[4], PCnt.Pm025+4, 4)):(memset(g_arMainUppDatAsc[4], '9', 4));
								(PCnt.Pm050[3] == ' ')?(memcpy(g_arMainUppDatAsc[5], PCnt.Pm050+4, 4)):(memset(g_arMainUppDatAsc[5], '9', 4));
								(PCnt.Pm100[3] == ' ')?(memcpy(g_arMainUppDatAsc[6], PCnt.Pm100+4, 4)):(memset(g_arMainUppDatAsc[6], '9', 4));
								break;
							case 0x0c :						// Open/Close Particle Measurement
								PM_UART.Status = g_arUart2RxBuf[3];
								break;
							case 0x1e :						// Read Software Version
								PM_UART.Version[0] = g_arUart2RxBuf[3];
								PM_UART.Version[1] = g_arUart2RxBuf[4];
								PM_UART.Version[2] = g_arUart2RxBuf[5];
								PM_UART.Version[3] = g_arUart2RxBuf[6];
								PM_UART.Version[4] = g_arUart2RxBuf[7];
								PM_UART.Version[5] = g_arUart2RxBuf[8];
								PM_UART.Version[6] = g_arUart2RxBuf[9];
								PM_UART.Version[7] = g_arUart2RxBuf[10];
								PM_UART.Version[8] = g_arUart2RxBuf[11];
								PM_UART.Version[9] = g_arUart2RxBuf[12];
								PM_UART.Version[10] = g_arUart2RxBuf[13];
								PM_UART.Version[11] = g_arUart2RxBuf[14];
								PM_UART.Version[12] = g_arUart2RxBuf[15];

								memcpy(PVersion.Version, PM_UART.Version+3, 10);
								break;
							case 0x1f :						// Read Serial Number
								PM_UART.Serial[0] = (((uint16_t)g_arUart2RxBuf[3]) << 8) | (((uint16_t)g_arUart2RxBuf[4]) & 0xff);
								PM_UART.Serial[1] = (((uint16_t)g_arUart2RxBuf[5]) << 8) | (((uint16_t)g_arUart2RxBuf[6]) & 0xff);
								PM_UART.Serial[2] = (((uint16_t)g_arUart2RxBuf[7]) << 8) | (((uint16_t)g_arUart2RxBuf[8]) & 0xff);
								PM_UART.Serial[3] = (((uint16_t)g_arUart2RxBuf[9]) << 8) | (((uint16_t)g_arUart2RxBuf[10]) & 0xff);
								PM_UART.Serial[4] = (((uint16_t)g_arUart2RxBuf[11]) << 8) | (((uint16_t)g_arUart2RxBuf[12]) & 0xff);
								break;
							default :
								break;
						}
						// if Use De-freeze Mode, Checking Same Value for 5 Consecutive Times or Not(Current to 4 times ago) & More than Twice Before(5 times ago)
						if(g_deMainModeDefNum == 1)
						{
							if(g_arSmaParticle[g_deSmaSampleCnt][1] == g_arSmaParticle[(g_deSmaSampleCnt-1) & 63][1])
							{
								if(g_arSmaParticle[(g_deSmaSampleCnt-1) & 63][1] == g_arSmaParticle[(g_deSmaSampleCnt-2) & 63][1])
								{
									if(g_arSmaParticle[(g_deSmaSampleCnt-2) & 63][1] == g_arSmaParticle[(g_deSmaSampleCnt-3) & 63][1])
									{
										if(g_arSmaParticle[(g_deSmaSampleCnt-3) & 63][1] == g_arSmaParticle[(g_deSmaSampleCnt-4) & 63][1])
										{
											if(g_arSmaParticle[g_deSmaSampleCnt][1] >= ((g_arSmaParticle[(g_deSmaSampleCnt-5) & 63][1]?g_arSmaParticle[(g_deSmaSampleCnt-5) & 63][1]:1)*2))
												g_deMainModeDefFlg = 1;
										}
									}
								}
							}
							else if(g_arSmaParticle[g_deSmaSampleCnt][1] < g_arSmaParticle[(g_deSmaSampleCnt-1) & 63][1])
								g_deMainModeDefFlg = 0;
						}
						else
							g_deMainModeDefFlg = 0;
					}

					if(g_deUart2RxCpltCnt == 65535)
						g_deUart2RxCpltCnt = 0;
					else
						g_deUart2RxCpltCnt++;

					l_deUart2PacketCnt = 0;
					l_deChksum = 0;
				}
				else										// if Received Data is 'Length' or 'Command' or 'Data_n'
				{
					g_arUart2RxBuf[l_deUart2PacketCnt] = l_deTemp;
					l_deUart2PacketCnt++;
					l_deChksum += l_deTemp;
				}
			}
			else
			{
				l_deUart2PacketCnt = 0;
			}
		}
	}

	g_deUart2ConnFlg = 1;
	g_deUart2ConnCnt = 0;
}

void UART2_TRANS(void)
{
	// Read Particle Measurement Result(Moving Average, m = 0 or 15 or 30 or 45 or 60 samples)
	HAL_UART_Transmit(&huart2, g_arUart2TxBuf, 5, 10);
	if(g_deUart2TxCpltCnt == 65535)
	{
		g_deUart2TxCpltCnt = 0;
	}
	else
	{
		g_deUart2TxCpltCnt++;
	}
}

void UART6_RECV(void)
{
//	uint8_t l_deTemp = 0;
//	static uint16_t l_deUart6PacketCnt = 0, l_deUart6PacketSize = 0, l_deChksumUpper = 0, l_deChksumLower = 0;
//	static uint16_t l_deChksum = 0;
//
//	if(0)	// if Received Packet is Error
//	{
//
//	}
//	else	// if Received Packet is Valid
//	{
//		l_deTemp = g_arUart6RxBuf[0];
//
//		if(l_deUart6PacketCnt == 0)						// if Only Uart6 is Ready
//		{
//			if(l_deTemp == 0x53)							// if Received Data is Stx
//			{
//				l_deUart6PacketCnt = 1;
//				l_deChksum = 0;
//				g_deUart6DataCmdFlg = 0;
//			}
//			else
//			{
//				l_deUart6PacketCnt = 0;
//			}
//		}
//		else if(l_deUart6PacketCnt < 6)
//		{
//			g_arUart6RxBuf[l_deUart6PacketCnt] = l_deTemp;
//			l_deUart6PacketCnt++;
//
//			l_deChksum += l_deTemp;
//		}
//		else if(l_deUart6PacketCnt == 6)
//		{
//			g_arUart6RxBuf[l_deUart6PacketCnt] = l_deTemp;
//			l_deUart6PacketCnt++;
//
//			l_deUart6PacketSize = ((g_arUart6RxBuf[2] & 0xff) | ((uint16_t)g_arUart6RxBuf[1] << 8));
//
//			l_deChksum += l_deTemp;
//		}
//		else														// if Uart6 is Running
//		{
//			if(l_deUart6PacketCnt < 30)							// if Only Counter is Not Over Limit(Maximum Packet)
//			{
//				g_arUart6RxBuf[l_deUart6PacketCnt] = l_deTemp;
//
//				if(l_deUart6PacketCnt > l_deUart6PacketSize+2)		// if Received All Data(Before Checksum)
//				{
//					if(l_deUart6PacketCnt == l_deUart6PacketSize+5)
//					{
//						if(l_deChksum == ((g_arUart6RxBuf[l_deUart6PacketCnt-1] & 0xff)	| ((uint16_t)g_arUart6RxBuf[l_deUart6PacketCnt-2] << 8)))		// if 'Check Sum' and Summation are Matched
//						{
//							g_deUart6ConnFlg = 1;
//							g_deUart6ConnCnt = 0;
//							g_deUart6DataCmdFlg = 0x001b;
//						}
//						l_deUart6PacketCnt = 0;
//						l_deChksum = 0;
//					}
//					else
//					{
//						l_deUart6PacketCnt++;
//					}
//				}
//				else										// [7]~[22]
//				{
//					l_deChksum += l_deTemp;
//					l_deUart6PacketCnt++;
//				}
//
//
//			}
//			else
//			{
//				l_deUart6PacketCnt = 0;
//				g_deUart6DataCmdFlg = 0;
//			}
//		}
//	}
}

void UART6_TRANS(void)
{
//	uint8_t l_deDleCnt = 0, l_deTemp2 = 0, l_deChksumCnt = 0, l_deSort = 0, l_deChksumUpper = 0, l_deChksumLower = 0;
//	uint16_t l_deChksum = 0;
//	static uint8_t l_deUart6Heartbit = 0, l_deDataCnt = 0;
//	uint16_t temp = 0, humi = 0;//temporary
//
//	l_deUart6Heartbit ^= 1;
//
//	if(g_deUart6DataCmdFlg == 0x001b)
//	{
//		g_arUart6TxBuf[0] = 'S';											// Start of Transmit
//		g_arUart6TxBuf[1] = 0x00;											// Length, Upper
//		g_arUart6TxBuf[2] = 0x2c;											// Length, Lower
//		g_arUart6TxBuf[3] = 0x02;											// Type : 0x02(PM Sensor Module)
//		g_arUart6TxBuf[4] = 0x00;											// Command, Upper
//		g_arUart6TxBuf[5] = 0x1b;											// Command, Lower
//		g_arUart6TxBuf[6] = l_deDataCnt;									// Counter : 0~255
//
//		temp = (uint16_t)(TH_I2C.Temperature*100);
//		humi = (uint16_t)(TH_I2C.Humidity*100);
//
//		memcpy((void*)(g_arUart6TxBuf+7), (void*)(&DNT_RTC), sizeof(DNT_RTC));								// [ 7]~[14] ; 8 Byte
//		memcpy((void*)(g_arUart6TxBuf+15), (void*)(&PM_UART.Cnt), sizeof(PM_UART.Cnt));						// [15]~[38] ; 24 Byte
//		memcpy((void*)(g_arUart6TxBuf+39), (void*)(&PM_UART.Alarm), sizeof(PM_UART.Alarm));					// [39]~[42] ; 4 Byte
//		memcpy((void*)(g_arUart6TxBuf+43), (void*)(&temp), sizeof(temp));										// [43]~[44] ; 2 Byte
//		memcpy((void*)(g_arUart6TxBuf+45), (void*)(&humi), sizeof(humi));										// [45]~[46] ; 2 Byte
//
//		for(l_deChksumCnt = 1; l_deChksumCnt < 47 ; l_deChksumCnt++)
//			l_deChksum += g_arUart6TxBuf[l_deChksumCnt];
//
//		g_arUart6TxBuf[47] = (uint8_t)(l_deChksum >> 8);					// Checksum, Upper
//		g_arUart6TxBuf[48] = (uint8_t)(l_deChksum & 0xff);					// Checksum, Lower
//		g_arUart6TxBuf[49] = 'E';											// End of Transmit
//
//		(l_deDataCnt == 255) ? (l_deDataCnt = 0) : (l_deDataCnt++);
//
//		HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);		//DE high
//		HAL_UART_Transmit_IT(&hUart6, g_arUart6TxBuf, 50);
//	}
//	g_deUart6DataCmdFlg = 0;
}

void UART7_RECV(void)
{
	uint8_t l_deTemp = 0;
	static uint16_t l_deUart7PacketCnt = 0, l_deUart7PacketSize = 0, l_deChksumUpper = 0, l_deChksumLower = 0;
	static uint16_t l_deChksum = 0;

	if(g_deMain485PtclNum == 0)		// ACT Protocol
	{
		if(0)	// if Received Packet is Error
		{

		}
		else	// if Received Packet is Valid
		{
			l_deTemp = g_arUart7RxBuf[0];

			if(l_deUart7PacketCnt == 0)						// if Only Uart7 is Ready
			{
				if(l_deTemp == 0x53)							// if Received Data is Stx
				{
					l_deUart7PacketCnt = 1;
					l_deChksum = 0;
					g_deUart7DataCmdFlg = 0;
				}
				else
				{
					l_deUart7PacketCnt = 0;
				}
			}
			else if(l_deUart7PacketCnt < 6)
			{
				g_arUart7RxBuf[l_deUart7PacketCnt] = l_deTemp;
				l_deUart7PacketCnt++;

				l_deChksum += l_deTemp;
			}
			else if(l_deUart7PacketCnt == 6)
			{
				g_arUart7RxBuf[l_deUart7PacketCnt] = l_deTemp;
				l_deUart7PacketCnt++;

				l_deUart7PacketSize = ((g_arUart7RxBuf[2] & 0xff) | ((uint16_t)g_arUart7RxBuf[1] << 8));

				l_deChksum += l_deTemp;
			}
			else														// if Uart7 is Running
			{
				if(l_deUart7PacketCnt < 31)							// if Only Counter is Not Over Limit(Maximum Packet)
				{
					g_arUart7RxBuf[l_deUart7PacketCnt] = l_deTemp;

					if(l_deUart7PacketCnt > l_deUart7PacketSize+2)		// if Received All Data(Before Checksum)
					{
						if(l_deUart7PacketCnt == l_deUart7PacketSize+5)
						{
							if(l_deChksum == ((g_arUart7RxBuf[l_deUart7PacketCnt-1] & 0xff)	| ((uint16_t)g_arUart7RxBuf[l_deUart7PacketCnt-2] << 8)))		// if 'Check Sum' and Summation are Matched
							{
								g_deUart7ConnFlg = 1;
								g_deUart7ConnCnt = 0;
								g_deUart7DataCmdFlg = 0x001b;
								UART7_TRANS();
							}
							l_deUart7PacketCnt = 0;
							l_deChksum = 0;
						}
						else
						{
							l_deUart7PacketCnt++;
						}
					}
					else										// [7]~[22]
					{
						l_deChksum += l_deTemp;
						l_deUart7PacketCnt++;
					}


				}
				else
				{
					l_deUart7PacketCnt = 0;
					g_deUart7DataCmdFlg = 0;
				}
			}
		}
	}
	else	// MC Protocol
	{
		if(0)	// if Received Packet is Error
		{

		}
		else	// if Received Packet is Valid
		{
			l_deTemp = g_arUart7RxBuf[0];

			if(l_deUart7PacketCnt == 0)						// if Only Uart7 is Ready
			{
				if(l_deTemp == 0x10)							// if Received Data is Stx
				{
					l_deUart7PacketCnt = 1;
				}
			}
			else if(l_deUart7PacketCnt == 1)
			{
				if(l_deTemp == 0x02)							// if Received Data is Stx
				{
					g_arUart7RxBuf[1] = l_deTemp;				// Clear Buffer for Length
					l_deUart7PacketCnt = 2;
				}
				else
				{
					l_deUart7PacketCnt = 0;
				}
			}
			else if(l_deUart7PacketCnt <= 3)
			{
				g_arUart7RxBuf[l_deUart7PacketCnt] = l_deTemp;
				l_deUart7PacketCnt++;

				l_deUart7PacketSize = ((g_arUart7RxBuf[2] & 0xff) | ((uint16_t)g_arUart7RxBuf[3] << 8));

				if(l_deTemp != 0x10)
					l_deChksum += l_deTemp;
			}
			else														// if Uart7 is Running
			{
				if(l_deUart7PacketCnt < 32)							// if Only Counter is Not Over Limit(Maximum Packet)
				{
					g_arUart7RxBuf[l_deUart7PacketCnt] = l_deTemp;

					if(l_deUart7PacketCnt == l_deUart7PacketSize+7)		// if
					{
						l_deChksum -= 0x03;
						l_deChksumUpper = ((l_deChksum >> 4) & 0x0f);
						l_deChksumLower = (l_deChksum & 0x0f);

						if(g_arUart7RxBuf[l_deUart7PacketCnt-1] == (l_deChksumUpper > 9) ? (l_deChksumUpper+'7') : (l_deChksumUpper+'0'))		// if 'Check Sum' and Summation are Matched
						{
							if(g_arUart7RxBuf[l_deUart7PacketCnt] == (l_deChksumLower > 9) ? (l_deChksumLower+'7') : (l_deChksumLower+'0'))
							{
								g_deUart7ConnFlg = 1;
								g_deUart7ConnCnt = 0;
							}
						}
						l_deUart7PacketCnt = 0;
						l_deChksum = 0;
					}
					else										// [4]~[18]
					{
						l_deUart7PacketCnt++;
						if(l_deUart7PacketCnt <= l_deUart7PacketSize+5)
							if(l_deTemp != 0x10)
								l_deChksum += l_deTemp;
					}
				}
				else
				{
					l_deUart7PacketCnt = 0;
				}
			}
		}
	}
}

void UART7_TRANS(void)
{
	uint8_t l_deDleCnt = 0, l_deTemp2 = 0, l_deChksumCnt = 0, l_deSort = 0, l_deChksumUpper = 0, l_deChksumLower = 0;
	uint16_t l_deChksum = 0;
	static uint8_t l_deUart7Heartbit = 0, l_deDataCnt = 0;
//	uint16_t temp = 0, humi = 0;//temporary

	l_deUart7Heartbit ^= 1;

	if(g_deMainMode485Num == 1)
	{
		if(g_deMain485PtclNum == 0)		// ACT Protocol
		{
			if((g_deUart7DataCmdFlg == 0x001b)||(g_deUart7DataCmdFlg == 0x0101))
			{
	//			g_arUart7TxBuf[0] = 'S';											// Start of Transmit
	//			g_arUart7TxBuf[1] = 0x00;											// Length, Upper
	//			g_arUart7TxBuf[2] = 0x2c;											// Length, Lower
	//			g_arUart7TxBuf[3] = 0x02;											// Type : 0x02(PM Sensor Module)
	//			g_arUart7TxBuf[4] = 0x00;											// Command, Upper
	//			g_arUart7TxBuf[5] = 0x1b;											// Command, Lower
	//			g_arUart7TxBuf[6] = l_deDataCnt;									// Counter : 0~255
	//
	//			temp = (uint16_t)(TH_I2C.Temperature*100);
	//			humi = (uint16_t)(TH_I2C.Humidity*100);
	//
	//			memcpy((void*)(g_arUart7TxBuf+7), (void*)(&DNT_RTC), sizeof(DNT_RTC));								// [ 7]~[14] ; 8 Byte
	//			memcpy((void*)(g_arUart7TxBuf+15), (void*)(&PM_UART.Cnt), sizeof(PM_UART.Cnt));						// [15]~[38] ; 24 Byte
	//			memcpy((void*)(g_arUart7TxBuf+39), (void*)(&PM_UART.Alarm), sizeof(PM_UART.Alarm));					// [39]~[42] ; 4 Byte
	//			memcpy((void*)(g_arUart7TxBuf+43), (void*)(&temp), sizeof(temp));										// [43]~[44] ; 2 Byte
	//			memcpy((void*)(g_arUart7TxBuf+45), (void*)(&humi), sizeof(humi));										// [45]~[46] ; 2 Byte
	//
	//			for(l_deChksumCnt = 1; l_deChksumCnt < 47 ; l_deChksumCnt++)
	//				l_deChksum += g_arUart7TxBuf[l_deChksumCnt];
	//
	//			g_arUart7TxBuf[47] = (uint8_t)(l_deChksum >> 8);					// Checksum, Upper
	//			g_arUart7TxBuf[48] = (uint8_t)(l_deChksum & 0xff);					// Checksum, Lower
	//			g_arUart7TxBuf[49] = 'E';											// End of Transmit
	//
	//			(l_deDataCnt == 255) ? (l_deDataCnt = 0) : (l_deDataCnt++);
	//
	//			HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);		//DE high
	//			HAL_UART_Transmit_IT(&huart7, g_arUart7TxBuf, 50);

				g_arUart7TxBuf[0] = 'S';															// Start of Transmit
				g_arUart7TxBuf[1] = 0x00;															// Length, Upper
				g_arUart7TxBuf[2] = 0x3e;															// Length, Lower
				g_arUart7TxBuf[3] = 0x02;															// Type : 0x02(PM Sensor Module)
				g_arUart7TxBuf[4] = 0x00;															// Command, Upper
				g_arUart7TxBuf[5] = 0x1b;															// Command, Lower
				g_arUart7TxBuf[6] = l_deDataCnt;													// Counter : 0~255

				g_arUart7TxBuf[7] = g_deMain485ModNum & 0x1f;										// [ 7] ; 1 Byte
				memcpy((void*)(g_arUart7TxBuf+8), (void*)(g_deRtcDateBuf), 3);						// [ 8]~[10] ; 3 Bytes
				memcpy((void*)(g_arUart7TxBuf+11), (void*)(g_deRtcTimeBuf), 4);						// [11]~[14] ; 4 Bytes
//				memcpy((void*)(g_arUart7TxBuf+15), (void*)(g_arRtcUptime), 8);						// [15]~[22] ; 8 Bytes
//				memcpy((void*)(g_arUart7TxBuf+23), (void*)(&PM_UART.Cnt), sizeof(PM_UART.Cnt));		// [23]~[46] ; 24 Byte
//
//				memcpy((void*)(g_arUart7TxBuf+47), (void*)(&BStatus.bit), 2);						// [47]~[48] ; 2 Byte
//				memcpy((void*)(g_arUart7TxBuf+49), (void*)(&BAlarm.bit), 2);						// [49]~[50] ; 2 Byte
//				memcpy((void*)(g_arUart7TxBuf+51), (void*)(&BError.bit), 2);						// [51]~[52] ; 2 Byte
//				memcpy((void*)(g_arUart7TxBuf+53), (void*)(&BDio.bit), 2);							// [53]~[54] ; 2 Byte


				g_arUart7TxBuf[15] = (uint8_t)(g_arRtcUptime[0] >> 8);
				g_arUart7TxBuf[16] = (uint8_t)(g_arRtcUptime[0] & 0xff);
				g_arUart7TxBuf[17] = (uint8_t)(g_arRtcUptime[1] >> 8);
				g_arUart7TxBuf[18] = (uint8_t)(g_arRtcUptime[1] & 0xff);
				g_arUart7TxBuf[19] = (uint8_t)(g_arRtcUptime[2] >> 8);
				g_arUart7TxBuf[20] = (uint8_t)(g_arRtcUptime[2] & 0xff);
				g_arUart7TxBuf[21] = (uint8_t)(g_arRtcUptime[3] >> 8);
				g_arUart7TxBuf[22] = (uint8_t)(g_arRtcUptime[3] & 0xff);
				g_arUart7TxBuf[23] = (uint8_t)(PM_UART.Cnt.Pm003 >> 24);
				g_arUart7TxBuf[24] = (uint8_t)(PM_UART.Cnt.Pm003 >> 16);
				g_arUart7TxBuf[25] = (uint8_t)(PM_UART.Cnt.Pm003 >> 8);
				g_arUart7TxBuf[26] = (uint8_t)(PM_UART.Cnt.Pm003 & 0xff);
				g_arUart7TxBuf[27] = (uint8_t)(PM_UART.Cnt.Pm005 >> 24);
				g_arUart7TxBuf[28] = (uint8_t)(PM_UART.Cnt.Pm005 >> 16);
				g_arUart7TxBuf[29] = (uint8_t)(PM_UART.Cnt.Pm005 >> 8);
				g_arUart7TxBuf[30] = (uint8_t)(PM_UART.Cnt.Pm005 & 0xff);
				g_arUart7TxBuf[31] = (uint8_t)(PM_UART.Cnt.Pm010 >> 24);
				g_arUart7TxBuf[32] = (uint8_t)(PM_UART.Cnt.Pm010 >> 16);
				g_arUart7TxBuf[33] = (uint8_t)(PM_UART.Cnt.Pm010 >> 8);
				g_arUart7TxBuf[34] = (uint8_t)(PM_UART.Cnt.Pm010 & 0xff);
				g_arUart7TxBuf[35] = (uint8_t)(PM_UART.Cnt.Pm025 >> 24);
				g_arUart7TxBuf[36] = (uint8_t)(PM_UART.Cnt.Pm025 >> 16);
				g_arUart7TxBuf[37] = (uint8_t)(PM_UART.Cnt.Pm025 >> 8);
				g_arUart7TxBuf[38] = (uint8_t)(PM_UART.Cnt.Pm025 & 0xff);
				g_arUart7TxBuf[39] = (uint8_t)(PM_UART.Cnt.Pm050 >> 24);
				g_arUart7TxBuf[40] = (uint8_t)(PM_UART.Cnt.Pm050 >> 16);
				g_arUart7TxBuf[41] = (uint8_t)(PM_UART.Cnt.Pm050 >> 8);
				g_arUart7TxBuf[42] = (uint8_t)(PM_UART.Cnt.Pm050 & 0xff);
				g_arUart7TxBuf[43] = (uint8_t)(PM_UART.Cnt.Pm100 >> 24);
				g_arUart7TxBuf[44] = (uint8_t)(PM_UART.Cnt.Pm100 >> 16);
				g_arUart7TxBuf[45] = (uint8_t)(PM_UART.Cnt.Pm100 >> 8);
				g_arUart7TxBuf[46] = (uint8_t)(PM_UART.Cnt.Pm100 & 0xff);
				g_arUart7TxBuf[47] = (uint8_t)(BStatus.all >> 8);
				g_arUart7TxBuf[48] = (uint8_t)(BStatus.all & 0xff);
				g_arUart7TxBuf[49] = (uint8_t)(BAlarm.all >> 8);
				g_arUart7TxBuf[50] = (uint8_t)(BAlarm.all & 0xff);
				g_arUart7TxBuf[51] = (uint8_t)(BError.all >> 8);
				g_arUart7TxBuf[52] = (uint8_t)(BError.all & 0xff);
				g_arUart7TxBuf[53] = (uint8_t)(BDio.all >> 8);
				g_arUart7TxBuf[54] = (uint8_t)(BDio.all & 0xff);


//				memset((void*)(g_arUart7TxBuf+55), 0, 10);

				for(l_deChksumCnt = 1; l_deChksumCnt < 65 ; l_deChksumCnt++)
					l_deChksum += g_arUart7TxBuf[l_deChksumCnt];

				g_arUart7TxBuf[65] = (uint8_t)(l_deChksum >> 8);					// Checksum, Upper
				g_arUart7TxBuf[66] = (uint8_t)(l_deChksum & 0xff);					// Checksum, Lower
				g_arUart7TxBuf[67] = 'E';											// End of Transmit

				(l_deDataCnt == 255) ? (l_deDataCnt = 0) : (l_deDataCnt++);

				HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);		//DE high
				HAL_UART_Transmit_IT(&huart7, g_arUart7TxBuf, 68);
			}
			g_deUart7DataCmdFlg = 0;
		}
		else	// MC Protocol
		{
			g_arUart7TxBuf[0] = 0x10;							// Control Code, DLE
			g_arUart7TxBuf[1] = 0x02;							// Control Code, STX

			g_arUart7TxBuf[2] = 32+18;							// Number of Data Bytes([4]~[53](50), Lower Byte, Except DLE)
			g_arUart7TxBuf[3] = 0x00;							// Number of Data Bytes([4]~[53](50), Upper Byte, Except DLE)
			g_arUart7TxBuf[4] = 0xf8;							// Frame ID No.(Frame 4C)
			g_arUart7TxBuf[5] = g_deMain485DestNum & 0x1f;		// Access Route 1 of 7(Station No.)
			g_arUart7TxBuf[6] = 0x00;							// Access Route 2 of 7(Network No.)
			g_arUart7TxBuf[7] = 0xff;							// Access Route 3 of 7(PLC No.)
			g_arUart7TxBuf[8] = 0xff;							// Access Route 4 of 7(Request Destination Module I/O No.)
			g_arUart7TxBuf[9] = 0x03;							// Access Route 5 of 7(Request Destination Module I/O No.)
			g_arUart7TxBuf[10] = 0x00;							// Access Route 6 of 7(Request Destination Module Station No.)
			g_arUart7TxBuf[11] = g_deMain485ModNum & 0x1f;		// Access Route 7 of 7(Self Station No.)

			g_arUart7TxBuf[12] = 0x01;							// Command(Batch Write, 01h)
			g_arUart7TxBuf[13] = 0x14;							// Command(Batch Write, 14h)
			g_arUart7TxBuf[14] = 0x00;							// Subcommand(MELSEC-Q/L series, 00h)
			g_arUart7TxBuf[15] = 0x00;							// Subcommand(MELSEC-Q/L series, 00h)
			g_arUart7TxBuf[16] = g_deMain485DvldNum & 0xff;		// Head Device(Lower Byte)
			g_arUart7TxBuf[17] = g_deMain485DvldNum >> 8;		// Head Device(Middle Byte)
			g_arUart7TxBuf[18] = 0x00;							// Head Device(Upper Byte)
			g_arUart7TxBuf[19] = g_deMain485DvcdNum;			// Device Code(Data or Link Register)
			g_arUart7TxBuf[20] = 0x10;							// Number of Device Points(16 word(0010h), Lower Byte)
			g_arUart7TxBuf[21] = 0x00;							// Number of Device Points(16 word(0000h), Upper Byte)

			g_arUart7TxBuf[22] = (uint8_t)(PM_UART.Cnt.Pm005 & 0xff);
			g_arUart7TxBuf[23] = (uint8_t)(PM_UART.Cnt.Pm005 >> 8);
			g_arUart7TxBuf[24] = (uint8_t)(PM_UART.Cnt.Pm005 >> 16);
			g_arUart7TxBuf[25] = (uint8_t)(PM_UART.Cnt.Pm005 >> 24);
			g_arUart7TxBuf[26] = (uint8_t)(PM_UART.Cnt.Pm010 & 0xff);
			g_arUart7TxBuf[27] = (uint8_t)(PM_UART.Cnt.Pm010 >> 8);
			g_arUart7TxBuf[28] = (uint8_t)(PM_UART.Cnt.Pm010 >> 16);
			g_arUart7TxBuf[29] = (uint8_t)(PM_UART.Cnt.Pm010 >> 24);
			g_arUart7TxBuf[30] = (uint8_t)(PM_UART.Cnt.Pm025 & 0xff);
			g_arUart7TxBuf[31] = (uint8_t)(PM_UART.Cnt.Pm025 >> 8);
			g_arUart7TxBuf[32] = (uint8_t)(PM_UART.Cnt.Pm025 >> 16);
			g_arUart7TxBuf[33] = (uint8_t)(PM_UART.Cnt.Pm025 >> 24);
			g_arUart7TxBuf[34] = (uint8_t)(PM_UART.Cnt.Pm050 & 0xff);
			g_arUart7TxBuf[35] = (uint8_t)(PM_UART.Cnt.Pm050 >> 8);
			g_arUart7TxBuf[36] = (uint8_t)(PM_UART.Cnt.Pm050 >> 16);
			g_arUart7TxBuf[37] = (uint8_t)(PM_UART.Cnt.Pm050 >> 24);
			g_arUart7TxBuf[38] = (uint8_t)(PM_UART.Cnt.Pm100 & 0xff);
			g_arUart7TxBuf[39] = (uint8_t)(PM_UART.Cnt.Pm100 >> 8);
			g_arUart7TxBuf[40] = (uint8_t)(PM_UART.Cnt.Pm100 >> 16);
			g_arUart7TxBuf[41] = (uint8_t)(PM_UART.Cnt.Pm100 >> 24);
			g_arUart7TxBuf[42] = (uint8_t)(((int32_t)(TH_I2C.Temperature*100)) & 0xff);
			g_arUart7TxBuf[43] = (uint8_t)(((int32_t)(TH_I2C.Temperature*100)) >> 8);
			g_arUart7TxBuf[44] = (uint8_t)(((int32_t)(TH_I2C.Temperature*100)) >> 16);
			g_arUart7TxBuf[45] = (uint8_t)(((int32_t)(TH_I2C.Temperature*100)) >> 24);
			g_arUart7TxBuf[46] = (uint8_t)(((int32_t)(TH_I2C.Humidity*100)) & 0xff);
			g_arUart7TxBuf[47] = (uint8_t)(((int32_t)(TH_I2C.Humidity*100)) >> 8);
			g_arUart7TxBuf[48] = (uint8_t)(((int32_t)(TH_I2C.Humidity*100)) >> 16);
			g_arUart7TxBuf[49] = (uint8_t)(((int32_t)(TH_I2C.Humidity*100)) >> 24);
			g_arUart7TxBuf[50] = (((BError.bit.EepromConnErr) || (BError.bit.SdConnErr))<< 3)
					| (BError.bit.PmConnErr << 2) | (BError.bit.FanAsErr << 1)
					| ((!BError.bit.EepromConnErr) && (!BError.bit.SdConnErr) && (!BError.bit.PmConnErr) && (!BError.bit.FanAsErr));

//			g_arUart7TxBuf[50] = (((BResult.bit.I2C1ConnErr) || (BResult.bit.I2C4ConnErr) || (BResult.bit.SdmmcConnErr) || (BResult.bit.Uart1ConnErr)
//					|| (BResult.bit.TempStatErr) || (BResult.bit.HumiStatErr)) << 3)
//					| (BResult.bit.Uart2ConnErr << 2) | (BResult.bit.FanStatErr << 1)
//					| ((!BResult.bit.I2C1ConnErr) && (!BResult.bit.I2C4ConnErr) && (!BResult.bit.SdmmcConnErr) && (!BResult.bit.Uart1ConnErr)
//							&& (!BResult.bit.Uart2ConnErr) && (!BResult.bit.FanStatErr) && (!BResult.bit.TempStatErr) && (!BResult.bit.HumiStatErr));
			g_arUart7TxBuf[51] = l_deUart7Heartbit << 7;
			g_arUart7TxBuf[52] = (BError.bit.EepromConnErr)?(0x63):((BError.bit.SdConnErr)?(0x67):((BError.bit.PmConnErr)?(0x65):((BError.bit.FanAsErr)?(0x61):(0))));
			g_arUart7TxBuf[53] = 0x00;

			g_arUart7TxBuf[54] = 0x10;		// Control Code, DLE
			g_arUart7TxBuf[55] = 0x03;		// Control Code, ETX

			// Summation of Data and Separate for Ascii Conversion
			for(l_deChksumCnt = 2; l_deChksumCnt < 54; l_deChksumCnt++)
				l_deChksum += g_arUart7TxBuf[l_deChksumCnt];

			l_deChksumUpper = ((l_deChksum >> 4) & 0x0f);
			l_deChksumLower = (l_deChksum & 0x0f);

			// Check DLE Code and Addition DLE Code
			for(l_deTemp2 = 4; l_deTemp2 < (54+l_deDleCnt); l_deTemp2++)
			{
				if(g_arUart7TxBuf[l_deTemp2] == 0x10)
				{
					for(l_deSort = (55+l_deDleCnt); l_deSort >= l_deTemp2; l_deSort--)
					{
						g_arUart7TxBuf[l_deSort+1] = g_arUart7TxBuf[l_deSort];
					}
					l_deTemp2++;
					l_deDleCnt++;
				}
			}

			// Addition Sum Check Code Converted to Ascii
			g_arUart7TxBuf[56+l_deDleCnt] = (l_deChksumUpper > 9) ? (l_deChksumUpper+'7') : (l_deChksumUpper+'0');	// Sum Check Code, Upper(Except DLE)
			g_arUart7TxBuf[57+l_deDleCnt] = (l_deChksumLower > 9) ? (l_deChksumLower+'7') : (l_deChksumLower+'0');	// Sum Check Code, Lower(Except DLE)

			HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, 1);		//DE high
			HAL_UART_Transmit_IT(&huart7, g_arUart7TxBuf, 58+l_deDleCnt);
		}
	}
}
