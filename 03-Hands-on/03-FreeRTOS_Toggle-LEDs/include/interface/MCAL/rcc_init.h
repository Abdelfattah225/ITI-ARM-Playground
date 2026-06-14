/******************************************************************************
 * RCC_interface.h
 * RCC Driver Interface for STM32F401CC
 * @author  Abdelfattah moawed & commented by AI
 * 2025
 *****************************************************************************/

#ifndef RCC_INIT_H
#define RCC_INIT_H
#include <stdint.h>

#include "../../configuration/MCAL/rcc_cfg.h"

/* System Clock Source Options */
typedef enum
{
    RCC_SYSCLK_HSI = 0,     // High Speed Internal (16 MHz)
    RCC_SYSCLK_HSE,         // High Speed External (4-26 MHz)
    RCC_SYSCLK_PLL          // Phase-Locked Loop
}RCC_SysClk_t;

/* Clock Status */
typedef enum {
    RCC_NOT_READY = 0,
    RCC_READY
} RCC_ClkStatus_t;


/* HSE Bypass Mode */
typedef enum {
    RCC_HSE_CRYSTAL = 0,    // HSE crystal oscillator
    RCC_HSE_EXTERNAL_CLOCK  // HSE external clock (Bypass mode) // CHANGED
} RCC_HSE_Mode_t;

/* AHB Prescaler */
typedef enum {
    RCC_AHB_PRESCALER_1 = 0,
    RCC_AHB_PRESCALER_2 = 8,
    RCC_AHB_PRESCALER_4,
    RCC_AHB_PRESCALER_8,
    RCC_AHB_PRESCALER_16,
    RCC_AHB_PRESCALER_64,
    RCC_AHB_PRESCALER_128,
    RCC_AHB_PRESCALER_256,
    RCC_AHB_PRESCALER_512
} RCC_AHB_Prescaler_t;


/* APB Prescaler */
typedef enum {
    RCC_APB_PRESCALER_1 = 0,
    RCC_APB_PRESCALER_2 = 4,
    RCC_APB_PRESCALER_4,
    RCC_APB_PRESCALER_8,
    RCC_APB_PRESCALER_16
} RCC_APB_Prescaler_t;


/* PLL Clock Source */
typedef enum {
    RCC_PLL_SRC_HSI = 0,
    RCC_PLL_SRC_HSE
} RCC_PLL_Source_t;


/* PLL P Division Factor */
typedef enum {
    RCC_PLL_P_2 = 0,
    RCC_PLL_P_4,
    RCC_PLL_P_6,
    RCC_PLL_P_8
} RCC_PLL_P_t;



/* PLL Configuration Structure */
typedef struct {
    RCC_PLL_Source_t PLL_Source;    // HSI or HSE
    uint8_t PLLM;                   // Division factor for PLL input (2-63)
    uint16_t PLLN;                  // Multiplication factor for VCO (50-432)
    RCC_PLL_P_t PLLP;               // Division factor for main system clock
    uint8_t PLLQ;                   // Division factor for USB OTG FS, SDIO (2-15)
} RCC_PLL_Config_t;


/* AHB1 Peripherals */
typedef enum {
    RCC_AHB1_GPIOA = 0,
    RCC_AHB1_GPIOB,
    RCC_AHB1_GPIOC,
    RCC_AHB1_GPIOD,
    RCC_AHB1_GPIOE,
    RCC_AHB1_GPIOH = 7,
    RCC_AHB1_CRC = 12,
    RCC_AHB1_DMA1 = 21,
    RCC_AHB1_DMA2
} RCC_AHB1_Peripheral_t;



/* AHB2 Peripherals */
typedef enum {
    RCC_AHB2_OTGFS = 7
} RCC_AHB2_Peripheral_t;



/* APB1 Peripherals */
typedef enum {
    RCC_APB1_TIM2 = 0,
    RCC_APB1_TIM3,
    RCC_APB1_TIM4,
    RCC_APB1_TIM5,
    RCC_APB1_WWDG = 11,
    RCC_APB1_SPI2 = 14,
    RCC_APB1_SPI3,
    RCC_APB1_USART2 = 17,
    RCC_APB1_I2C1 = 21,
    RCC_APB1_I2C2,
    RCC_APB1_I2C3,
    RCC_APB1_PWR = 28
} RCC_APB1_Peripheral_t;



