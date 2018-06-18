/******************************************************************************/
/**
* @file Spi.h
* @brief Implement SPI1
*
*******************************************************************************/

#if defined(SELECT_WATCHDOG)

#ifndef SPI_H
#define SPI_H

/* ***************** Header / include files ( #include ) **********************/
#include <stm32f0xx.h>

#include "BSP.h"
#include "Common.h"

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
void SpiInit(tBSPType BSPType);
void SpiSend(uint8_t *pTxData, uint16_t size);
void SpiReset(void);
eFUNCTION_RETURN SpiRecv(uint8_t *pRxData, uint16_t size);

#endif // SPI_H_

#endif // SELECT_WATCHDOG
