#ifndef NVIC_INTERFACE
#define NVIC_INTERFACE

/******************************************************************************
 * @file    NVIC_interface.h
 * @brief   NVIC Interface Header File for STM32F401CC
 * @details Contains function prototypes and public definitions for NVIC driver
 * @author  Abdeflattah Moawed
 * @date    2025
 ******************************************************************************/

#include <stdint.h>
#include "../../private/MCAL/NVIC_Private.h"

/******************************************************************************
 * Function Return Types
 ******************************************************************************/
typedef enum {
    NVIC_OK = 0,
    NVIC_ERROR,
    NVIC_INVALID_IRQ,
    NVIC_INVALID_PRIORITY
} NVIC_ErrorStatus_t;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief  Enable a specific interrupt in the NVIC
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_EnableIRQ(IRQn_Type IRQn);

/**
 * @brief  Disable a specific interrupt in the NVIC
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_DisableIRQ(IRQn_Type IRQn);

/**
 * @brief  Set pending status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_SetPendingIRQ(IRQn_Type IRQn);

/**
 * @brief  Clear pending status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_ClearPendingIRQ(IRQn_Type IRQn);

/**
 * @brief  Get pending status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval uint8_t: 1 if interrupt is pending, 0 otherwise
 */
uint8_t NVIC_GetPendingIRQ(IRQn_Type IRQn);

/**
 * @brief  Get active status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval uint8_t: 1 if interrupt is active, 0 otherwise
 */
uint8_t NVIC_GetActiveIRQ(IRQn_Type IRQn);

/**
 * @brief  Set priority for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @param  priority: Priority value (0-15, where 0 is highest priority)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_SetPriority(IRQn_Type IRQn, NVIC_Priority_t priority);

/**
 * @brief  Get priority for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval uint8_t: Priority value (0-15)
 */
NVIC_Priority_t NVIC_GetPriority(IRQn_Type IRQn);

/**
 * @brief  Initialize NVIC with configuration from NVIC_config.h
 * @param  None
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_Init(void);

/**
 * @brief  Trigger a software interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 * @note   This is equivalent to NVIC_SetPendingIRQ
 */
NVIC_ErrorStatus_t NVIC_TriggerSWI(IRQn_Type IRQn);

#endif /* NVIC_INTERFACE_H */