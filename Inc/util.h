//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	util.h
//	MAKE	BY		:
//	DESCRIPTION		:	utility functions
//	CREATED DATE	:	0000.00.00
//	----------------------------------------------------------------------------------------------------


#ifndef UTIL_H_
#define UTIL_H_
// Includes



// Defines
#define PI				3.1415926535897932
#define INV_PI2			0.1591549430918953

#define BOUND_PI(x)		((x>0) ? ((x)-2.*PI*(long)((x+PI)*INV_PI2)) : ((x)-2.*PI*(long)((x-PI)*INV_PI2)))
#define ABS(x)				((x < 0)? -(x): (x))

#define SWAPBYTE16(x)		(((x <<  8) & 0xff00) | ((x >>  8) & 0x00ff))
#define SWAPBYTE32(x)		(((x << 24) & 0xff000000) | ((x <<  8) & 0x00ff0000) | ((x >> 8) & 0x0000ff00) | ((x >> 24) & 0x000000ff))

#define SWAPINT8(x, y)		do{int8_t temp = x; x = y; y = temp;}while(0)
#define SWAPUINT8(x, y)		do{uint8_t temp = x; x = y; y = temp;}while(0)
#define SWAPINT16(x, y)		do{int16_t temp = x; x = y; y = temp;}while(0)
#define SWAPUINT16(x, y)	do{uint16_t temp = x; x = y; y = temp;}while(0)



// Variables



// Structures
typedef struct
{
    uint32_t front;
    uint32_t rear;
    uint32_t lenq;
    uint32_t size;
    uint16_t *ch;
}QUEUE;

extern QUEUE g_stSciaRxQue;
extern QUEUE g_stSciaTxQue;
extern QUEUE g_stScibRxQue;
extern QUEUE g_stScibTxQue;
extern QUEUE g_stScicRxQue;
extern QUEUE g_stScicTxQue;
extern QUEUE g_stScidRxQue;
extern QUEUE g_stScidTxQue;
extern QUEUE g_stSpiaRxQue;
extern QUEUE g_stSpiaTxQue;
extern QUEUE g_stSpibRxQue;
extern QUEUE g_stSpibTxQue;
extern QUEUE g_stSpicRxQue;
extern QUEUE g_stSpicTxQue;
extern QUEUE g_stSpidRxQue;
extern QUEUE g_stSpidTxQue;

typedef struct
{
	uint8_t type;
	float w0;
	float delT;
	float coeff[3], reg;
}IIR1;

extern IIR1 g_stIIR1Coeff1;
extern IIR1 g_stIIR1Coeff2;
extern IIR1 g_stIIR1Coeff3;
extern IIR1 g_stIIR1Coeff4;
extern IIR1 g_stIIR1Coeff5;
extern IIR1 g_stIIR1Coeff6;
extern IIR1 g_stIIR1Coeff7;
extern IIR1 g_stIIR1Coeff8;

typedef struct
{
	uint8_t type;
	float w0, zeta;
	float delT;
	float coeff[5], reg[2];
}IIR2;

extern IIR2 g_stIIR2Coeff1;
extern IIR2 g_stIIR2Coeff2;
extern IIR2 g_stIIR2Coeff3;
extern IIR2 g_stIIR2Coeff4;
extern IIR2 g_stIIR2Coeff5;
extern IIR2 g_stIIR2Coeff6;
extern IIR2 g_stIIR2Coeff7;
extern IIR2 g_stIIR2Coeff8;


// Functions
extern uint8_t Crc8Maxim(uint8_t a_arBuffer, uint8_t a_deInitial);
extern uint16_t Crc16Xmodem(uint16_t* a_arBuffer, uint16_t a_deLength);
extern void IIR1Init(IIR1 *a_arIir, float a_deOmega0);
extern float IIR1Update(IIR1 *a_arIir, const float a_deInput);
extern void IIR2Init(IIR2 *a_arIir, float a_deOmega0, float a_deZeta);
extern float IIR2Update(IIR2 *a_arIir, const float a_deInput);
extern void QueInit(QUEUE *a_arQueue, uint16_t* a_arBuffer, uint32_t a_deLength);
extern uint8_t DeQueRegI8(QUEUE *a_arQueue, volatile uint16_t* a_coRegister, uint16_t a_deLength);
extern uint8_t EnQueRegI8(QUEUE *a_arQueue, volatile uint16_t* a_coRegister, uint32_t a_deLength);
extern void ConvertNum2AscDec(uint32_t a_deNumber, int8_t* a_arAscii, uint8_t a_deDigit);
extern void ConvertNum2AscHex(uint32_t a_deNumber, int8_t* a_arAscii, uint8_t a_deDigit);

#endif /* UTIL_H_ */
