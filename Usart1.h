/******************************************************************************/
/**
* @file Usart1.h
* @brief Implement usart1
*
*******************************************************************************/

#if defined(SELECT_TORQUE) || defined(SELECT_PILOT)

#ifndef USART1_H_
#define USART1_H_

/* ***************** Header / include files ( #include ) **********************/

#include <stm32f0xx.h>

#include "BSP.h"
#include "Common.h"

/* *************** Constant / macro definitions ( #define ) *******************/
#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

/* ********************* Type definitions ( typedef ) *************************/
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
void Usart1Init(tBSPType BSPType);
void Usart1Send(uint8_t *pTxData, uint16_t size);
void Usart1Reset(void);
eFUNCTION_RETURN Usart1Recv(uint8_t *pRxData, uint16_t size);

#endif // USART1_H_

#endif // SELECT_TORQUE || SELECT_PILOT
