/******************************************************************************/
/**
* @file Flash.c
* @brief Implement Flash driver
*
*******************************************************************************/
/* ***************** Header / include files ( #include ) **********************/

#include <stddef.h>

#include <stm32f0xx.h>

#include "CRC.h"

#include "Flash.h"

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static tFlashLimits FlashSettings;
/******************************************************************************/
/**
* void FlashInit(void)
* @brief Set the flash addresses for the target board
* All boards other than Pilot have identical flash sizes.
* @param[in] BSPtype is passed to select the flash memory size
*
*******************************************************************************/
void FlashInit(tBSPType BSPType)
{
    if(BSPType == BSP_Pilot)
    {
        FlashSettings.CRCinFlash = BSP_ABSOLUTE_FLASH_END_16KB - 4UL;
        FlashSettings.LENinFlash = BSP_ABSOLUTE_FLASH_END_16KB - 2UL;
        FlashSettings.TOTALPages = BSP_APP_PROGRAM_PAGES_16KB;
    }else 
    {
        FlashSettings.CRCinFlash = BSP_ABSOLUTE_FLASH_END_32KB - 4UL;
        FlashSettings.LENinFlash = BSP_ABSOLUTE_FLASH_END_32KB - 2UL;
        FlashSettings.TOTALPages = BSP_APP_PROGRAM_PAGES_32KB;
    }
}

/******************************************************************************/
/**
* uint8_t FlashWrite(uint8_t* buf, uint16_t size)
* @brief Write to Flash and lock it afterwards.
*
* @param[in] buf pointer to data to be written to flash
* @param[in] size number of bytes
* @param[in] packet number
* @returns   1 if successful
*            or
*            0 if an error occurs.
*
*******************************************************************************/
uint8_t FlashWrite(uint8_t* buf, const uint16_t size, const uint16_t pktNo)
{
    uint16_t i = 0;
    uint32_t flashWait = BootTIMEOUT;
    uint16_t* p16 = (uint16_t *)(BSP_ABSOLUTE_APP_START + (pktNo * size));  
    /**
     *    Size should be a non zero number less than 1025 and should be a multiple
     *     of two since we write 2 bytes.
     */
    if((size > 1024UL) || (size == 0) || (buf == NULL))
    {
        return 0;
    }
    // Program Flash Page
    FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
    while(i < size) 
    {
        FLASH->CR |= FLASH_CR_PG;
        *p16++ = (uint16_t)(buf[i+1] << 8) | buf[i];
        /* Reload the busy wait timeout */
        flashWait = BootTIMEOUT;  
        while((FLASH->SR & FLASH_SR_BSY) != 0)
        {
            if(!(flashWait--))
            {
                /** Return if the busy wait timer expires */
                return 0;
            }
        }
        FLASH->CR &= ~FLASH_CR_PG;
        if((FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) != 0)
        {
            FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
            return 0;
        }
        i += sizeof(uint16_t);
    }
    /** Lets verify flash if we have written correctly */
    i = 0;
    p16 = (uint16_t *)(BSP_ABSOLUTE_APP_START + (pktNo * size));
    while(i < size) 
    {
        if(*p16++ != ((uint16_t)(buf[i+1] << 8) | buf[i]))
        {
            return 0;
        }
        i += sizeof(uint16_t);
    }
    return 1;
}

/******************************************************************************/
/**
* uint8_t FlashErase(void)
* @brief Erase Flash from start of application to the last page, each page is 1kB.
*
* @returns   1 if successful
*            or
*            0 if an error occurs.
*
*******************************************************************************/
uint8_t FlashErase(void)
{
    uint32_t flashWait = BootTIMEOUT;
    uint32_t flashAdr = (uint32_t)BSP_ABSOLUTE_APP_START;
    
    // Unlock Flash
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    while((FLASH->CR & FLASH_CR_LOCK) != 0)
    {
        if(!(flashWait--))
        {
            return 0;
        }
    }
    for(uint8_t i = 0; i < FlashSettings.TOTALPages; i++)
    {
        FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
        FLASH->CR |= FLASH_CR_PER;
        FLASH->AR = flashAdr;
        FLASH->CR |= FLASH_CR_STRT;
        /** Reload the busy wait time out */
        flashWait = BootTIMEOUT;
        while((FLASH->SR & FLASH_SR_BSY) != 0)
        {
            if(!(flashWait--))
            {
                return 0;
            }
        }
        FLASH->CR &= ~FLASH_CR_PER;
        if((FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) != 0)
        {
            FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
            return 0;
        }
        flashAdr = flashAdr + BSP_FLASH_PAGE_SIZE_BYTES;
    }
    return 1;
}

