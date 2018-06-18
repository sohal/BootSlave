/******************************************************************************/
/**
* @file CRC.c
* @brief CRC16 checksum calculator
*
*******************************************************************************/
/* ***************** Header / include files ( #include ) **********************/

#include <stddef.h>

#include "CRC.h"

/* *************** Constant / macro definitions ( #define ) *******************/
#define CRC16 0xAA55
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
/* **************** Local func/proc prototypes ( static ) *********************/
/******************************************************************************/
/**
* uint16_t CRCCalc16(const uint8_t *data, uint16_t size, uint16_t startVal)
* @brief Generate 16 bits CRC for an input array.
*
* @param[in] data pointer to data array
* @param[in] size byte number of the array
* @param[in] startVal start value of the CRC calculation
* @returns   calculated 16 bits CRC.
*******************************************************************************/
uint16_t CRCCalc16(const uint8_t *data, uint16_t size, uint16_t startVal)
{
    uint16_t out = startVal;
    uint8_t bits_read = 0, bit_flag;
    /* Sanity check */
    if(data == NULL)
    {
        return 0;
    }
    while(size > 0)
    {
        bit_flag = out >> 15;
        /* Get next bit */
        out <<= 1;
        out |= (*data >> (7 - bits_read)) & 1;
        /* Increment bit counter */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }
        /* Cycle check */
        if(bit_flag)
        {
            out ^= CRC16;
        }
    }
    return out;
}
