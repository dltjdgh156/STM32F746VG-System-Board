//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	i2c.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef I2C_H_
#define I2C_H_
// Includes



// Defines



// Variables
extern uint8_t g_deI2C1ConnFlg;
extern uint8_t g_deI2C4ConnFlg, g_deI2C4ConnCnt;
extern uint8_t g_deI2C1ResetFlg, g_deI21ClearFlg;

extern uint8_t g_arI2c1TxBuf[1280];
// Structures



// Functions
extern void I2C_INIT(void);					// Initialize Inter-integrated Circuit
extern void I2C1_READ(void);
extern void I2C1_WRITE(void);
extern void I2C4_READ(void);
extern void I2C4_WRITE(void);


#endif /* I2C_H_ */
