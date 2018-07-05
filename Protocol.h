
/******************************************************************************/
/**
* @file Protocol.h
* @brief Header file for state machine for bootloader protocol
*
*******************************************************************************/
#ifndef PROTOCOL_H
#define PROTOCOL_H
/* ***************** Header / include files ( #include ) **********************/

#include <stdint.h>

#include "BSP.h"
#include "Common.h"
#include "Command.h"
#include "Packet.h"

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
typedef union myCmd{
    uint8_t         bufferCMD[4];
    eRESPONSE_ID    returnValue;
    eCOMMAND_ID     receivedvalue;
}tCmdUnion;

typedef union myPayload{
    tDATA_PACKET    packet;
    uint8_t         bufferPLD[68];
}tPldUnion;

typedef enum {
    eDefaultState = 0,
    eFlashEraseCMD,
    eWriteMemory,
    ePayloadReceive,
    ePayloadCheck,
    eFinishUpdate,
    eFlashVerifyApplication,
    eStartAppCMD
}tProtoState;
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
uint32_t ProtocolSM_Run(const tBSPStruct *);

#endif
