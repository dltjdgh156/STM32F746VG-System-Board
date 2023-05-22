//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	util.c
//	MAKE	BY		:
//	DESCRIPTION		:	utility functions
//	CREATED DATE	:	0000.00.00
//	----------------------------------------------------------------------------------------------------


// Inlcudes
#include "stm32f7xx.h"
#include "main.h"


// Defines



// Variables
static const uint16_t g_arCrc16Table[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};


// Structures
QUEUE g_stSciaRxQue;
QUEUE g_stSciaTxQue;
QUEUE g_stScibRxQue;
QUEUE g_stScibTxQue;
QUEUE g_stScicRxQue;
QUEUE g_stScicTxQue;
QUEUE g_stScidRxQue;
QUEUE g_stScidTxQue;

QUEUE g_stSpiaRxQue;
QUEUE g_stSpiaTxQue;
QUEUE g_stSpibRxQue;
QUEUE g_stSpibTxQue;
QUEUE g_stSpicRxQue;
QUEUE g_stSpicTxQue;
QUEUE g_stSpidRxQue;
QUEUE g_stSpidTxQue;

IIR1 g_stIIR1Coeff1;
IIR1 g_stIIR1Coeff2;
IIR1 g_stIIR1Coeff3;
IIR1 g_stIIR1Coeff4;
IIR1 g_stIIR1Coeff5;
IIR1 g_stIIR1Coeff6;
IIR1 g_stIIR1Coeff7;
IIR1 g_stIIR1Coeff8;

IIR2 g_stIIR2Coeff1;
IIR2 g_stIIR2Coeff2;
IIR2 g_stIIR2Coeff3;
IIR2 g_stIIR2Coeff4;
IIR2 g_stIIR2Coeff5;
IIR2 g_stIIR2Coeff6;
IIR2 g_stIIR2Coeff7;
IIR2 g_stIIR2Coeff8;


