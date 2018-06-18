/******************************************************************************/
/**
* @file Spi.c
* @brief Implement SPI1
*
*******************************************************************************/

/* ***************** Header / include files ( #include ) **********************/
#include "Spi.h"
#include "Gpio.h"
//#include "Timeout.h"
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
* void SpiInit(tBSPType)
* @brief Configure SPI1 (STM32F031:PA4(NSS),PA7(MOSI),PA6(MISO),PA5(SCK)) and
*        initialze variables.
* @param[in] tBSPType The type of target to be configured according to BSP
*
*******************************************************************************/
void SpiInit(tBSPType BSPType)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_CLK_PIN >> 3U] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_CLK_PIN) & (uint32_t)0x07) << 2));
    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_CLK_PIN >> 3U] |=  ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_CLK_PIN) & (uint32_t)0x07) << 2));

    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MISO_PIN >> 3U] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MISO_PIN) & (uint32_t)0x07) << 2));
    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MISO_PIN >> 3U] |=  ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MISO_PIN) & (uint32_t)0x07) << 2));

    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MOSI_PIN >> 3U] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MOSI_PIN) & (uint32_t)0x07) << 2));
    BSP_TARGET_SPI_PORT->AFR[BSP_TARGET_SPI_MOSI_PIN >> 3U] |=  ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)BSP_TARGET_SPI_MOSI_PIN) & (uint32_t)0x07) << 2));

    BSP_TARGET_SPI_PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (BSP_TARGET_SPI_CLK_PIN << 1));
    BSP_TARGET_SPI_PORT->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (BSP_TARGET_SPI_CLK_PIN << 1));

    BSP_TARGET_SPI_PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (BSP_TARGET_SPI_MISO_PIN << 1));
    BSP_TARGET_SPI_PORT->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (BSP_TARGET_SPI_MISO_PIN << 1));

    BSP_TARGET_SPI_PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (BSP_TARGET_SPI_MOSI_PIN << 1));
    BSP_TARGET_SPI_PORT->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (BSP_TARGET_SPI_MOSI_PIN << 1));

    BSP_TARGET_SPI_PORT->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)BSP_TARGET_SPI_CLK_PIN));
    BSP_TARGET_SPI_PORT->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)BSP_TARGET_SPI_CLK_PIN));

    BSP_TARGET_SPI_PORT->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)BSP_TARGET_SPI_MISO_PIN));
    BSP_TARGET_SPI_PORT->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)BSP_TARGET_SPI_MISO_PIN));

    BSP_TARGET_SPI_PORT->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)BSP_TARGET_SPI_MOSI_PIN));
    BSP_TARGET_SPI_PORT->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)BSP_TARGET_SPI_MOSI_PIN));

    BSP_TARGET_SPI_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_TARGET_SPI_CLK_PIN << 1));
    BSP_TARGET_SPI_PORT->MODER |= ((uint32_t)GPIO_Mode_AF << (BSP_TARGET_SPI_CLK_PIN << 1));

    BSP_TARGET_SPI_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_TARGET_SPI_MISO_PIN << 1));
    BSP_TARGET_SPI_PORT->MODER |= ((uint32_t)GPIO_Mode_AF << (BSP_TARGET_SPI_MISO_PIN << 1));

    BSP_TARGET_SPI_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_TARGET_SPI_MOSI_PIN << 1));
    BSP_TARGET_SPI_PORT->MODER |= ((uint32_t)GPIO_Mode_AF << (BSP_TARGET_SPI_MOSI_PIN << 1));

    BSP_TARGET_SPI_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_TARGET_SPI_CLK_PIN << 1));
    BSP_TARGET_SPI_PORT->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (BSP_TARGET_SPI_CLK_PIN << 1));

    BSP_TARGET_SPI_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_TARGET_SPI_MISO_PIN << 1));
    BSP_TARGET_SPI_PORT->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (BSP_TARGET_SPI_MISO_PIN << 1));

    BSP_TARGET_SPI_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_TARGET_SPI_MOSI_PIN << 1));
    BSP_TARGET_SPI_PORT->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (BSP_TARGET_SPI_MOSI_PIN << 1));

    /*! Enable SPI1 peripheral */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    /*! SPI Slave event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
     *  SPI has 8 bit data word length 
     */
    SPI1->CR2 = SPI_CR2_FRXTH | 
                SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; 
    /*! SPI peripheral is set in slave mode */ 
    SPI1->CR1 = SPI_CR1_SPE | /*! SPI is enabled in the hardware module */
                SPI_CR1_SSM | /*! Software management is selected for Chip Select */
                SPI_CR1_SSI;  /*! Chip select is active low */
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
void SpiSend(uint8_t *pTxData, uint16_t size)
{
    uint16_t i = 0U;
    volatile uint8_t tmp;
    
    while (size)
    {
        SPI1->DR = ((uint16_t)pTxData[0] << 8U) | pTxData[1];
        /*! The Software controlled Chip select is made active (low), master clock can
         *  can now be used to send data out from the LFSR
         */
        SPI1->CR1 &= ~SPI_CR1_SSI;
        /*! Check if the transmit FIFO is empty */
        /** @ todo add a timeout here */
        while((SPI1->SR & SPI_SR_FTLVL) != 0);
        /*! Check if the SPI peripheral is busy*/
        /** @ todo add a timeout here */
        while((SPI1->SR & SPI_SR_BSY));
        /*! Do dummy read from receive FIFO until empty */
        while((SPI1->SR & SPI_SR_FRLVL))
        {
            tmp = SPI1->DR;
            (void)tmp;
        }
        size -= 2U;
    }
    /*! The Software controlled Chip select is de-activated (high) */
    SPI1->CR1 |= SPI_CR1_SSI;
}

/******************************************************************************/
/**
* eFUNCTION_RETURN SpiRecv(uint8_t *pRxData, uint16_t size)
*
* @brief Read from SPI1.
*
* @param[out] pRxData pointer to 68 bytes data
* @param[in]  size number of bytes
* @returns    eFunction_Ok if successful
*             or
*             eFunction_Error if timeout error occurs.
*
*******************************************************************************/
eFUNCTION_RETURN SpiRecv(uint8_t *pRxData, uint16_t size)
{

    eFUNCTION_RETURN retVal = eFunction_Timeout;
    /*! The Software controlled Chip select is made active (low), master clock can
     *  can now be used to send data out from the LFSR
     */
    SPI1->CR1 &= ~SPI_CR1_SSI;
    if(SPI1->SR & SPI_SR_RXNE)
    {
        pRxData[index] = SPI1->DR;
        index++;
    }

    if(index >= size)
    {
        index = 0;
        /*! The Software controlled Chip select is de-activated (high) */
        SPI1->CR1 |= SPI_CR1_SSI;
        retVal = eFunction_Ok;
    }

    return retVal;
}

/******************************************************************************/
/**
* void SpiReset(void)
*
* @brief Reset receive pointer index
*
* @returns    none
*
*******************************************************************************/
inline void SpiReset(void)
{
    index = 0;
}


#endif // defined SELECT_WATCHDOG

