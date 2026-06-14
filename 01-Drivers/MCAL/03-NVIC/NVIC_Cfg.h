/******************************************************************************
 * @file    NVIC_config.h
 * @brief   NVIC Configuration Header File for STM32F401CC
 * @details Contains user-configurable settings for the NVIC driver
 * @author  Abdeflattah Moawed
 * @date    2025
 ******************************************************************************/

#ifndef NVIC_CONFIG_H
#define NVIC_CONFIG_H

/******************************************************************************
 * Priority Grouping Configuration
 * Choose one of the following options:
 * 
 * NVIC_PRIORITY_GROUP_0: 0 bits preemption, 4 bits subpriority (0-15)
 * NVIC_PRIORITY_GROUP_1: 1 bit  preemption, 3 bits subpriority (0-1, 0-7)
 * NVIC_PRIORITY_GROUP_2: 2 bits preemption, 2 bits subpriority (0-3, 0-3)
 * NVIC_PRIORITY_GROUP_3: 3 bits preemption, 1 bit  subpriority (0-7, 0-1)
 * NVIC_PRIORITY_GROUP_4: 4 bits preemption, 0 bits subpriority (0-15)
 * 
 * Note: This setting is typically configured once at system initialization
 ******************************************************************************/
#define NVIC_PRIORITY_GROUPING      NVIC_PRIORITY_GROUP_4

/******************************************************************************
 * Interrupt Priority Configuration
 * Configure the priority for each interrupt you plan to use
 * Priority values range from 0 (highest) to 15 (lowest)
 * 
 * Format: #define IRQ_NAME_PRIORITY  value
 ******************************************************************************/

/* External Interrupt Priorities */
#define EXTI0_PRIORITY              0
#define EXTI1_PRIORITY              1
#define EXTI2_PRIORITY              2
#define EXTI3_PRIORITY              3
#define EXTI4_PRIORITY              4
#define EXTI9_5_PRIORITY            5
#define EXTI15_10_PRIORITY          6

/* Timer Interrupt Priorities */
#define TIM1_BRK_TIM9_PRIORITY      7
#define TIM1_UP_TIM10_PRIORITY      7
#define TIM1_TRG_COM_TIM11_PRIORITY 7
#define TIM1_CC_PRIORITY            7
#define TIM2_PRIORITY               8
#define TIM3_PRIORITY               8
#define TIM4_PRIORITY               8
#define TIM5_PRIORITY               8

/* USART Interrupt Priorities */
#define USART1_PRIORITY             9
#define USART2_PRIORITY             9
#define USART6_PRIORITY             9

/* SPI Interrupt Priorities */
#define SPI1_PRIORITY               10
#define SPI2_PRIORITY               10
#define SPI3_PRIORITY               10
#define SPI4_PRIORITY               10

/* I2C Interrupt Priorities */
#define I2C1_EV_PRIORITY            11
#define I2C1_ER_PRIORITY            11
#define I2C2_EV_PRIORITY            11
#define I2C2_ER_PRIORITY            11
#define I2C3_EV_PRIORITY            11
#define I2C3_ER_PRIORITY            11

/* DMA Interrupt Priorities */
#define DMA1_Stream0_PRIORITY       12
#define DMA1_Stream1_PRIORITY       12
#define DMA1_Stream2_PRIORITY       12
#define DMA1_Stream3_PRIORITY       12
#define DMA1_Stream4_PRIORITY       12
#define DMA1_Stream5_PRIORITY       12
#define DMA1_Stream6_PRIORITY       12
#define DMA1_Stream7_PRIORITY       12
#define DMA2_Stream0_PRIORITY       12
#define DMA2_Stream1_PRIORITY       12
#define DMA2_Stream2_PRIORITY       12
#define DMA2_Stream3_PRIORITY       12
#define DMA2_Stream4_PRIORITY       12
#define DMA2_Stream5_PRIORITY       12
#define DMA2_Stream6_PRIORITY       12
#define DMA2_Stream7_PRIORITY       12

/* ADC Interrupt Priority */
#define ADC_PRIORITY                13

/* USB OTG Interrupt Priorities */
#define OTG_FS_PRIORITY             14
#define OTG_FS_WKUP_PRIORITY        14

/* SDIO Interrupt Priority */
#define SDIO_PRIORITY               13

/* System Interrupt Priorities */
#define WWDG_PRIORITY               15
#define PVD_PRIORITY                14
#define TAMP_STAMP_PRIORITY         14
#define RTC_WKUP_PRIORITY           14
#define RTC_Alarm_PRIORITY          14
#define FLASH_PRIORITY              13
#define RCC_PRIORITY                13
#define FPU_PRIORITY                15

/******************************************************************************
 * Interrupt Enable/Disable Configuration
 * Set to 1 to enable interrupt at initialization, 0 to disable
 * 
 * Note: You can also enable/disable interrupts at runtime using the API
 ******************************************************************************/

/* External Interrupts */
#define EXTI0_ENABLE                1
#define EXTI1_ENABLE                0
#define EXTI2_ENABLE                0
#define EXTI3_ENABLE                0
#define EXTI4_ENABLE                0
#define EXTI9_5_ENABLE              0
#define EXTI15_10_ENABLE            0

/* Timer Interrupts */
#define TIM1_BRK_TIM9_ENABLE        0
#define TIM1_UP_TIM10_ENABLE        0
#define TIM1_TRG_COM_TIM11_ENABLE   0
#define TIM1_CC_ENABLE              0
#define TIM2_ENABLE                 0
#define TIM3_ENABLE                 0
#define TIM4_ENABLE                 0
#define TIM5_ENABLE                 0

/* USART Interrupts */
#define USART1_ENABLE               0
#define USART2_ENABLE               0
#define USART6_ENABLE               0

/* SPI Interrupts */
#define SPI1_ENABLE                 0
#define SPI2_ENABLE                 0
#define SPI3_ENABLE                 0
#define SPI4_ENABLE                 0

/* I2C Interrupts */
#define I2C1_EV_ENABLE              0
#define I2C1_ER_ENABLE              0
#define I2C2_EV_ENABLE              0
#define I2C2_ER_ENABLE              0
#define I2C3_EV_ENABLE              0
#define I2C3_ER_ENABLE              0

/* DMA Interrupts */
#define DMA1_Stream0_ENABLE         0
#define DMA1_Stream1_ENABLE         0
#define DMA1_Stream2_ENABLE         0
#define DMA1_Stream3_ENABLE         0
#define DMA1_Stream4_ENABLE         0
#define DMA1_Stream5_ENABLE         0
#define DMA1_Stream6_ENABLE         0
#define DMA1_Stream7_ENABLE         0
#define DMA2_Stream0_ENABLE         0
#define DMA2_Stream1_ENABLE         0
#define DMA2_Stream2_ENABLE         0
#define DMA2_Stream3_ENABLE         0
#define DMA2_Stream4_ENABLE         0
#define DMA2_Stream5_ENABLE         0
#define DMA2_Stream6_ENABLE         0
#define DMA2_Stream7_ENABLE         0

/* Other Interrupts */
#define ADC_ENABLE                  0
#define OTG_FS_ENABLE               0
#define SDIO_ENABLE                 0

#endif /* NVIC_CONFIG_H */