/* APB2 Peripherals */
typedef enum {
    RCC_APB2_TIM1 = 0,
    RCC_APB2_USART1 = 4,
    RCC_APB2_USART6,
    RCC_APB2_ADC1 = 8,
    RCC_APB2_SDIO = 11,
    RCC_APB2_SPI1,
    RCC_APB2_SPI4,
    RCC_APB2_SYSCFG,
    RCC_APB2_TIM9 = 16,
    RCC_APB2_TIM10,
    RCC_APB2_TIM11
} RCC_APB2_Peripheral_t;


/* Error Codes */
typedef enum {
    RCC_OK = 0,
    RCC_ERROR,
    RCC_TIMEOUT,
    RCC_INVALID_PARAM
} RCC_Status_t;





/******************************************************************************
 * Function Prototypes - System Clock Configuration
 *****************************************************************************/

/**
 *Initializes the system clock to a specified frequency using PLL
 *None
 *RCC_Status_t: Operation status
 *This function configures the system to 84 MHz using HSE (if available)
 */
RCC_Status_t RCC_Init(void);


/**
 * Enables the HSI (High Speed Internal) clock
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_HSI_Enable(void);


/**
 * Disables the HSI clock
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_HSI_Disable(void);


/**
 * Enables the HSE (High Speed External) clock
 * Mode: Crystal or Bypass mode
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_HSE_Enable(RCC_HSE_Mode_t Mode);

/*
 * Disables the HSE clock
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_HSE_Disable(void);


/**
 *  Configures and enables the PLL
 *  Config: Pointer to PLL configuration structure
 *  RCC_Status_t: Operation status
 */
RCC_Status_t RCC_PLL_Config(const RCC_PLL_Config_t* Config);

/**
 * Enables the PLL
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_PLL_Enable(void);

/**
 * Disables the PLL
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_PLL_Disable(void);


/**
 * Selects the system clock source
 * ClkSource: Clock source (HSI, HSE, or PLL)
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_SetSystemClock(RCC_SysClk_t ClkSource);


/**
 *Checks if a clock source is ready
 *ClkSource: Clock source to check
 *RCC_ClkStatus_t: Clock ready status
 */
RCC_ClkStatus_t RCC_GetClockStatus(RCC_SysClk_t ClkSource);


/**
 * Gets the current system clock source
 * None
 * RCC_SysClk_t: Current system clock source
 */
RCC_SysClk_t RCC_GetSystemClock(void);

/**
 * Checks if a clock source is ready
 * ClkSource: Clock source to check
 * RCC_ClkStatus_t: Clock ready status
 */
RCC_ClkStatus_t RCC_GetClockStatus(RCC_SysClk_t ClkSource);


/******************************************************************************
 * Function Prototypes - Bus Prescalers
 *****************************************************************************/

/**
 * Sets the AHB prescaler
 * Prescaler: AHB prescaler value
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_SetAHBPrescaler(RCC_AHB_Prescaler_t Prescaler);

/**
 * Sets the APB1 prescaler (Low-speed APB)
 * Prescaler: APB1 prescaler value
 * RCC_Status_t: Operation status
 * APB1 max frequency is 42 MHz
 */
RCC_Status_t RCC_SetAPB1Prescaler(RCC_APB_Prescaler_t Prescaler);

/**
 * Sets the APB2 prescaler (High-speed APB)
 * Prescaler: APB2 prescaler value
 * RCC_Status_t: Operation status
 * APB2 max frequency is 84 MHz
 */
RCC_Status_t RCC_SetAPB2Prescaler(RCC_APB_Prescaler_t Prescaler);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (AHB1)
 *****************************************************************************/




/**
 * @brief  Enables clock for an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to enable
 * @retval None
 */
