/******************************************************************************
 * @file    SYSTICK_private.h
 * @brief   SysTick Private Header File for STM32F401CC
 * @details Contains register definitions and addresses for the SysTick Timer
 * @author  Abdelfattah Moawed
 * @date    2025
 ******************************************************************************/

#ifndef SYSTICK_PRIVATE_H
#define SYSTICK_PRIVATE_H

#include <stdint.h>

/******************************************************************************
 * SysTick Base Address
 ******************************************************************************/
#define SYSTICK_BASE_ADDRESS        0xE000E010UL

/******************************************************************************
 * SysTick Register Structure
 ******************************************************************************/
typedef struct {
    volatile uint32_t CTRL;     /* SysTick Control and Status Register    (0x00) */
    volatile uint32_t LOAD;     /* SysTick Reload Value Register          (0x04) */
    volatile uint32_t VAL;      /* SysTick Current Value Register         (0x08) */
    volatile uint32_t CALIB;    /* SysTick Calibration Value Register     (0x0C) */
} SYSTICK_RegDef_t;

/******************************************************************************
 * SysTick Peripheral Pointer
 ******************************************************************************/
#define SYSTICK                     ((SYSTICK_RegDef_t*)SYSTICK_BASE_ADDRESS)

/******************************************************************************
 * CTRL Register Bit Definitions
 ******************************************************************************/
#define SYSTICK_CTRL_ENABLE_POS     0
#define SYSTICK_CTRL_ENABLE         (1UL << SYSTICK_CTRL_ENABLE_POS)

#define SYSTICK_CTRL_TICKINT_POS    1
#define SYSTICK_CTRL_TICKINT        (1UL << SYSTICK_CTRL_TICKINT_POS)

#define SYSTICK_CTRL_CLKSOURCE_POS  2
#define SYSTICK_CTRL_CLKSOURCE      (1UL << SYSTICK_CTRL_CLKSOURCE_POS)

#define SYSTICK_CTRL_COUNTFLAG_POS  16
#define SYSTICK_CTRL_COUNTFLAG      (1UL << SYSTICK_CTRL_COUNTFLAG_POS)

/******************************************************************************
 * LOAD Register Bit Definitions
 ******************************************************************************/
#define SYSTICK_LOAD_RELOAD_MASK    0x00FFFFFFUL
#define SYSTICK_LOAD_MAX_VALUE      0x00FFFFFFUL

/******************************************************************************
 * VAL Register Bit Definitions
 ******************************************************************************/
#define SYSTICK_VAL_CURRENT_MASK    0x00FFFFFFUL

/******************************************************************************
 * CALIB Register Bit Definitions
 ******************************************************************************/
#define SYSTICK_CALIB_TENMS_MASK    0x00FFFFFFUL

#define SYSTICK_CALIB_SKEW_POS      30
#define SYSTICK_CALIB_SKEW          (1UL << SYSTICK_CALIB_SKEW_POS)

#define SYSTICK_CALIB_NOREF_POS     31
#define SYSTICK_CALIB_NOREF         (1UL << SYSTICK_CALIB_NOREF_POS)

/******************************************************************************
 * Clock Source Definitions
 ******************************************************************************/
#define SYSTICK_CLKSOURCE_AHB_DIV8  0   /* AHB/8 */
#define SYSTICK_CLKSOURCE_AHB       1   /* AHB */

/******************************************************************************
 * Helper Macros
 ******************************************************************************/
#define SYSTICK_CTRL_REG            (SYSTICK->CTRL)
#define SYSTICK_LOAD_REG            (SYSTICK->LOAD)
#define SYSTICK_VAL_REG             (SYSTICK->VAL)
#define SYSTICK_CALIB_REG           (SYSTICK->CALIB)

#endif /* SYSTICK_PRIVATE_H */