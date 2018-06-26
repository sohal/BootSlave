/******************************************************************************/
/**
* @file Flash.h
* @brief Implement Flash driver
*
*******************************************************************************/
#ifndef FLASH_H
#define FLASH_H

/* ***************** Header / include files ( #include ) **********************/

#include <stdint.h>

#include "BSP.h"
#include "Packet.h"

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
typedef struct myFlash{
    uint32_t    CRCinFlash;
    uint32_t    LENinFlash;
    uint32_t    TOTALPages;
}tFlashLimits;

typedef enum flasherrors{
		eFlash_OK,
		eFlash_AddressError,
		eFlash_LastAddress,
		eFlash_WriteTimeOut,
		eFlash_WriteError,
		eFlash_ReadError,
		eFlash_EraseError
}eFlashError_t;
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
/*******************************************************************************/
void FlashInit(tBSPType BSPType);
eFlashError_t FlashWrite(uint8_t* buf, const uint16_t size, const uint16_t pktNo);
eFlashError_t FlashErase(void);
void FlashLock(void);
eFlashError_t FlashWriteFWParam(tFIRMWARE_PARAM fwParam);
eFlashError_t FlashVerifyFirmware(void);

#endif

/* end of Flash.h */
