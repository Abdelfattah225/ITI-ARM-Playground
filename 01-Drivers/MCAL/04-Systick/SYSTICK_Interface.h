/******************************************************************************
 * @file    SYSTICK_interface.h
 * @brief   SysTick Interface Header File for STM32F401CC
 * @details Contains function prototypes and public definitions for SysTick driver
 * @author  Abdelfattah Moawed
 * @date    2025
 ******************************************************************************/

#ifndef SYSTICK_INTERFACE_H
#define SYSTICK_INTERFACE_H

#include <stdint.h>
#include "../../configuration/MCAL/SYSTICK_Cfg.h"
/******************************************************************************
 * Clock Source Selection
 ******************************************************************************/
typedef enum {
    SYSTICK_CLK_AHB_DIV8 = 0,   /* AHB clock divided by 8 */
    SYSTICK_CLK_AHB              /* Processor clock (AHB) */
} SYSTICK_ClkSource_t;

/******************************************************************************
 * Interrupt Mode Selection
 ******************************************************************************/
typedef enum {
    SYSTICK_INT_DISABLE = 0,    /* Polling mode - no interrupt */
    SYSTICK_INT_ENABLE          /* Interrupt mode */
} SYSTICK_IntMode_t;

/******************************************************************************
 * Function Return Types
 ******************************************************************************/
typedef enum {
    SYSTICK_OK = 0,
    SYSTICK_ERROR,
    SYSTICK_INVALID_RELOAD
} SYSTICK_ErrorStatus_t;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief  Initialize SysTick timer with configuration
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_Init(void);

/**
 * @brief  Set reload value for SysTick timer
 * @param  reloadValue: 24-bit reload value (0x000001 to 0xFFFFFF)
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_SetReloadValue(uint32_t reloadValue);

/**
 * @brief  Get current reload value
 * @param  None
 * @retval uint32_t: Current reload value
 */
uint32_t SYSTICK_GetReloadValue(void);

/**
 * @brief  Get current counter value
 * @param  None
 * @retval uint32_t: Current counter value
 */
uint32_t SYSTICK_GetCurrentValue(void);

/**
 * @brief  Set clock source for SysTick
 * @param  clkSource: Clock source selection (SYSTICK_ClkSource_t)
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_SetClockSource(SYSTICK_ClkSource_t clkSource);

/**
 * @brief  Enable SysTick timer
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_Enable(void);

/**
 * @brief  Disable SysTick timer
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_Disable(void);

/**
 * @brief  Enable SysTick interrupt
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_EnableInterrupt(void);

/**
 * @brief  Disable SysTick interrupt
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_DisableInterrupt(void);

/**
 * @brief  Check if SysTick counter has counted to zero
 * @param  None
 * @retval uint8_t: 1 if counted to zero, 0 otherwise
 * @note   Reading this flag clears it automatically
 */
uint8_t SYSTICK_GetCountFlag(void);

/**
 * @brief  Reset current value register (restarts counting)
 * @param  None
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_Reset(void);

/**
 * @brief  Configure SysTick for delay in microseconds
 * @param  microSeconds: Delay time in microseconds
 * @param  ahbFreqMHz: AHB clock frequency in MHz
 * @param  clkSource: Clock source selection
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_ConfigDelay_us(uint32_t microSeconds, uint32_t ahbFreqMHz, SYSTICK_ClkSource_t clkSource);

/**
 * @brief  Configure SysTick for delay in milliseconds
 * @param  milliSeconds: Delay time in milliseconds
 * @param  ahbFreqMHz: AHB clock frequency in MHz
 * @param  clkSource: Clock source selection
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_ConfigDelay_ms(uint32_t milliSeconds, uint32_t ahbFreqMHz, SYSTICK_ClkSource_t clkSource);

/**
 * @brief  Blocking delay in microseconds
 * @param  microSeconds: Delay time in microseconds
 * @retval None
 * @note   SysTick must be configured before calling this function
 */
void SYSTICK_Delay_us(uint32_t microSeconds);

/**
 * @brief  Blocking delay in milliseconds
 * @param  milliSeconds: Delay time in milliseconds
 * @retval None
 * @note   SysTick must be configured before calling this function
 */
void SYSTICK_Delay_ms(uint32_t milliSeconds);

/**
 * @brief  Set callback function for SysTick interrupt
 * @param  callbackFunc: Pointer to callback function
 * @retval SYSTICK_ErrorStatus_t: Status of the operation
 */
SYSTICK_ErrorStatus_t SYSTICK_SetCallback(void (*callbackFunc)(void));

#endif /* SYSTICK_INTERFACE_H */