/******************************************************************************/
/**
* @file CRC.h
* @brief CRC16 checksum calculator
*
*******************************************************************************/
#ifndef CRC_H
#define CRC_H
/* ***************** Header / include files ( #include ) **********************/

#include <stdint.h>

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
uint16_t CRCCalc16(const uint8_t *data, uint16_t size, uint16_t startVal);

#endif

/* end of CRC.h */
