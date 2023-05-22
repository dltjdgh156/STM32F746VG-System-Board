//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	icd.c
//	MAKE	BY		:
//	DESCRIPTION		:	interface control document structures
//	CREATED DATE	:	0000.00.00
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"


// Defines



// Variables



// Structures
PM_MOD MOD_DATA;
PMtoCPU_t PM_UART;
THtoCPU_t TH_I2C;
CPUtoEEPROM_t CPU_I2C;
BitStatus_t BStatus;
BitAlarm_t BAlarm;
BitAlarm_t BAlarm_k;
BitError_t BError;
BitError_t BError_k;
BitDio_t BDio;
ParticleCount_t PCnt;
ParticleAlram_t PAlarm;
ParticleVersion_t PVersion;
//RealTimeData_t DNT_RTC;

// Functions

