//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	uart.h
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


#ifndef UART_H_
#define UART_H_
// Includes



// Defines



// Variables
extern uint8_t g_arUart1RxBuf[64], g_arUart1TxBuf[64];
extern uint8_t g_arUart2RxBuf[64], g_arUart2TxBuf[8];
extern uint8_t g_arUart7RxBuf[32], g_arUart7TxBuf[128];
extern uint8_t g_deUart1ConnFlg, g_deUart2ConnFlg, g_deUart7ConnFlg;
extern uint8_t g_deUart1ConnCnt, g_deUart2ConnCnt, g_deUart7ConnCnt;
extern uint8_t g_deUart2ConnErrCnt, g_deUart2ConnErrRef;
extern uint16_t g_deUart1RxCpltCnt, g_deUart1TxCpltCnt;
extern uint16_t g_deUart2RxCpltCnt, g_deUart2TxCpltCnt;


// Structures



// Functions
extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
extern void UART_INIT(void);
extern void UART1_RECV(void);
extern void UART1_TRANS(void);
extern void UART2_RECV(void);
extern void UART2_TRANS(void);
extern void UART7_RECV(void);
extern void UART7_TRANS(void);

#endif /* UART_H_ */
