/******************************************************************************
 * @file    SYSTICK_config.h
 * @brief   SysTick Configuration Header File for STM32F401CC
 * @details Contains user-configurable settings for the SysTick driver
 * @author  Abdelfattah Moawed
 * @date    2025
 ******************************************************************************/

#ifndef SYSTICK_CONFIG_H
#define SYSTICK_CONFIG_H

/******************************************************************************
 * Clock Source Configuration
 * Options:
 * - SYSTICK_CLK_AHB_DIV8: AHB clock divided by 8 (external clock)
 * - SYSTICK_CLK_AHB:      Processor clock (AHB)
 ******************************************************************************/
#define SYSTICK_CLOCK_SOURCE        SYSTICK_CLK_AHB

/******************************************************************************
 * Interrupt Mode Configuration
 * Options:
 * - SYSTICK_INT_DISABLE: Polling mode (no interrupt generated)
 * - SYSTICK_INT_ENABLE:  Interrupt mode (SysTick_Handler called on timeout)
 ******************************************************************************/
#define SYSTICK_INTERRUPT_MODE      SYSTICK_INT_ENABLE

/******************************************************************************
 * System Clock Frequency (in MHz)
 * Configure this based on your system clock settings
 * Common values for STM32F401CC:
 * - 84 MHz (Maximum frequency)
 * - 16 MHz (HSI default)
 * - 8 MHz  (HSE with 8MHz crystal)
 ******************************************************************************/
#define SYSTICK_AHB_FREQ_MHZ        84

/******************************************************************************
 * Auto-start Configuration
 * Set to 1 to enable SysTick automatically after initialization
 * Set to 0 to keep SysTick disabled (must call SYSTICK_Enable() manually)
 ******************************************************************************/
#define SYSTICK_AUTO_START          0

/******************************************************************************
 * Default Reload Value Configuration
 * This value is loaded during SYSTICK_Init()
 * - Range: 0x000001 to 0xFFFFFF (24-bit value)
 * - Default: 0 (must be set manually or use delay config functions)
 * 
 * Example calculations:
 * For 1ms tick with 84MHz AHB clock:
 *   SYSTICK_RELOAD_VALUE = (84,000,000 / 1,000) - 1 = 83,999
 * 
 * For 1ms tick with 84MHz AHB/8:
 *   SYSTICK_RELOAD_VALUE = (84,000,000 / 8 / 1,000) - 1 = 10,499
 ******************************************************************************/
#define SYSTICK_RELOAD_VALUE        0

/******************************************************************************
 * Delay Configuration
 * Enable pre-configured delay functions
 * Set to 1 to enable, 0 to disable
 ******************************************************************************/
#define SYSTICK_DELAY_ENABLE        1

#endif /* SYSTICK_CONFIG_H */