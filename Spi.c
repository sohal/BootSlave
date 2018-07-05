/******************************************************************************/
/**
* @file Spi.c
* @brief Implement SPI1
*
*******************************************************************************/

/* ***************** Header / include files ( #include ) **********************/
#include "Spi.h"
#include "Gpio.h"
#if defined (SELECT_WATCHDOG)
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static uint16_t index = 0U;
/* *************** Modul global constants ( static const ) ********************/
/* **************** Local func/proc prototypes ( static ) *********************/
/******************************************************************************/
/**
* eBSPError_t SpiInit(tBSPType)
* @brief Configure SPI1 (STM32F031:PA4(NSS),PA7(MOSI),PA6(MISO),PA5(SCK)) and
*        initialze variables.
* @param[in] tBSPType The type of target to be configured according to BSP
* @returns eBSP_OK 
*
*******************************************************************************/
eBSPError_t SpiInit(tBSPType BSPType)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOBEN;

    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_CLK_PIN >> 3U] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_CLK_PIN) & (uint32_t)0x07) << 2));
    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_CLK_PIN >> 3U] |=  ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_CLK_PIN) & (uint32_t)0x07) << 2));

    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MISO_PIN >> 3U] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MISO_PIN) & (uint32_t)0x07) << 2));
    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MISO_PIN >> 3U] |=  ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MISO_PIN) & (uint32_t)0x07) << 2));

    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MOSI_PIN >> 3U] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MOSI_PIN) & (uint32_t)0x07) << 2));
    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MOSI_PIN >> 3U] |=  ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MOSI_PIN) & (uint32_t)0x07) << 2));

    BSP_TARGET_SPI_NSS_PORT->AFR[BSP_TARGET_SPI_NSS_PIN >> 3U] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_NSS_PIN) & (uint32_t)0x07) << 2));
    BSP_TARGET_SPI_NSS_PORT->AFR[BSP_TARGET_SPI_NSS_PIN >> 3U] |=  ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_NSS_PIN) & (uint32_t)0x07) << 2));

    BSP_TARGET_SPI_PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (BSP_TARGET_SPI_CLK_PIN << 1));
    BSP_TARGET_SPI_PORT->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (BSP_TARGET_SPI_CLK_PIN << 1));

    BSP_TARGET_SPI_PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (BSP_TARGET_SPI_MISO_PIN << 1));
    BSP_TARGET_SPI_PORT->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (BSP_TARGET_SPI_MISO_PIN << 1));

    BSP_TARGET_SPI_PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (BSP_TARGET_SPI_MOSI_PIN << 1));
    BSP_TARGET_SPI_PORT->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (BSP_TARGET_SPI_MOSI_PIN << 1));
        
    BSP_TARGET_SPI_NSS_PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (BSP_TARGET_SPI_NSS_PIN << 1));
    BSP_TARGET_SPI_NSS_PORT->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (BSP_TARGET_SPI_NSS_PIN << 1));
        
    BSP_TARGET_SPI_PORT->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)BSP_TARGET_SPI_CLK_PIN));
    BSP_TARGET_SPI_PORT->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)BSP_TARGET_SPI_CLK_PIN));

    BSP_TARGET_SPI_PORT->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)BSP_TARGET_SPI_MISO_PIN));
    BSP_TARGET_SPI_PORT->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)BSP_TARGET_SPI_MISO_PIN));

    BSP_TARGET_SPI_PORT->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)BSP_TARGET_SPI_MOSI_PIN));
    BSP_TARGET_SPI_PORT->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)BSP_TARGET_SPI_MOSI_PIN));

    BSP_TARGET_SPI_NSS_PORT->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)BSP_TARGET_SPI_NSS_PIN));
    BSP_TARGET_SPI_NSS_PORT->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)BSP_TARGET_SPI_NSS_PIN));
        
    BSP_TARGET_SPI_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_TARGET_SPI_CLK_PIN << 1));
    BSP_TARGET_SPI_PORT->MODER |= ((uint32_t)GPIO_Mode_AF << (BSP_TARGET_SPI_CLK_PIN << 1));

    BSP_TARGET_SPI_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_TARGET_SPI_MISO_PIN << 1));
    BSP_TARGET_SPI_PORT->MODER |= ((uint32_t)GPIO_Mode_AF << (BSP_TARGET_SPI_MISO_PIN << 1));

    BSP_TARGET_SPI_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_TARGET_SPI_MOSI_PIN << 1));
    BSP_TARGET_SPI_PORT->MODER |= ((uint32_t)GPIO_Mode_AF << (BSP_TARGET_SPI_MOSI_PIN << 1));
        
    BSP_TARGET_SPI_NSS_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_TARGET_SPI_NSS_PIN << 1));
    BSP_TARGET_SPI_NSS_PORT->MODER |= ((uint32_t)GPIO_Mode_AF << (BSP_TARGET_SPI_NSS_PIN << 1));

    BSP_TARGET_SPI_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_TARGET_SPI_CLK_PIN << 1));
    BSP_TARGET_SPI_PORT->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (BSP_TARGET_SPI_CLK_PIN << 1));

    BSP_TARGET_SPI_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_TARGET_SPI_MISO_PIN << 1));
    BSP_TARGET_SPI_PORT->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (BSP_TARGET_SPI_MISO_PIN << 1));

    BSP_TARGET_SPI_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_TARGET_SPI_MOSI_PIN << 1));
    BSP_TARGET_SPI_PORT->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (BSP_TARGET_SPI_MOSI_PIN << 1));
        
    BSP_TARGET_SPI_NSS_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_TARGET_SPI_NSS_PIN << 1));
    BSP_TARGET_SPI_NSS_PORT->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (BSP_TARGET_SPI_NSS_PIN << 1));

    /*! Enable SPI1 peripheral */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    /*! SPI Slave event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
     *  SPI has 8 bit data word length 
     */
    SPI1->CR2 = SPI_CR2_FRXTH |
                SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; 
    /*! SPI peripheral is set in slave mode */ 
    SPI1->CR1 = SPI_CR1_SPE |  /*! SPI is enabled in the hardware module */
                SPI_CR1_CPOL;  /*! Clock polarity is set */

    return(eBSP_OK);
}