/******************************************************************************/
/**
* void FlashLock(void)
* @brief Lock flash.
*
*******************************************************************************/
void FlashLock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

/******************************************************************************/
/**
* uint8_t FlashWriteFWParam(tFIRMWARE_PARAM fwParam)
* @brief Write 4 bytes firmware parameters (i.e. FW crc and length) to a fixed 
*        Flash address.
*
* @param[in] fwParam firmware parameters to be written to flash
* @returns   1 if successful
*            or
*            0 if an error occurs.
*
*******************************************************************************/
uint8_t FlashWriteFWParam(tFIRMWARE_PARAM fwParam)
{
    uint32_t flashWait = BootTIMEOUT;
    uint16_t *ad = (uint16_t *)FlashSettings.CRCinFlash;
    FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
    /* Write FW CRC */
    FLASH->CR |= FLASH_CR_PG;
    *ad = (uint16_t)fwParam.u16FWCRC;
    while((FLASH->SR & FLASH_SR_BSY) != 0)
    {
        if(!(flashWait--))
        {
            return 0;
        }
    }
    FLASH->CR &= ~FLASH_CR_PG;
    if((FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) != 0)
    {
        FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
        return 0;
    }
    ad = (uint16_t *)FlashSettings.LENinFlash;
    /* Write FW length */
    FLASH->CR |= FLASH_CR_PG;
        flashWait = BootTIMEOUT;
    *ad = (uint16_t)fwParam.u16FWLen;
    while((FLASH->SR & FLASH_SR_BSY) != 0)
    {
        if(!(flashWait--))
        {
            return 0;
        }
    }
    FLASH->CR &= ~FLASH_CR_PG;
    if((FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) != 0)
    {
        FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
        return 0;
    }
        
    /** Now start verification immidiately */
    ad = (uint16_t*)FlashSettings.CRCinFlash;
    if(*ad != fwParam.u16FWCRC)
    {
        return 0;
    }
    ad = (uint16_t*)FlashSettings.LENinFlash;
    if(*ad != fwParam.u16FWLen)
    {
        return 0;
    }
    return 1;
}

/******************************************************************************/
/**
* uint8_t FlashVerifyFirmware(void)
* @brief Verify firmware in Flash by comparing the stored crc with the 
*        calculated crc.
*
* @returns   1 if matches
*            or
*            0 if doesn't match.
*
*******************************************************************************/
uint8_t FlashVerifyFirmware(void)
{
    volatile uint16_t i = 0;
    uint32_t temp32 = *(uint32_t *)FlashSettings.CRCinFlash;
    volatile const uint16_t lenFromHost = (uint16_t)(temp32 >> 16U);
    volatile const uint16_t crcFromHost = (uint16_t)(temp32 & 0x0000FFFFUL);
    uint16_t dataByte = 0;
    uint16_t CRCtemp = 0;
    uint16_t *fwar = (uint16_t*)BSP_ABSOLUTE_APP_START;
    /* Read from FLASH_CRC_LENGTH_ADDRESS the firmware crc and length from host */

    /** Check if the length is within flash range or the read flash will fail */
    if(lenFromHost > (FlashSettings.CRCinFlash - BSP_ABSOLUTE_APP_START))
    {
        return 0;
    }

    /* Calculate local crc */
    while(i < lenFromHost) 
    {
        /* Read from address of the firmware and calculate crc */
        dataByte = *fwar++;
        CRCtemp = CRCCalc16((uint8_t *)&dataByte, 2, CRCtemp);
        i += 2;
    }
    if(CRCtemp == crcFromHost)
    {
        return 1;
    }
    return 0;
}
