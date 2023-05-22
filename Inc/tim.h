//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	tim.h
//	MAKE	BY		:
//	DESCRIPTION		:	periodic timer function
//	CREATED DATE	:	0000.00.00
//	----------------------------------------------------------------------------------------------------


#ifndef TIM_H_
#define TIM_H_
// Includes



// Defines
#define SCALE_4C	7644		// 261.6256 Hz
#define SCALE_4CS	7214		// 277.1826 Hz
#define SCALE_4D	6809		// 293.6648 Hz
#define SCALE_4DS	6247		// 311.1270 Hz
#define SCALE_4E	6066		// 339.6276 Hz
#define SCALE_4F	5726		// 349.2282 Hz
#define SCALE_4FS	5404		// 369.9944 Hz
#define SCALE_4G 	5101		// 391.9954 Hz
#define SCALE_4GS	4815		// 415.3047 Hz
#define SCALE_4A	4544		// 440.0000 Hz
#define SCALE_4AS	4289		// 466.1638 Hz
#define SCALE_4B	4049		// 493.8833 Hz
#define SCALE_5C	3821		// 523.2511 Hz
#define SCALE_5CS	3607		// 554.3653 Hz
#define SCALE_5D	3404		// 587.3295 Hz
#define SCALE_5DS	3213		// 622.2540 Hz
#define SCALE_5E	3003		// 659.2551 Hz
#define SCALE_5F	2862		// 698.4565 Hz
#define SCALE_NO	2			// 1 MHz

#define BEAT_EGHT	24


// Variables
extern uint8_t g_deTimer2Flg;
extern uint32_t g_deTimer6Cnt;


// Structures
typedef struct
{
	float E_EepromRx;
	float E_EepromTx;
	float E_Lan1Rx;
	float E_Lan1Tx;
	float E_Lan2Rx;
	float E_Lan2Tx;
	float I_Spi4Rx;
	float I_Spi4Tx;
	float I_Uart3Rx;
	float I_Uart3Tx;
}
EffDataTrRate_t;
extern EffDataTrRate_t Edtr;


// Functions
void TIM_INIT(void);
void TIM2_WRITE(void);


#endif /* TIM_H_ */
