
/******************************************************************************/
/**
* @file Protocol.c
* @brief Implement State machine for protocol handling of bootloader
*
*******************************************************************************/

/* ***************** Header / include files ( #include ) **********************/

#include "CRC.h"
#include "Flash.h"
#include "Protocol.h"

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static tCmdUnion         Command;
static tPldUnion         Payload;
static volatile uint32_t *AppVectorsInFlash = (volatile uint32_t *)BSP_ABSOLUTE_APP_START;
static volatile uint32_t *AppVectorsInRAM   = (volatile uint32_t *)BSP_ABSOLUTE_SRAM_START;
/* *************** Modul global constants ( static const ) ********************/
/* **************** Local func/proc prototypes ( static ) *********************/


/******************************************************************************/
/**
* eFUNCTION_RETURN ProtocolSM_Run(const tBSPStruct *pBSP)
* @brief     Main function to to bootload the new application from interface for
*            the selected BSP.
*
* @param[in] contant pointer to the BSP structure decided as per Hardware
* @returns   eBSP_OK (0) if all ok
*            or
*            error otherwise.
*
*******************************************************************************/
uint32_t ProtocolSM_Run(const tBSPStruct *pBSP)
{
    static tProtoState  stateNow, stateNext;
    uint16_t            crcCalculated = 0U;
    static uint32_t     tickCounter = 0U;
    static uint32_t     stickyTimer = 0U;
    eFlashError_t       eFlashError = eFlash_OK;
    eBSPError_t         retVal = eBSP_Busy;

    switch(stateNow)
    {
        case eDefaultState:
            if(eBSP_OK == pBSP->pRecv(Command.bufferCMD, 2U))
            {
                if(eCMD_BootloadMode == Command.receivedvalue)
                {
                    stateNext = eFlashEraseCMD;
                    tickCounter = 0;
                    Command.returnValue = eRES_Ready;
                    pBSP->pSend(Command.bufferCMD, 2U);
                }
            }else
            {
                if(tickCounter > pBSP->AppStartTicks)
                {
                    stateNext = eFlashVerifyApplication;
                    tickCounter = 0;
                }else
                {
                    tickCounter++;
                    stateNext = eDefaultState;
                }
            }
            break;

        case eFlashEraseCMD:
            if(eBSP_OK == pBSP->pRecv(Command.bufferCMD, 2U))
            {
                if(eCMD_EraseFlash == Command.receivedvalue)
                {
                    eFlashError = FlashErase();
                    if(eFlash_OK == eFlashError)
                    {
                        stateNext = eWriteMemory;
                        Command.returnValue = eRES_OK;
                    }else
                    {
                        Command.returnValue = eRES_Error;
                    }
                    pBSP->pSend(Command.bufferCMD, 2U);
                }
            }
            break;

        case eWriteMemory:
            if(eBSP_OK == pBSP->pRecv(Command.bufferCMD, 2U))
            {
                if(eCMD_WriteMemory == Command.receivedvalue)
                {
                    stateNext = ePayloadReceive;
                    Payload.packet.u16SeqCnt = 0xFFFFU;
                    Command.returnValue = eRES_OK;
                    pBSP->pSend(Command.bufferCMD, 2);
                    pBSP->pReset();
                }
            }
            break;

        case ePayloadReceive:
            if(eBSP_OK == pBSP->pRecv(Payload.bufferPLD, sizeof(tPldUnion)))
            {
                stateNext = ePayloadCheck;
            }
            break;

        case ePayloadCheck:
            crcCalculated = CRCCalc16(Payload.packet.u8Data, sizeof(tPldUnion) - 2U, 0);
            Command.returnValue = eRES_OK;
            stateNext = ePayloadReceive;
            if(crcCalculated == Payload.packet.u16CRC)
            {
                eFlashError = FlashWrite(Payload.bufferPLD, BLOCK_SIZE, Payload.packet.u16SeqCnt);
                if(eFlash_OK != eFlashError)
                {
                    Command.returnValue = eRES_Error;
                    if(eFlash_LastAddress == eFlashError)
                    {
                        Command.returnValue = eRES_OK;
                        stateNext = eFinishUpdate;
                    }
                }
            }else
            {
                pBSP->pReset();
                Command.returnValue = eRES_Error;
            }
            Payload.packet.u16SeqCnt = 0xFFFFU;
            Payload.packet.u16CRC = 0xFFFFU;
            crcCalculated = 0U;
            pBSP->pSend(Command.bufferCMD, 2);
            break;

        case eFinishUpdate:
            if(eBSP_OK == pBSP->pRecv(Command.bufferCMD, 2U))
            {
                if(eCMD_Finish == Command.receivedvalue)
                {
                    stateNext = eFlashVerifyApplication;
                }
            }
            break;

        case eFlashVerifyApplication:
            Command.returnValue = eRES_Abort;
            eFlashError = FlashVerifyFirmware();
            if(eFlash_OK == eFlashError)
            {
                Command.returnValue = eRES_OK;
                stateNext = eStartAppCMD;
            }else
            {
                Command.returnValue = eRES_AppCrcErr;
                stateNext = eDefaultState;
            }
            pBSP->pSend(Command.bufferCMD, 2);
            break;

        case eStartAppCMD:
            /** Busy wait for some time */
            tickCounter = pBSP->CommDoneTicks;
            do{
            }while(tickCounter--);

            /* Lock flash from further write */
            FlashLock();
            /* Remap Application Vectors */
            for(int i = 0; i < BSP_APP_VECTOR_SIZE_WORDS; i++)
            {
                AppVectorsInRAM[i] = AppVectorsInFlash[i];
            }
            /* Setup controller mode to consider vectors from RAM */
            RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
            SYSCFG->CFGR1 |= SYSCFG_CFGR1_MEM_MODE;

            /* Setup Application Stack Pointer */
            __set_MSP(*AppVectorsInFlash);
            /* Setup Program counter with the start of Application */
            ((void (*)(void))*(AppVectorsInFlash + 1UL))();
            break;

        default:
            /* We will never come here if all is OK, nevertheless we restore state machine to default state */
            stateNext = eDefaultState;
            break;
    }

    /* Check if the same state is repeating, no transition suggests a
     * hung state. We can count the stickyness of the software and reset
     * to a known state.
     */
    if(stateNext == stateNow)
    {
        stickyTimer++;
        /* If the timeout has expired, we reboot the system */
        if(stickyTimer > pBSP->BootTimeoutTicks)
        {
            stateNext = eDefaultState;
            stickyTimer = 0U;
        }
    }else
    {
        /* Reset the sticky counter if the state transition takes place */
        stickyTimer = 0U;
    }

    stateNow = stateNext;

    return((uint32_t)retVal);
}
