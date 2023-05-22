//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	adcdac.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef ADCDAC_H_
#define ADCDAC_H_
// Includes



// Defines
#define P3V30_BIT12	0.00080586080586080586					// +3.30V / 4095bit
#define P1V65_BIT12	0.00040293040293040293					// +1.65V / 4095bit
#define BIT12_P3V30	1240.90909090909090909					// 4095bit / +3.30V
#define BIT12_P1V65	2481.81818181818181818					// 4095bit / +1.65V


// Variables



// Structures



// Functions
extern void ADCDAC_INIT(void);
extern void ADCDAC_READ(void);
extern void ADCDAC_WRITE(void);

#endif /* ADCDAC_H_ */
