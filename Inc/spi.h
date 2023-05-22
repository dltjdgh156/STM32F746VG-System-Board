//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	spi.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef SPI_H_
#define SPI_H_
// Includes



// Defines
#define SPI_BUSY			((SPI2->SR >> 7) & 1)
#define SPI_TXBUFF(x, y)	HAL_SPI_Transmit(&hspi2, x, y, 10)


// Variables



// Structures



// Functions
extern void SPI_INIT(void);
extern void SPI2_RECV(void);
extern void SPI2_TRANS(void);


#endif /* SPI_H_ */
