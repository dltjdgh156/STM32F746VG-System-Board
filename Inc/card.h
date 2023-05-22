//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	card.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef CARD_H_
#define CARD_H_
// Includes



// Defines



// Variables
extern uint8_t g_arCardRxBuf[8], g_arCardTxBuf[128];


// Structures



// Functions
extern void CARD_INIT(void);
extern void CARD_RECV(void);
extern void CARD_TRANS(void);


#endif /* CARD_H_ */
