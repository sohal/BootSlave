/******************************************************************************/
/**
* @file Packet.h
* @brief Definition of the 68-byte data packet and its' status
*
*******************************************************************************/
#ifndef PACKET_H
#define PACKET_H

/* ***************** Header / include files ( #include ) **********************/
/* *************** Constant / macro definitions ( #define ) *******************/
#define BLOCK_SIZE 64

/* ********************* Type definitions ( typedef ) *************************/
/**
* @struct tDATA_PACKET
* @brief Packet includes 64-byte blocks of data plus two-byte sequence count and two-byte CRC
*/
typedef struct
{
    uint8_t  u8Data[BLOCK_SIZE];  /**< Data block     */
    uint16_t u16SeqCnt;           /**< Sequence count */
    uint16_t u16CRC;              /**< Two-byte CRC   */
}tDATA_PACKET;

/**
* @struct tFIRMWARE_PARAM
* @brief Two-byte CRC over the whole firmware and the length of firmware in bytes
*/
typedef struct
{
    uint16_t u16FWCRC;    /**< Two-byte CRC over firmware */
    uint16_t u16FWLen;    /**< Length of the firmware     */
}tFIRMWARE_PARAM;

/**
* @enum ePACKET_STATUS
* @brief Status of the data packet.
*/
typedef enum
{
    ePACKET_Ok           = 0,  /**< No error              */
    ePACKET_CRCError     = 1,  /**< CRC mismatch          */
    ePACKET_SNError      = 2   /**< Sequence number error */
}ePACKET_STATUS;

/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
void PacketInit(void);
ePACKET_STATUS PacketProcess(tDATA_PACKET packet);

#endif

/* end of Packet.h */
