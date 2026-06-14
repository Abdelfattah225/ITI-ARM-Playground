/******************************************************************************
 * @file    NVIC_private.h
 * @brief   NVIC Private Header File for STM32F401CC
 * @details Contains register definitions and addresses for the Nested 
 *          Vectored Interrupt Controller (NVIC) peripheral
 * @author  Abdeflattah Moawed
 * @date    2025
 ******************************************************************************/

#ifndef NVIC_PRIVATE_H
#define NVIC_PRIVATE_H

#include <stdint.h>

/******************************************************************************
 * NVIC Base Address
 ******************************************************************************/
#define NVIC_BASE_ADDRESS           0xE000E100UL

/******************************************************************************
 * NVIC Register Structure
 ******************************************************************************/
typedef struct {
    volatile uint32_t ISER[8];      /* Interrupt Set-Enable Registers      (0x100 - 0x11C) */
    uint32_t RESERVED0[24];         /* Reserved                            (0x120 - 0x17C) */
    volatile uint32_t ICER[8];      /* Interrupt Clear-Enable Registers    (0x180 - 0x19C) */
    uint32_t RESERVED1[24];         /* Reserved                            (0x1A0 - 0x1FC) */
    volatile uint32_t ISPR[8];      /* Interrupt Set-Pending Registers     (0x200 - 0x21C) */
    uint32_t RESERVED2[24];         /* Reserved                            (0x220 - 0x27C) */
    volatile uint32_t ICPR[8];      /* Interrupt Clear-Pending Registers   (0x280 - 0x29C) */
    uint32_t RESERVED3[24];         /* Reserved                            (0x2A0 - 0x2FC) */
    volatile uint32_t IABR[8];      /* Interrupt Active Bit Registers      (0x300 - 0x31C) */
    uint32_t RESERVED4[56];         /* Reserved                            (0x320 - 0x3FC) */
    volatile uint8_t  IPR[240];     /* Interrupt Priority Registers        (0x400 - 0x4EF) */
    uint32_t RESERVED5[644];
    volatile uint32_t STIR   ;      /* Interrupt Active Bit Registers      (0xE00 ) */

} NVIC_RegDef_t;

/******************************************************************************
 * NVIC Peripheral Pointer
 ******************************************************************************/
#define NVIC                        ((NVIC_RegDef_t*)NVIC_BASE_ADDRESS)

/******************************************************************************
 * STM32F401CC Interrupt Numbers (IRQn)
 ******************************************************************************/
