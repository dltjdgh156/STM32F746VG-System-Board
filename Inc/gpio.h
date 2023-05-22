//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	gpio.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef GPIO_H_
#define GPIO_H_
// Includes



// Defines
#define SW_SHORT_CNT		50		// 0.05 s @ 1 kHz
#define SW_MIDDLE_CNT		500		// 0.5 s @ 1 kHz
#define SW_LONG_CNT			2500	// 2.5 s @ 1 kHz
#define SW_MAX_CNT			10000	// 10. s @ 1 kHz
#define GPIO_SELECT			GPIOE->BSRR = (1 << 30)	//.bit.BR14 = 1
#define GPIO_UNSELECT		GPIOE->BSRR = (1 << 14)	//.bit.BS14 = 1
#define GPIO_CMD			GPIOB->BSRR = (1 << 30)	//.bit.BR14 = 1
#define GPIO_DATA			GPIOB->BSRR = (1 << 14)	//.bit.BS14 = 1
#define GPIO_RESET			GPIOE->BSRR = (1 << 31)	//.bit.BR15 = 1
#define GPIO_SET			GPIOE->BSRR = (1 << 15)	//.bit.BS15 = 1
#define GPIO_LEDOFF			GPIOD->BSRR = (1 << 29)	//.bit.BR13 = 1
#define GPIO_LEDON			GPIOD->BSRR = (1 << 13)	//.bit.BS13 = 1


// Variables
extern uint8_t g_deSw1ShortFlg, g_deSw2ShortFlg, g_deSw3ShortFlg, g_deSw4ShortFlg, g_deSw1LongFlg, g_deSw2LongFlg, g_deSw3LongFlg, g_deSw4LongFlg;
extern uint8_t g_deSw1ShortFlg_k, g_deSw2ShortFlg_k, g_deSw3ShortFlg_k, g_deSw4ShortFlg_k, g_deSw1LongFlg_k, g_deSw2LongFlg_k, g_deSw3LongFlg_k, g_deSw4LongFlg_k;
extern uint8_t g_deSwCmdFlg;
extern uint8_t g_deScrnSaveFlg, g_deScrnSaveFlg_k;
extern uint32_t g_deScrnSaveCnt;

// Structures
typedef struct
{
	uint32_t ExternalInput1Inv		: 1;
	uint32_t ExternalInput2Inv		: 1;
	uint32_t ExternalInput3Inv		: 1;
	uint32_t ExternalInput4Inv		: 1;
	uint32_t Switch1Inv				: 1;
	uint32_t Switch2Inv				: 1;
	uint32_t Switch3Inv				: 1;
	uint32_t Switch4Inv				: 1;
	uint32_t						: 0;
}
DigitalIn_t;
extern DigitalIn_t Din;

typedef struct
{

	uint32_t ExternalOutput1		: 1;
	uint32_t ExternalOutput2		: 1;
	uint32_t ExternalOutput3		: 1;
	uint32_t ExternalOutput4		: 1;
	uint32_t LanResetInv			: 1;
	uint32_t LcdLed					: 1;
	uint32_t LcdResetInv			: 1;
	uint32_t Led1Inv				: 1;
	uint32_t Led2Inv				: 1;
	uint32_t SensorPower			: 1;
	uint32_t Spi4SelectInv			: 1;
	uint32_t						: 0;
}
DigitalOut_t;
extern DigitalOut_t Dout;


// Functions
extern void GPIO_INIT(void);					// Initialize General Purpose Input Output
extern void GPIO_READ(void);
extern void GPIO_WRITE(void);


#endif /* GPIO_H_ */