// Functions
//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	Crc8Maxim
//	ARGUMENTS	:	uint16_t* a_buffer, uint16_t a_length
//	RETURN		:	uint16_t
//	SSD			:
//	DESCRIPTION	:	calculate CRC-16(CRC-CCITT XModem)
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
uint8_t Crc8Maxim(uint8_t a_arBuffer, uint8_t a_deInitial)
{
	register uint8_t cnt = 0;
	register uint8_t crc = a_deInitial;
	crc ^= a_arBuffer;
	for(cnt=0; cnt<8; cnt++)
	{
		crc = (crc&0x80) ? ((crc<<1) ^ 0x31) : (crc<<1);
	}
	return crc;
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	Crc16Xmodem
//	ARGUMENTS	:	uint16_t* a_buffer, uint16_t a_length
//	RETURN		:	uint16_t
//	SSD			:
//	DESCRIPTION	:	calculate CRC-16(CRC-CCITT XModem)
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
uint16_t Crc16Xmodem(uint16_t* a_arBuffer, uint16_t a_deLength)
{
	register uint16_t cnt = 0;
	register uint16_t crc = 0;
	for(cnt=0; cnt<a_deLength; cnt++)
	{
		crc = (crc<<8) ^ g_arCrc16Table[((crc>>8) ^ (*((uint16_t*)a_arBuffer++)&0x00ff))];
	}
	return crc;
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	IIR1Init
//	ARGUMENTS	:	IIR1 *a_arIir, float a_deOmega0
//	RETURN		:	void
//	SSD			:
//	DESCRIPTION	:	initialize IIR 1st filter
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
void IIR1Init(IIR1 *a_arIir, float a_deOmega0)
{
	float a0, b0, b1;
	float INV_alpha, dt;
	int16_t type;

	// Continuous-time Filter Coefficients
	a_arIir->w0 = a_deOmega0 ;
	type = a_arIir->type;
	dt = a_arIir->delT;

	a0 = a_deOmega0;
	switch(type)
	{
		case 1://Lowpass
			b0 = a_deOmega0;
			b1 = 0;
			break;
		case 2://Highpass
			b0 = 0;
			b1 = (float)1;
			break;
		default:
		case 0:	//Allpass
			b0 = -a_deOmega0;
			b1 = (float)1;
	}

	// Discrete-time Filter Coefficients
	INV_alpha = (float)1./((float)2 + dt*a0);
	a_arIir->coeff[0] = ((float)2*b1 + dt*b0)*INV_alpha;
	a_arIir->coeff[1] = (-(float)2*b1 + dt*b0)*INV_alpha;
	a_arIir->coeff[2] = -(-(float)2 + dt*a0)*INV_alpha;

	// Initialize Storage Elements
	a_arIir->reg = 0;
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	IIR1Update
//	ARGUMENTS	:	IIR1 *a_arIir, const float a_deInput
//	RETURN		:	float
//	SSD			:
//	DESCRIPTION	:	update IIR 1st filter
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
float IIR1Update(IIR1 *a_arIir, const float a_deInput)
{
	float l_vaOutput;

	l_vaOutput = a_arIir->reg + a_arIir->coeff[0]*a_deInput;
	a_arIir->reg = a_arIir->coeff[1]*a_deInput + a_arIir->coeff[2]*l_vaOutput;

	return l_vaOutput;
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	IIR2Init
//	ARGUMENTS	:	IIR2 *a_arIir, float a_deOmega0, float a_deZeta
//	RETURN		:	void
//	SSD			:
//	DESCRIPTION	:	initialize IIR 2nd filter
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
void IIR2Init(IIR2 *a_arIir, float a_deOmega0, float a_deZeta)
{
	float a0, a1, b0, b1, b2;
	float INV_alpha, dt;
	int16_t type;

	// Continuous-time Filter Coefficients
	a_arIir->w0 = a_deOmega0;
	a_arIir->zeta = a_deZeta;
	dt = a_arIir->delT ;
	type = a_arIir->type;

	a0 = a_deOmega0*a_deOmega0;
	a1 = 2*a_deZeta*a_deOmega0;
	switch(type)
	{
		case 1://Lowpass
			b0 = a_deOmega0*a_deOmega0;
			b1 = 0;
			b2 = 0;
			break;
		case 2://Highpass
			b0 = 0;
			b1 = 0;
			b2 = (float)1;
			break;
		case 3://Banspass
			b0 = 0;
			b1 = (float)2*a_deZeta*a_deOmega0;
			b2 = 0;
			break;
		case 4://Notch
			b0 = a_deOmega0*a_deOmega0;
			b1 = 0;
			b2 = (float)1;
			break;
		case 0://Allpass
		default:
			b0 = a_deOmega0*a_deOmega0;
			b1 = -(float)2*a_deZeta*a_deOmega0;
			b2 = (float)1;
	}

	// Discrete-time Filter Coefficients
	INV_alpha = (float)1./((float)4 + (float)2*dt*a1 + dt*dt*a0);
	a_arIir->coeff[0] = ((float)4*b2 + (float)2*dt*b1 + dt*dt*b0)*INV_alpha;
	a_arIir->coeff[1] = ((float)2*dt*dt*b0 - (float)8*b2)*INV_alpha;
	a_arIir->coeff[2] = -((float)2*dt*dt*a0 - (float)8)*INV_alpha;
	a_arIir->coeff[3] = ((float)4*b2 - (float)2*dt*b1 + dt*dt*b0)*INV_alpha;
	a_arIir->coeff[4] = -((float)4 - (float)2*dt*a1 + dt*dt*a0)*INV_alpha;

	// Initialize Storage Elements
	a_arIir->reg[0] = 0;
	a_arIir->reg[1] = 0;
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	IIR2Update
//	ARGUMENTS	:	IIR2 *a_arIir, const float a_deInput
//	RETURN		:	float
//	SSD			:
//	DESCRIPTION	:	update IIR 2nd filter
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
float IIR2Update(IIR2 *a_arIir, const float a_deInput)
{
	float l_vaOutput;

	l_vaOutput = a_arIir->reg[0] + a_arIir->coeff[0]*a_deInput;
	a_arIir->reg[0] = a_arIir->reg[1] + a_arIir->coeff[1]*a_deInput + a_arIir->coeff[2]*l_vaOutput;
	a_arIir->reg[1] = a_arIir->coeff[3]*a_deInput + a_arIir->coeff[4]*l_vaOutput;

	return l_vaOutput;
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	QueInit
//	ARGUMENTS	:	QUEUE *a_arQueue, uint16_t* a_arBuffer, uint32_t a_deLength
//	RETURN		:	void
//	SSD			:
//	DESCRIPTION	:	initialize circular queue buffer
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
void QueInit(QUEUE *a_arQueue, uint16_t* a_arBuffer, uint32_t a_deLength)
{
	uint16_t i;

    a_arQueue->front = 0;
    a_arQueue->rear = 0;
    a_arQueue->lenq = 0;
    a_arQueue->size = a_deLength;
    a_arQueue->ch   = a_arBuffer;

    for(i=0; i<a_deLength; i++)
    {
        a_arQueue->ch[i] = 0;
    }
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	DeQueReg
//	ARGUMENTS	:	QUEUE *a_arQueue, volatile uint16_t* a_coRegister, uint16_t a_deLength
//	RETURN		:	uint8_t
//	SSD			:
//	DESCRIPTION	:	dequeue to register(default uint8_t data)
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
uint8_t DeQueReg(QUEUE *a_arQueue, volatile uint16_t* a_coRegister, uint16_t a_deLength)
{
	uint16_t i;
    uint16_t ch;
    uint8_t l_deRet = 1;

    for(i=0; i < a_deLength; i++)
    {
        if(a_arQueue->lenq > 0)
        {

            ch = a_arQueue->ch[a_arQueue->front];//dequeue

            // Check the end of queue
            if(a_arQueue->front == a_arQueue->size -1)
            {
                a_arQueue->front = 0;
            }
            else
            {
                a_arQueue->front++;
            }
            // Calculate length of Queue
            if((int16_t)((int16_t)a_arQueue->rear - (int16_t)a_arQueue->front) >= 0)
            {
                a_arQueue->lenq = a_arQueue->rear - a_arQueue->front;
            }
            else
            {
                a_arQueue->lenq = (a_arQueue->size + a_arQueue->rear) - a_arQueue->front;
            }

            *a_coRegister = ch & 0xff;   // Write data
        }
        else	//if queue is empty
        {
        	l_deRet = 0;
			break;
        }
    }
	return l_deRet;
}

//	----------------------------------------------------------------------------------------------------
//	FUNCTION	:	EnQueReg
//	ARGUMENTS	:	QUEUE *a_arQueue, volatile uint16_t* a_coRegister, uint32_t a_deLength
//	RETURN		:	uint8_t
//	SSD			:
//	DESCRIPTION	:	enqueue from register(default uint8_t data)
//	UPDATE DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------
uint8_t EnQueReg(QUEUE *a_arQueue, volatile uint16_t* a_coRegister, uint32_t a_deLength)
{
	uint16_t i;
    uint16_t tmp;
    uint8_t l_deRet = 0;

    for(i=0UL; i<a_deLength; i++)
    {
        a_arQueue->ch[a_arQueue->rear] = *a_coRegister & 0xff;//enqueue & write

        if(a_arQueue->lenq == a_arQueue->size)//if queue is full
        {
            a_arQueue->front++;
            a_arQueue->rear++;

            if(a_arQueue->rear == a_arQueue->size)
            {
                a_arQueue->rear = 0;
                a_arQueue->front = 0;
                l_deRet = 1;
            }
            continue;
        }

        if(a_arQueue->rear == a_arQueue->size-1)// Check the end of queue
        {
            a_arQueue->rear = 0;
        }
        else
        {
            a_arQueue->rear++;
        }

        tmp = a_arQueue->rear - a_arQueue->front;//Calculate length of Queue

        if(tmp > 0)
        {
            a_arQueue->lenq = (uint32_t)tmp;
        }
        else
        {
            a_arQueue->lenq = a_arQueue->size + tmp;
        }
    }
	return l_deRet;
}

void ConvertNum2AscDec(uint32_t a_deNumber, int8_t* a_arAscii, uint8_t a_deDigit)
{
	switch(a_deDigit)
	{
		case 2 :
			*(int8_t*)a_arAscii = (a_deNumber/10)%10+'0';
			*(int8_t*)(a_arAscii+1) = a_deNumber%10+'0';

			if(*(int8_t*)a_arAscii == '0')
			{
				*(int8_t*)a_arAscii = ' ';
			}
			break;

		case 3 :
			*(int8_t*)a_arAscii = (a_deNumber/100)%10+'0';
			*(int8_t*)(a_arAscii+1) = (a_deNumber/10)%10+'0';
			*(int8_t*)(a_arAscii+2) = a_deNumber%10+'0';

			if(*(int8_t*)a_arAscii == '0')
			{
				*(int8_t*)a_arAscii = ' ';
				if(*(int8_t*)(a_arAscii+1) == '0')
				{
					*(int8_t*)(a_arAscii+1) = ' ';
				}
			}
			break;

		case 4 :
			*(int8_t*)a_arAscii = (a_deNumber/1000)%10+'0';
			*(int8_t*)(a_arAscii+1) = (a_deNumber/100)%10+'0';
			*(int8_t*)(a_arAscii+2) = (a_deNumber/10)%10+'0';
			*(int8_t*)(a_arAscii+3) = a_deNumber%10+'0';

			if(*(int8_t*)a_arAscii == '0')
			{
				*(int8_t*)a_arAscii = ' ';
				if(*(int8_t*)(a_arAscii+1) == '0')
				{
					*(int8_t*)(a_arAscii+1) = ' ';
					if(*(int8_t*)(a_arAscii+2) == '0')
					{
						*(int8_t*)(a_arAscii+2) = ' ';
					}
				}
			}
			break;

		case 5 :
			*(int8_t*)a_arAscii = (a_deNumber/10000)%10+'0';
			*(int8_t*)(a_arAscii+1) = (a_deNumber/1000)%10+'0';
			*(int8_t*)(a_arAscii+2) = (a_deNumber/100)%10+'0';
			*(int8_t*)(a_arAscii+3) = (a_deNumber/10)%10+'0';
			*(int8_t*)(a_arAscii+4) = a_deNumber%10+'0';

			if(*(int8_t*)a_arAscii == '0')
			{
				*(int8_t*)a_arAscii = ' ';
				if(*(int8_t*)(a_arAscii+1) == '0')
				{
					*(int8_t*)(a_arAscii+1) = ' ';
					if(*(int8_t*)(a_arAscii+2) == '0')
					{
						*(int8_t*)(a_arAscii+2) = ' ';
						if(*(int8_t*)(a_arAscii+3) == '0')
						{
							*(int8_t*)(a_arAscii+3) = ' ';
						}
					}
				}
			}
			break;

		case 6 :
			*(int8_t*)a_arAscii = (a_deNumber/100000)%10+'0';
			*(int8_t*)(a_arAscii+1) = (a_deNumber/10000)%10+'0';
			*(int8_t*)(a_arAscii+2) = (a_deNumber/1000)%10+'0';
			*(int8_t*)(a_arAscii+3) = (a_deNumber/100)%10+'0';
			*(int8_t*)(a_arAscii+4) = (a_deNumber/10)%10+'0';
			*(int8_t*)(a_arAscii+5) = a_deNumber%10+'0';

			if(*(int8_t*)a_arAscii == '0')
			{
				*(int8_t*)a_arAscii = ' ';
				if(*(int8_t*)(a_arAscii+1) == '0')
				{
					*(int8_t*)(a_arAscii+1) = ' ';
					if(*(int8_t*)(a_arAscii+2) == '0')
					{
						*(int8_t*)(a_arAscii+2) = ' ';
						if(*(int8_t*)(a_arAscii+3) == '0')
						{
							*(int8_t*)(a_arAscii+3) = ' ';
							if(*(int8_t*)(a_arAscii+4) == '0')
							{
								*(int8_t*)(a_arAscii+4) = ' ';
							}
						}
					}
				}
			}
			break;

		case 8 :
			*(int8_t*)a_arAscii = ((a_deNumber/10000000)%10)+'0';
			*(int8_t*)(a_arAscii+1) = ((a_deNumber/1000000)%10)+'0';
			*(int8_t*)(a_arAscii+2) = ((a_deNumber/100000)%10)+'0';
			*(int8_t*)(a_arAscii+3) = ((a_deNumber/10000)%10)+'0';
			*(int8_t*)(a_arAscii+4) = ((a_deNumber/1000)%10)+'0';
			*(int8_t*)(a_arAscii+5) = ((a_deNumber/100)%10)+'0';
			*(int8_t*)(a_arAscii+6) = ((a_deNumber/10)%10)+'0';
			*(int8_t*)(a_arAscii+7) = (a_deNumber%10)+'0';
			if(*(int8_t*)a_arAscii == '0')
			{
				*(int8_t*)a_arAscii = ' ';
				if(*(int8_t*)(a_arAscii+1) == '0')
				{
					*(int8_t*)(a_arAscii+1) = ' ';
					if(*(int8_t*)(a_arAscii+2) == '0')
					{
						*(int8_t*)(a_arAscii+2) = ' ';
						if(*(int8_t*)(a_arAscii+3)== '0')
						{
							*(int8_t*)(a_arAscii+3) = ' ';
							if(*(int8_t*)(a_arAscii+4) == '0')
							{
								*(int8_t*)(a_arAscii+4) = ' ';
								if(*(int8_t*)(a_arAscii+5) == '0')
								{
									*(int8_t*)(a_arAscii+5) = ' ';
									if(*(int8_t*)(a_arAscii+6) == '0')
									{
										*(int8_t*)(a_arAscii+6) = ' ';
									}
								}
							}
						}
					}
				}
			}
			break;
		default :
			break;
	}
}

void ConvertNum2AscHex(uint32_t a_deNumber, int8_t* a_arAscii, uint8_t a_deDigit)
{
	switch(a_deDigit)
	{
		case 1 :
			if((a_deNumber & 15) < 10)
			{
				*(int8_t*)a_arAscii= (a_deNumber & 15)+'0';
			}
			else
			{
				*(int8_t*)a_arAscii= (a_deNumber & 15)+'7';
			}
			break;

		case 2 :
			if((a_deNumber >> 4) < 10)
			{
				*(int8_t*)a_arAscii = (a_deNumber >> 4)+'0';
			}
			else
			{
				*(int8_t*)a_arAscii = (a_deNumber >> 4)+'7';
			}
			if((a_deNumber & 15) < 10)
			{
				*(int8_t*)(a_arAscii+1)= (a_deNumber & 15)+'0';
			}
			else
			{
				*(int8_t*)(a_arAscii+1)= (a_deNumber & 15)+'7';
			}
			break;
	}
}
