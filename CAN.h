/******************************************************************************/
/**
* @file Can.h
* @brief Implement CAN
*
*******************************************************************************/

#if defined(SELECT_CAN)

#ifndef CAN_H_
#define CAN_H_

/* ***************** Header / include files ( #include ) **********************/

#include <stdint.h>

#include <stm32f0xx.h>

#include "BSP.h"
#include "Common.h"

/* *************** Constant / macro definitions ( #define ) *******************/
#define CAN_MAX_DATA_LENGTH        (8U)  // Maximum number of bytes in CAN bus (4H + 4L)
/* ********************* Type definitions ( typedef ) *************************/
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
typedef union {
    uint32_t    Word[2];
    uint8_t     Byte[8];
}tCANData;

void CanInit(tBSPType BSPType);
void CanSend(uint8_t *pTxData, uint16_t size);
void CanReset(void);
eFUNCTION_RETURN CanRecv(uint8_t *pRxData, uint16_t size);

#endif // CAN_H_

#endif // SELECT_CAN