typedef enum {
    /* Cortex-M4 Processor Exceptions */
    NONMASKABLEINT_IRQn         = -14,    /* Non Maskable Interrupt */
    HARDFAULT_IRQn              = -13,    /* Hard Fault Interrupt */
    MEMORYMANAGEMENT_IRQn       = -12,    /* Memory Management Interrupt */
    BUSFAULT_IRQn               = -11,    /* Bus Fault Interrupt */
    USAGEFAULT_IRQn             = -10,    /* Usage Fault Interrupt */
    SVCALL_IRQn                 = -5,     /* SV Call Interrupt */
    DEBUGMONITOR_IRQn           = -4,     /* Debug Monitor Interrupt */
    PENDSV_IRQn                 = -2,     /* Pend SV Interrupt */
    SYSTICK_IRQn                = -1,     /* System Tick Interrupt */

    /* STM32F401CC Specific Interrupts */
    WWDG_IRQn                   = 0,      /* Window Watchdog Interrupt */
    PVD_IRQn                    = 1,      /* PVD through EXTI Line detection */
    TAMP_STAMP_IRQn             = 2,      /* Tamper and TimeStamp interrupts through EXTI */
    RTC_WKUP_IRQn               = 3,      /* RTC Wakeup interrupt through EXTI */
    FLASH_IRQn                  = 4,      /* Flash global interrupt */
    RCC_IRQn                    = 5,      /* RCC global interrupt */
    EXTI0_IRQn                  = 6,      /* EXTI Line0 interrupt */
    EXTI1_IRQn                  = 7,      /* EXTI Line1 interrupt */
    EXTI2_IRQn                  = 8,      /* EXTI Line2 interrupt */
    EXTI3_IRQn                  = 9,      /* EXTI Line3 interrupt */
    EXTI4_IRQn                  = 10,     /* EXTI Line4 interrupt */
    DMA1_Stream0_IRQn           = 11,     /* DMA1 Stream0 global interrupt */
    DMA1_Stream1_IRQn           = 12,     /* DMA1 Stream1 global interrupt */
    DMA1_Stream2_IRQn           = 13,     /* DMA1 Stream2 global interrupt */
    DMA1_Stream3_IRQn           = 14,     /* DMA1 Stream3 global interrupt */
    DMA1_Stream4_IRQn           = 15,     /* DMA1 Stream4 global interrupt */
    DMA1_Stream5_IRQn           = 16,     /* DMA1 Stream5 global interrupt */
    DMA1_Stream6_IRQn           = 17,     /* DMA1 Stream6 global interrupt */
    ADC_IRQn                    = 18,     /* ADC1 global interrupt */
    EXTI9_5_IRQn                = 23,     /* EXTI Line[9:5] interrupts */
    TIM1_BRK_TIM9_IRQn          = 24,     /* TIM1 Break interrupt and TIM9 */
    TIM1_UP_TIM10_IRQn          = 25,     /* TIM1 Update interrupt and TIM10 */
    TIM1_TRG_COM_TIM11_IRQn     = 26,     /* TIM1 Trigger/Commutation and TIM11 */
    TIM1_CC_IRQn                = 27,     /* TIM1 Capture Compare interrupt */
    TIM2_IRQn                   = 28,     /* TIM2 global interrupt */
    TIM3_IRQn                   = 29,     /* TIM3 global interrupt */
    TIM4_IRQn                   = 30,     /* TIM4 global interrupt */
    I2C1_EV_IRQn                = 31,     /* I2C1 event interrupt */
    I2C1_ER_IRQn                = 32,     /* I2C1 error interrupt */
    I2C2_EV_IRQn                = 33,     /* I2C2 event interrupt */
    I2C2_ER_IRQn                = 34,     /* I2C2 error interrupt */
    SPI1_IRQn                   = 35,     /* SPI1 global interrupt */
    SPI2_IRQn                   = 36,     /* SPI2 global interrupt */
    USART1_IRQn                 = 37,     /* USART1 global interrupt */
    USART2_IRQn                 = 38,     /* USART2 global interrupt */
    EXTI15_10_IRQn              = 40,     /* EXTI Line[15:10] interrupts */
    RTC_Alarm_IRQn              = 41,     /* RTC Alarms (A and B) through EXTI */
    OTG_FS_WKUP_IRQn            = 42,     /* USB OTG FS Wakeup through EXTI */
    DMA1_Stream7_IRQn           = 47,     /* DMA1 Stream7 global interrupt */
    SDIO_IRQn                   = 49,     /* SDIO global interrupt */
    TIM5_IRQn                   = 50,     /* TIM5 global interrupt */
    SPI3_IRQn                   = 51,     /* SPI3 global interrupt */
    DMA2_Stream0_IRQn           = 56,     /* DMA2 Stream0 global interrupt */
    DMA2_Stream1_IRQn           = 57,     /* DMA2 Stream1 global interrupt */
    DMA2_Stream2_IRQn           = 58,     /* DMA2 Stream2 global interrupt */
    DMA2_Stream3_IRQn           = 59,     /* DMA2 Stream3 global interrupt */
    DMA2_Stream4_IRQn           = 60,     /* DMA2 Stream4 global interrupt */
    OTG_FS_IRQn                 = 67,     /* USB OTG FS global interrupt */
    DMA2_Stream5_IRQn           = 68,     /* DMA2 Stream5 global interrupt */
    DMA2_Stream6_IRQn           = 69,     /* DMA2 Stream6 global interrupt */
    DMA2_Stream7_IRQn           = 70,     /* DMA2 Stream7 global interrupt */
    USART6_IRQn                 = 71,     /* USART6 global interrupt */
    I2C3_EV_IRQn                = 72,     /* I2C3 event interrupt */
    I2C3_ER_IRQn                = 73,     /* I2C3 error interrupt */
    FPU_IRQn                    = 81,     /* FPU global interrupt */
    SPI4_IRQn                   = 84      /* SPI4 global interrupt */
} IRQn_Type;



/******************************************************************************
 * Priority Levels Enumeration
 ******************************************************************************/
typedef enum {
    NVIC_PRIORITY_0 = 0,    /* Highest priority */
    NVIC_PRIORITY_1,
    NVIC_PRIORITY_2,
    NVIC_PRIORITY_3,
    NVIC_PRIORITY_4,
    NVIC_PRIORITY_5,
    NVIC_PRIORITY_6,
    NVIC_PRIORITY_7,
    NVIC_PRIORITY_8,
    NVIC_PRIORITY_9,
    NVIC_PRIORITY_10,
    NVIC_PRIORITY_11,
    NVIC_PRIORITY_12,
    NVIC_PRIORITY_13,
    NVIC_PRIORITY_14,
    NVIC_PRIORITY_15        /* Lowest priority */
} NVIC_Priority_t;


typedef enum{
    NOTPENDING,
    PENDING
}Pending_state_t;

typedef enum{
    NOTACTIVE,
    ACTIVE
}Active_State_t;


/******************************************************************************
 * Helper Macros
 ******************************************************************************/
/* Priority Level Mask (only 4 bits implemented in STM32F4) */
#define NVIC_PRIORITY_MASK          0xF0

/* Maximum number of interrupts for STM32F401CC */
#define NVIC_MAX_IRQn               85
#define NVIC_MIN_IRQn               0


/******************************************************************************
 * Register Access Macros
 ******************************************************************************/
#define NVIC_ISER(n)                (NVIC->ISER[n])
#define NVIC_ICER(n)                (NVIC->ICER[n])
#define NVIC_ISPR(n)                (NVIC->ISPR[n])
#define NVIC_ICPR(n)                (NVIC->ICPR[n])
#define NVIC_IABR(n)                (NVIC->IABR[n])
#define NVIC_IPR(n)                 (NVIC->IPR[n])
#define NVIC_STIR                   (NVIC->STIR)
#endif /* NVIC_PRIVATE_H */