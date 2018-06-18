/******************************************************************************/
/**
* @file Usart1.c
* @brief Implement usart1
*
*******************************************************************************/

/* ***************** Header / include files ( #include ) **********************/

#include <stddef.h>

#include "Gpio.h"

#include "Usart1.h"

#if defined(SELECT_TORQUE) || defined(SELECT_PILOT)

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static uint16_t index = 0U;
static uint32_t TxPin = 0UL;
static uint32_t RxPin = 0UL;
static uint32_t Baud = 0UL;
static GPIO_TypeDef *pGPIO_USART = NULL;
/* *************** Modul global constants ( static const ) ********************/

/* **************** Local func/proc prototypes ( static ) *********************/
/******************************************************************************/
/**
* void Usart1Init(tBSPType)
* @brief Configure USART1 according to board type and initialze variables.
*         pin values for rx and tx are specified in respective BSP macros.
* @param[in] tBSPType The type of target to be configured according to BSP, Only serial
*              based bsp types handled.
*
*******************************************************************************/
void Usart1Init(tBSPType BSPType)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    switch(BSPType)
    {
        case BSP_Pilot:
            TxPin = BSP_PILOT_UART_TX_PIN;
            RxPin = BSP_PILOT_UART_RX_PIN;
            Baud  = BSP_PILOT_UART_BAUD;
            pGPIO_USART = BSP_PILOT_UART_PORT;
            break;

        case BSP_TorqueSensor:
        default:
            TxPin = BSP_TORQUE_UART_TX_PIN;
            RxPin = BSP_TORQUE_UART_RX_PIN;
            Baud  = BSP_TORQUE_UART_BAUD;
            pGPIO_USART = BSP_TORQUE_UART_PORT;
            break;
    }

    pGPIO_USART->AFR[TxPin >> 3] &= ~((uint32_t)MASK4 << (((uint32_t)TxPin & MASK3) << 2U));
    pGPIO_USART->AFR[TxPin >> 3] |= ((uint32_t)GPIO_AF_1 << (((uint32_t)TxPin & MASK3) << 2U));

    pGPIO_USART->AFR[RxPin >> 3] &= ~((uint32_t)MASK4 << (((uint32_t)RxPin & MASK3) << 2U));
    pGPIO_USART->AFR[RxPin >> 3] |= ((uint32_t)GPIO_AF_1 << (((uint32_t)RxPin & MASK3) << 2U));

    pGPIO_USART->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (TxPin << 1));
    pGPIO_USART->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (TxPin << 1));
    pGPIO_USART->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)TxPin));
    pGPIO_USART->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)TxPin));
    pGPIO_USART->MODER &= ~(GPIO_MODER_MODER0 << (TxPin << 1));
    pGPIO_USART->MODER |= ((uint32_t)GPIO_Mode_AF << (TxPin << 1));
    pGPIO_USART->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (TxPin << 1));
    pGPIO_USART->PUPDR |= ((uint32_t)GPIO_PuPd_UP << (TxPin << 1));

    pGPIO_USART->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (RxPin << 1));
    pGPIO_USART->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (RxPin << 1));
    pGPIO_USART->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)RxPin));
    pGPIO_USART->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)RxPin));
    pGPIO_USART->MODER &= ~(GPIO_MODER_MODER0 << (RxPin << 1));
    pGPIO_USART->MODER |= ((uint32_t)GPIO_Mode_AF << (RxPin << 1));
    pGPIO_USART->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (RxPin << 1));
    pGPIO_USART->PUPDR |= ((uint32_t)GPIO_PuPd_UP << (RxPin << 1));

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    USART1->BRR = __USART_BRR(SystemCoreClock, Baud);  
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;  // 8N1
}

/******************************************************************************/
/**
* void Usart1Send(uint8_t *pTxData, uint16_t size)
* @brief Implement usart1 send.
*
* @param[in] pTxData pointer to the data to be transmitted
* @param[in] size number of bytes
*
*******************************************************************************/
void Usart1Send(uint8_t *pTxData, const uint16_t size)
{
    uint16_t i = 0U;
    while(i < size)
    {
        while((USART1->ISR & USART_ISR_TXE) == 0);
        USART1->TDR = pTxData[i++];
    }
}

/******************************************************************************/
/**
* eFUNCTION_RETURN Usart1Recv(uint8_t *pRxData, uint16_t size)
*
* @brief Read from UART. 
*
* @param[out] pRxData pointer to 68 bytes data
* @param[in]  size number of bytes
* @returns    eFunction_Ok if successful
*             or
*             eFunction_Error if data losts.
*             or
*             eFunction_Timeout if an timeout error occurs.
*
*******************************************************************************/
eFUNCTION_RETURN Usart1Recv(uint8_t *pRxData, const uint16_t size)
{
    eFUNCTION_RETURN retVal = eFunction_Timeout;
    
    if(USART1->ISR & USART_ISR_RXNE)
    {
        pRxData[index] = USART1->RDR;
        index++;
    }
    
    if(index >= size)
    {
        index = 0;
        retVal = eFunction_Ok;
    }
    return retVal;
}

/******************************************************************************/
/**
* void Usart1Reset(void)
*
* @brief Reset receive pointer index
*
* @returns    none
*
*******************************************************************************/
inline void Usart1Reset(void)
{
    index = 0;
}

#endif // SELECT_TORQUE || SELECT_PILOT