void RCC_AHB1_EnableClock(RCC_AHB1_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to disable
 * @retval None
 */
void RCC_AHB1_DisableClock(RCC_AHB1_Peripheral_t Peripheral);

/**
 * @brief  Resets an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to reset
 * @retval None
 */
void RCC_AHB1_ResetPeripheral(RCC_AHB1_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (AHB2)
 *****************************************************************************/

/**
 * @brief  Enables clock for an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to enable
 * @retval None
 */
void RCC_AHB2_EnableClock(RCC_AHB2_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to disable
 * @retval None
 */
void RCC_AHB2_DisableClock(RCC_AHB2_Peripheral_t Peripheral);

/**
 * @brief  Resets an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to reset
 * @retval None
 */
void RCC_AHB2_ResetPeripheral(RCC_AHB2_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (APB1)
 *****************************************************************************/

/**
 * @brief  Enables clock for an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to enable
 * @retval None
 */
void RCC_APB1_EnableClock(RCC_APB1_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to disable
 * @retval None
 */
void RCC_APB1_DisableClock(RCC_APB1_Peripheral_t Peripheral);

/**
 * @brief  Resets an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to reset
 * @retval None
 */
void RCC_APB1_ResetPeripheral(RCC_APB1_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (APB2)
 *****************************************************************************/

/**
 * @brief  Enables clock for an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to enable
 * @retval None
 */
void RCC_APB2_EnableClock(RCC_APB2_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to disable
 * @retval None
 */
void RCC_APB2_DisableClock(RCC_APB2_Peripheral_t Peripheral);

/**
 * @brief  Resets an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to reset
 * @retval None
 */
void RCC_APB2_ResetPeripheral(RCC_APB2_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Clock Frequency Information
 *****************************************************************************/

/**
 * @brief  Gets the system clock frequency in Hz
 * @param  None
 * @retval uint32_t: System clock frequency
 */
uint32_t RCC_GetSYSCLKFrequency(void);

/**
 * @brief  Gets the AHB clock frequency in Hz
 * @param  None
/**
 * @brief  Enables clock for an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to enable
 * @retval None
 */
void RCC_AHB1_EnableClock(RCC_AHB1_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to disable
 * @retval None
 */
void RCC_AHB1_DisableClock(RCC_AHB1_Peripheral_t Peripheral);

/**
 * @brief  Resets an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to reset
 * @retval None
 */
void RCC_AHB1_ResetPeripheral(RCC_AHB1_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (AHB2)
 *****************************************************************************/

/**
 * @brief  Enables clock for an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to enable
 * @retval None
 */
void RCC_AHB2_EnableClock(RCC_AHB2_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to disable
 * @retval None
 */
void RCC_AHB2_DisableClock(RCC_AHB2_Peripheral_t Peripheral);

/**
 * @brief  Resets an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to reset
 * @retval None
 */
void RCC_AHB2_ResetPeripheral(RCC_AHB2_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (APB1)
 *****************************************************************************/

/**
 * @brief  Enables clock for an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to enable
 * @retval None
 */
void RCC_APB1_EnableClock(RCC_APB1_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to disable
 * @retval None
 */
void RCC_APB1_DisableClock(RCC_APB1_Peripheral_t Peripheral);

/**
 * @brief  Resets an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to reset
 * @retval None
 */
void RCC_APB1_ResetPeripheral(RCC_APB1_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (APB2)
 *****************************************************************************/

/**
 * @brief  Enables clock for an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to enable
 * @retval None
 */
void RCC_APB2_EnableClock(RCC_APB2_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to disable
 * @retval None
 */
void RCC_APB2_DisableClock(RCC_APB2_Peripheral_t Peripheral);

/**
 * @brief  Resets an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to reset
 * @retval None
 */
void RCC_APB2_ResetPeripheral(RCC_APB2_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Clock Frequency Information
 *****************************************************************************/

/**
 * Gets the system clock frequency in Hz
 * None
 * uint32_t: System clock frequency
 */
uint32_t RCC_GetSYSCLKFrequency(void);

/**
 * Gets the AHB clock frequency in Hz
 * None
 * uint32_t: AHB clock frequency
 */
uint32_t RCC_GetHCLKFrequency(void);

/**
 * Gets the APB1 clock frequency in Hz
 * None
 * uint32_t: APB1 clock frequency
 */
uint32_t RCC_GetPCLK1Frequency(void);

/**
 * Gets the APB2 clock frequency in Hz
 * None
 * uint32_t: APB2 clock frequency
 */
uint32_t RCC_GetPCLK2Frequency(void);

/**
 * Gets the APB1 timer clock frequency in Hz
 * None
 * uint32_t: APB1 timer clock frequency
 * Timer clocks are 2x APB clock when APB prescaler != 1
 */
uint32_t RCC_GetTIM_APB1_Frequency(void);

/**
 * Gets the APB2 timer clock frequency in Hz
 * None
 * uint32_t: APB2 timer clock frequency
 * Timer clocks are 2x APB clock when APB prescaler != 1
 */
uint32_t RCC_GetTIM_APB2_Frequency(void);

/******************************************************************************
 * Function Prototypes - CSS and Security
 *****************************************************************************/

/**
 * Enables the Clock Security System (CSS)
 * None
 * None
 * CSS monitors the HSE clock and switches to HSI if HSE fails
 */
void RCC_CSS_Enable(void);

/**
 * Disables the Clock Security System (CSS)
 * None
 * None
 */
void RCC_CSS_Disable(void);

/******************************************************************************
 * Function Prototypes - MCO (Microcontroller Clock Output)
 *****************************************************************************/

/**
 * Configures MCO1 (PA8) to output a clock signal
 * ClkSource: Clock source to output (0:HSI, 1:LSE, 2:HSE, 3:PLL)
 * Prescaler: Output prescaler (0:No div, 4:/2, 5:/3, 6:/4, 7:/5)
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_MCO1_Config(uint8_t ClkSource, uint8_t Prescaler);

/**
 *  Configures MCO2 (PC9) to output a clock signal
 *  ClkSource: Clock source to output (0:SYSCLK, 1:PLLI2S, 2:HSE, 3:PLL)
 *  Prescaler: Output prescaler (0:No div, 4:/2, 5:/3, 6:/4, 7:/5)
 *  RCC_Status_t: Operation status
 */
RCC_Status_t RCC_MCO2_Config(uint8_t ClkSource, uint8_t Prescaler);

/******************************************************************************
 * Function Prototypes - LSE and RTC
 *****************************************************************************/

/**
 * Enables the LSE (Low Speed External) oscillator
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSE_Enable(void);

/**
 *  Disables the LSE oscillator
 *  None
 *  RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSE_Disable(void);

/**
 * Enables the LSI (Low Speed Internal) oscillator
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSI_Enable(void);

/**
 * Disables the LSI oscillator
 * None
 * RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSI_Disable(void);

/**
 *  Configures the RTC clock source
 *  ClkSource: RTC clock source (0:No clock, 1:LSE, 2:LSI, 3:HSE)
 *  RCC_Status_t: Operation status
 */
RCC_Status_t RCC_RTC_SetClockSource(uint8_t ClkSource);

/**
 *  Enables the RTC clock
 *  None
 *  None
 */
void RCC_RTC_EnableClock(void);

/**
 * Disables the RTC clock
 * None
 * None
 */
void RCC_RTC_DisableClock(void);

/******************************************************************************
 * Function Prototypes - Backup Domain and Reset Flags
 *****************************************************************************/

/**
 *  Resets the Backup Domain (RTC registers and RTC clock)
 *  None
 *  None
 */
void RCC_BackupDomain_Reset(void);

/**
 *  Clears all reset flags
 *  None
 *  None
 */
void RCC_ClearResetFlags(void);
/**
 *  Enables clock for an AHB1 peripheral
 *  Peripheral: AHB1 peripheral to enable
 *  None
 */
void RCC_AHB1_EnableClock(RCC_AHB1_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to disable
 * @retval None
 */
void RCC_AHB1_DisableClock(RCC_AHB1_Peripheral_t Peripheral);

/**
 * @brief  Resets an AHB1 peripheral
 * @param  Peripheral: AHB1 peripheral to reset
 * @retval None
 */
void RCC_AHB1_ResetPeripheral(RCC_AHB1_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (AHB2)
 *****************************************************************************/

/**
 * @brief  Enables clock for an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to enable
 * @retval None
 */
void RCC_AHB2_EnableClock(RCC_AHB2_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to disable
 * @retval None
 */
void RCC_AHB2_DisableClock(RCC_AHB2_Peripheral_t Peripheral);

/**
 * @brief  Resets an AHB2 peripheral
 * @param  Peripheral: AHB2 peripheral to reset
 * @retval None
 */
void RCC_AHB2_ResetPeripheral(RCC_AHB2_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (APB1)
 *****************************************************************************/

/**
 * @brief  Enables clock for an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to enable
 * @retval None
 */
void RCC_APB1_EnableClock(RCC_APB1_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to disable
 * @retval None
 */
void RCC_APB1_DisableClock(RCC_APB1_Peripheral_t Peripheral);

/**
 * @brief  Resets an APB1 peripheral
 * @param  Peripheral: APB1 peripheral to reset
 * @retval None
 */
void RCC_APB1_ResetPeripheral(RCC_APB1_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Peripheral Clock Control (APB2)
 *****************************************************************************/

/**
 * @brief  Enables clock for an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to enable
 * @retval None
 */
void RCC_APB2_EnableClock(RCC_APB2_Peripheral_t Peripheral);

/**
 * @brief  Disables clock for an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to disable
 * @retval None
 */
void RCC_APB2_DisableClock(RCC_APB2_Peripheral_t Peripheral);

/**
 * @brief  Resets an APB2 peripheral
 * @param  Peripheral: APB2 peripheral to reset
 * @retval None
 */
void RCC_APB2_ResetPeripheral(RCC_APB2_Peripheral_t Peripheral);

/******************************************************************************
 * Function Prototypes - Clock Frequency Information
 *****************************************************************************/

/**
 * @brief  Gets the system clock frequency in Hz
 * @param  None
 * @retval uint32_t: System clock frequency
 */
uint32_t RCC_GetSYSCLKFrequency(void);

/**
 * @brief  Gets the AHB clock frequency in Hz
 * @param  None
 * @retval uint32_t: AHB clock frequency
 */
uint32_t RCC_GetHCLKFrequency(void);

/**
 * @brief  Gets the APB1 clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB1 clock frequency
 */
uint32_t RCC_GetPCLK1Frequency(void);

/**
 * @brief  Gets the APB2 clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB2 clock frequency
 */
uint32_t RCC_GetPCLK2Frequency(void);

/**
 * @brief  Gets the APB1 timer clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB1 timer clock frequency
 * @note   Timer clocks are 2x APB clock when APB prescaler != 1
 */
uint32_t RCC_GetTIM_APB1_Frequency(void);

/**
 * @brief  Gets the APB2 timer clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB2 timer clock frequency
 * @note   Timer clocks are 2x APB clock when APB prescaler != 1
 */
uint32_t RCC_GetTIM_APB2_Frequency(void);

/******************************************************************************
 * Function Prototypes - CSS and Security
 *****************************************************************************/

/**
 * @brief  Enables the Clock Security System (CSS)
 * @param  None
 * @retval None
 * @note   CSS monitors the HSE clock and switches to HSI if HSE fails
 */
void RCC_CSS_Enable(void);

/**
 * @brief  Disables the Clock Security System (CSS)
 * @param  None
 * @retval None
 */
void RCC_CSS_Disable(void);

/******************************************************************************
 * Function Prototypes - MCO (Microcontroller Clock Output)
 *****************************************************************************/

/**
 * @brief  Configures MCO1 (PA8) to output a clock signal
 * @param  ClkSource: Clock source to output (0:HSI, 1:LSE, 2:HSE, 3:PLL)
 * @param  Prescaler: Output prescaler (0:No div, 4:/2, 5:/3, 6:/4, 7:/5)
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_MCO1_Config(uint8_t ClkSource, uint8_t Prescaler);

/**
 * @brief  Configures MCO2 (PC9) to output a clock signal
 * @param  ClkSource: Clock source to output (0:SYSCLK, 1:PLLI2S, 2:HSE, 3:PLL)
 * @param  Prescaler: Output prescaler (0:No div, 4:/2, 5:/3, 6:/4, 7:/5)
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_MCO2_Config(uint8_t ClkSource, uint8_t Prescaler);

/******************************************************************************
 * Function Prototypes - LSE and RTC
 *****************************************************************************/

/**
 * @brief  Enables the LSE (Low Speed External) oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSE_Enable(void);

/**
 * @brief  Disables the LSE oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSE_Disable(void);

/**
 * @brief  Enables the LSI (Low Speed Internal) oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSI_Enable(void);

/**
 * @brief  Disables the LSI oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSI_Disable(void);

/**
 * @brief  Configures the RTC clock source
 * @param  ClkSource: RTC clock source (0:No clock, 1:LSE, 2:LSI, 3:HSE)
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_RTC_SetClockSource(uint8_t ClkSource);

/**
 * @brief  Enables the RTC clock
 * @param  None
 * @retval None
 */
void RCC_RTC_EnableClock(void);

/**
 * @brief  Disables the RTC clock
 * @param  None
 * @retval None
 */
void RCC_RTC_DisableClock(void);

/******************************************************************************
 * Function Prototypes - Backup Domain and Reset Flags
 *****************************************************************************/

/**
 * @brief  Resets the Backup Domain (RTC registers and RTC clock)
 * @param  None
 * @retval None
 */
void RCC_BackupDomain_Reset(void);

/**
 * @brief  Clears all reset flags
 * @param  None
 * @retval None
 */
void RCC_ClearResetFlags(void);

/**
 * @brief  Gets the reset source flags
 * @param  None
 * @retval uint32_t: Reset flags (bit 25-31 of CSR register)
 */
uint32_t RCC_GetResetFlags(void);

/**
 * @brief  Gets the reset source flags
 * @param  None
 * @retval uint32_t: Reset flags (bit 25-31 of CSR register)
 */
uint32_t RCC_GetResetFlags(void);
 /* @retval uint32_t: AHB clock frequency
 */
uint32_t RCC_GetHCLKFrequency(void);

/**
 * @brief  Gets the APB1 clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB1 clock frequency
 */
uint32_t RCC_GetPCLK1Frequency(void);

/**
 * @brief  Gets the APB2 clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB2 clock frequency
 */
uint32_t RCC_GetPCLK2Frequency(void);

/**
 * @brief  Gets the APB1 timer clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB1 timer clock frequency
 * @note   Timer clocks are 2x APB clock when APB prescaler != 1
 */
uint32_t RCC_GetTIM_APB1_Frequency(void);

/**
 * @brief  Gets the APB2 timer clock frequency in Hz
 * @param  None
 * @retval uint32_t: APB2 timer clock frequency
 * @note   Timer clocks are 2x APB clock when APB prescaler != 1
 */
uint32_t RCC_GetTIM_APB2_Frequency(void);

/******************************************************************************
 * Function Prototypes - CSS and Security
 *****************************************************************************/

/**
 * @brief  Enables the Clock Security System (CSS)
 * @param  None
 * @retval None
 * @note   CSS monitors the HSE clock and switches to HSI if HSE fails
 */
void RCC_CSS_Enable(void);

/**
 * @brief  Disables the Clock Security System (CSS)
 * @param  None
 * @retval None
 */
void RCC_CSS_Disable(void);

/******************************************************************************
 * Function Prototypes - MCO (Microcontroller Clock Output)
 *****************************************************************************/

/**
 * @brief  Configures MCO1 (PA8) to output a clock signal
 * @param  ClkSource: Clock source to output (0:HSI, 1:LSE, 2:HSE, 3:PLL)
 * @param  Prescaler: Output prescaler (0:No div, 4:/2, 5:/3, 6:/4, 7:/5)
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_MCO1_Config(uint8_t ClkSource, uint8_t Prescaler);

/**
 * @brief  Configures MCO2 (PC9) to output a clock signal
 * @param  ClkSource: Clock source to output (0:SYSCLK, 1:PLLI2S, 2:HSE, 3:PLL)
 * @param  Prescaler: Output prescaler (0:No div, 4:/2, 5:/3, 6:/4, 7:/5)
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_MCO2_Config(uint8_t ClkSource, uint8_t Prescaler);

/******************************************************************************
 * Function Prototypes - LSE and RTC
 *****************************************************************************/

/**
 * @brief  Enables the LSE (Low Speed External) oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSE_Enable(void);

/**
 * @brief  Disables the LSE oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSE_Disable(void);

/**
 * @brief  Enables the LSI (Low Speed Internal) oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSI_Enable(void);

/**
 * @brief  Disables the LSI oscillator
 * @param  None
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_LSI_Disable(void);

/**
 * @brief  Configures the RTC clock source
 * @param  ClkSource: RTC clock source (0:No clock, 1:LSE, 2:LSI, 3:HSE)
 * @retval RCC_Status_t: Operation status
 */
RCC_Status_t RCC_RTC_SetClockSource(uint8_t ClkSource);

/**
 * @brief  Enables the RTC clock
 * @param  None
 * @retval None
 */
void RCC_RTC_EnableClock(void);

/**
 * @brief  Disables the RTC clock
 * @param  None
 * @retval None
 */
void RCC_RTC_DisableClock(void);

/******************************************************************************
 * Function Prototypes - Backup Domain and Reset Flags
 *****************************************************************************/

/**
 * @brief  Resets the Backup Domain (RTC registers and RTC clock)
 * @param  None
 * @retval None
 */
void RCC_BackupDomain_Reset(void);

/**
 * @brief  Clears all reset flags
 * @param  None
 * @retval None
 */
void RCC_ClearResetFlags(void);

/**
 * @brief  Gets the reset source flags
 * @param  None
 * @retval uint32_t: Reset flags (bit 25-31 of CSR register)
 */
uint32_t RCC_GetResetFlags(void);







#endif