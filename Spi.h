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
eBSPError_t SpiInit(tBSPType BSPType);
eBSPError_t SpiSend(uint8_t *pTxData, uint16_t size);
eBSPError_t SpiReset(void);
eBSPError_t SpiRecv(uint8_t *pRxData, uint16_t size);

#endif // SPI_H_

#endif // SELECT_WATCHDOG