/******************************************************************************/
/**
* void SpiSend(uint8_t *pTxData, uint16_t size)
* @brief Implement SPI1 send. 1s timeout if nothing is received for transmitting.
*
* @param[in] pTxData pointer to the data to be transmitted
* @param[in] size number of bytes
* @returns   eFunction_Ok if successful
*            or
*            eFunction_Timeout if an timeout error occurs.
*
*******************************************************************************/
eBSPError_t SpiSend(uint8_t *pTxData, uint16_t size)
{
    volatile uint16_t   tmp16;
    uint32_t            bspWait = BootTIMEOUT;
    uint16_t            i = 0U;
    
    while (i < size)
    {
        /* Reload the busy wait timeout */
        bspWait = BootTIMEOUT;
        while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE)
        {
            if(!(bspWait--))
            {
                /** Return if the busy wait timer expires */
                return(eBSP_XmitTimeOut);
            }
        }
        *(volatile uint8_t *)&(SPI1->DR) = pTxData[i++];
        if(SPI1->SR & SPI_SR_OVR)
        {
            tmp16 = SPI1->DR;
            tmp16 = SPI1->SR;
            (void)tmp16;
        }
    }
    return(eBSP_OK);
}

/******************************************************************************/
/**
* eBSPError_t SpiRecv(uint8_t *pRxData, uint16_t size)
*
* @brief Read from SPI1.
*
* @param[out] pRxData pointer to 68 bytes data
* @param[in]  size number of bytes
* @returns    eBSP_OK if successful
*             or
*             eBSP_RecvTimeOut if timeout error occurs.
*             eBSP_Busy if the polling is ongoing for new data on interface
*
*******************************************************************************/
eBSPError_t SpiRecv(uint8_t *pRxData, uint16_t size)
{
    volatile uint16_t   tmp;
    static uint16_t     timeout = 0U;

    if((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
    {
        pRxData[index] = (uint8_t)SPI1->DR;
        index++;
        timeout = 0U;
    }
    else
    {
        if(size > 2U)
        {
            timeout++;
            if(timeout > 0x1FFU)
            {
                timeout = 0;
                SpiReset();
                return(eBSP_RecvTimeOut);
            }
        }
    }

    if(index >= size)
    {
        index = 0U;
        while((SPI1->SR & SPI_SR_OVR) == SPI_SR_OVR)
        {
            tmp = SPI1->DR;
            tmp = SPI1->SR;
            (void)tmp;
        }
        return(eBSP_OK);
    }
    return(eBSP_Busy);
}

/******************************************************************************/
/**
* eBSPError_t SpiReset(void)
*
* @brief Reset receive pointer index
*
* @returns    eBSP_OK if successful
*
*******************************************************************************/
eBSPError_t SpiReset(void)
{
    index = 0;
    volatile uint16_t tmp;
    
    while((SPI1->SR & SPI_SR_OVR) == SPI_SR_OVR)
    {
        tmp = SPI1->DR;
        tmp = SPI1->SR;
        (void)tmp;
    }
    return(eBSP_OK);
}


#endif // defined SELECT_WATCHDOG

