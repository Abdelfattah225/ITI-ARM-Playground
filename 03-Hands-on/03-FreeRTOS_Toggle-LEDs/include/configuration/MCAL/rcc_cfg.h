/******************************************************************************
 * @file    RCC_cfg.h
 * @brief   RCC Driver Configuration for STM32F401CC
 * @author  Abdelfattah moawed & commented by AI
 * @date    2025
 *****************************************************************************/

#ifndef RCC_CFG_H
#define RCC_CFG_H
#include "../../private/MCAL/rcc_prv.h"
/******************************************************************************
 * Configuration Options
 *****************************************************************************/

/*============================================================================
 * System Clock Configuration
 *============================================================================*/

/**
 * @brief  Target System Clock Frequency (SYSCLK) in Hz
 * @note   Valid options for STM32F401CC:
 *         - 16000000  (16 MHz - HSI)
 *         - 25000000  (25 MHz - if using 25 MHz HSE)
 *         - 50000000  (50 MHz)
 *         - 84000000  (84 MHz - Maximum for STM32F401)
 */
#define RCC_SYSCLK_FREQ                 84000000UL

/**
 * @brief  System Clock Source Selection
 * @note   Options:
 *         - 0: HSI (High Speed Internal 16 MHz)
 *         - 1: HSE (High Speed External)
 *         - 2: PLL (Phase-Locked Loop)
 */
#define RCC_CLOCK_SOURCE                2  // 0=HSI, 1=HSE, 2=PLL

/*============================================================================
 * HSE (High Speed External) Configuration
 *============================================================================*/

/**
 * @brief  HSE Clock Frequency in Hz
 * @note   Typical values: 4000000 to 26000000 Hz
 *         Common: 8000000 (8 MHz), 25000000 (25 MHz)
 */
#define RCC_HSE_FREQ                    25000000UL

/**
 * @brief  HSE Mode Selection
 * @note   Options:
 *         - 0: Crystal/Ceramic resonator
 *         - 1: External clock source (Bypass mode)
 */
#define RCC_HSE_BYPASS                  0  // 0=Crystal, 1=Bypass

/**
 * @brief  Enable/Disable HSE
 * @note   Options:
 *         - 0: Disabled
 *         - 1: Enabled
 */
#define RCC_HSE_ENABLE                  1  // 0=Disabled, 1=Enabled

/*============================================================================
 * HSI (High Speed Internal) Configuration
 *============================================================================*/

/**
 * @brief  HSI Clock Frequency (Fixed at 16 MHz)
 */
#define RCC_HSI_FREQ                    16000000UL

/**
 * @brief  Enable/Disable HSI
 * @note   HSI is always enabled at startup and used as default clock
 *         Options:
 *         - 0: Can be disabled after switching to HSE/PLL
 *         - 1: Keep enabled (recommended for CSS backup)
 */
#define RCC_HSI_ENABLE                  1  // 0=Disabled, 1=Enabled

/*============================================================================
 * PLL Configuration
 *============================================================================*/

/**
 * @brief  PLL Clock Source
 * @note   Options:
 *         - 0: HSI
 *         - 1: HSE
 */
#define RCC_PLL_SOURCE                  1  // 0=HSI, 1=HSE

/**
 * @brief  PLLM: Division factor for PLL input clock
 * @note   Range: 2 to 63
 *         Formula: VCO input = PLL input clock / PLLM
 *         VCO input must be between 1 and 2 MHz (recommended: 2 MHz)
 *         Example: If HSE=25MHz, PLLM=25 gives VCO input = 1 MHz
 */
#define RCC_PLLM_VALUE                  25

/**
 * @brief  PLLN: Multiplication factor for VCO
 * @note   Range: 50 to 432
 *         Formula: VCO output = VCO input × PLLN
 *         VCO output must be between 100 and 432 MHz
 *         Example: VCO input=1MHz, PLLN=336 gives VCO output = 336 MHz
 */
#define RCC_PLLN_VALUE                  336

/**
 * @brief  PLLP: Division factor for main system clock
 * @note   Options:
 *         - 0: /2 (PLLP = 2)
 *         - 1: /4 (PLLP = 4)
 *         - 2: /6 (PLLP = 6)
 *         - 3: /8 (PLLP = 8)
 *         Formula: SYSCLK = VCO output / PLLP
 *         Example: VCO=336MHz, PLLP=2 gives SYSCLK = 168MHz (too high!)
 *                  VCO=336MHz, PLLP=4 gives SYSCLK = 84MHz (perfect!)
 */
#define RCC_PLLP_VALUE                  1  // 0=/2, 1=/4, 2=/6, 3=/8

/**
 * @brief  PLLQ: Division factor for USB OTG FS, SDIO clocks
 * @note   Range: 2 to 15
 *         Formula: USB/SDIO clock = VCO output / PLLQ
 *         USB requires exactly 48 MHz
 *         Example: VCO=336MHz, PLLQ=7 gives 48MHz (perfect for USB!)
 */
#define RCC_PLLQ_VALUE                  7

/*============================================================================
 * Bus Prescalers Configuration
 *============================================================================*/

/**
 * @brief  AHB Prescaler (HCLK)
 * @note   Options:
 *         - 0: /1    - 1: /2    - 8: /2    - 9: /4
 *         - 10: /8   - 11: /16  - 12: /64  - 13: /128
 *         - 14: /256 - 15: /512
 *         HCLK feeds: CPU, AHB bus, memory, DMA, Cortex-M4 system timer
 *         Maximum: 84 MHz for STM32F401
 */
#define RCC_AHB_PRESCALER               0  // No division (HCLK = SYSCLK)

/**
 * @brief  APB1 Prescaler (PCLK1) - Low speed peripherals
 * @note   Options:
 *         - 0: /1 (Not divided)
 *         - 4: /2   - 5: /4   - 6: /8   - 7: /16
 *         PCLK1 feeds: TIM2-5, SPI2/3, USART2, I2C1-3, etc.
 *         Maximum: 42 MHz for STM32F401
 *         WARNING: Must not exceed 42 MHz!
 */
#define RCC_APB1_PRESCALER              4  // /2 (PCLK1 = 42 MHz if HCLK=84MHz)

/**
 * @brief  APB2 Prescaler (PCLK2) - High speed peripherals
 * @note   Options:
 *         - 0: /1 (Not divided)
 *         - 4: /2   - 5: /4   - 6: /8   - 7: /16
 *         PCLK2 feeds: TIM1/9-11, USART1/6, SPI1/4, ADC1, SYSCFG
 *         Maximum: 84 MHz for STM32F401
 */
#define RCC_APB2_PRESCALER              0  // No division (PCLK2 = 84 MHz)

/*============================================================================
 * Clock Security System (CSS) Configuration
 *============================================================================*/

/**
 * @brief  Enable/Disable Clock Security System
 * @note   CSS monitors HSE and switches to HSI if HSE fails
 *         Options:
 *         - 0: Disabled
 *         - 1: Enabled (Recommended for critical applications)
 */
#define RCC_CSS_ENABLE                  1  // 0=Disabled, 1=Enabled

/*============================================================================
 * LSE (Low Speed External) Configuration for RTC
 *============================================================================*/

/**
 * @brief  LSE Clock Frequency (Typically 32.768 kHz for RTC)
 */
#define RCC_LSE_FREQ                    32768UL

/**
 * @brief  Enable/Disable LSE
 * @note   Options:
 *         - 0: Disabled
 *         - 1: Enabled (Required for RTC with external crystal)
 */
#define RCC_LSE_ENABLE                  0  // 0=Disabled, 1=Enabled

/**
 * @brief  LSE Bypass Mode
 * @note   Options:
 *         - 0: Crystal oscillator
 *         - 1: External clock source
 */
#define RCC_LSE_BYPASS                  0  // 0=Crystal, 1=Bypass

/*============================================================================
 * LSI (Low Speed Internal) Configuration
 *============================================================================*/

/**
 * @brief  LSI Clock Frequency (Approximately 32 kHz, not accurate)
 */
#define RCC_LSI_FREQ                    32000UL

/**
 * @brief  Enable/Disable LSI
 * @note   Options:
 *         - 0: Disabled
 *         - 1: Enabled (Can be used for IWDG or RTC if no LSE)
 */
#define RCC_LSI_ENABLE                  0  // 0=Disabled, 1=Enabled

/*============================================================================
 * RTC Clock Configuration
 *============================================================================*/

/**
 * @brief  RTC Clock Source Selection
 * @note   Options:
 *         - 0: No clock
 *         - 1: LSE oscillator
 *         - 2: LSI oscillator
 *         - 3: HSE oscillator divided by RTC prescaler
 */
#define RCC_RTC_CLOCK_SOURCE            0  // 0=None, 1=LSE, 2=LSI, 3=HSE

/**
 * @brief  Enable/Disable RTC Clock
 * @note   Options:
 *         - 0: Disabled
 *         - 1: Enabled
 */
#define RCC_RTC_ENABLE                  0  // 0=Disabled, 1=Enabled

/*============================================================================
 * MCO (Microcontroller Clock Output) Configuration
 *============================================================================*/

/**
 * @brief  Enable MCO1 Output (PA8)
 * @note   Options:
 *         - 0: Disabled
 *         - 1: Enabled
 */
#define RCC_MCO1_ENABLE                 0  // 0=Disabled, 1=Enabled

/**
 * @brief  MCO1 Clock Source
 * @note   Options (when enabled):
 *         - 0: HSI    - 1: LSE    - 2: HSE    - 3: PLL
 */
#define RCC_MCO1_SOURCE                 2  // HSE

/**
 * @brief  MCO1 Prescaler
 * @note   Options:
 *         - 0: No division    - 4: /2    - 5: /3    - 6: /4    - 7: /5
 */
#define RCC_MCO1_PRESCALER              0  // No division

/**
 * @brief  Enable MCO2 Outputا (PC9)
 * @note   Options:
 *         - 0: Disabled
 *         - 1: Enabled
 */
#define RCC_MCO2_ENABLE                 0  // 0=Disabled, 1=Enabled

/**
 * @brief  MCO2 Clock Source
 * @note   Options (when enabled):
 *         - 0: SYSCLK    - 1: PLLI2S    - 2: HSE    - 3: PLL
 */
#define RCC_MCO2_SOURCE                 0  // SYSCLK

/**
 * @brief  MCO2 Prescaler
 * @note   Options:
 *         - 0: No division    - 4: /2    - 5: /3    - 6: /4    - 7: /5
 */
#define RCC_MCO2_PRESCALER              0  // No division

/*============================================================================
 * Timeout Configuration
 *============================================================================*/

/**
 * @brief  Timeout value for clock stabilization (in loop iterations)
 * @note   Adjust based on your system requirements
 *         Higher values = more tolerance but slower error detection
 */
#define RCC_HSE_TIMEOUT                 5000
#define RCC_HSI_TIMEOUT                 5000
#define RCC_PLL_TIMEOUT                 5000
#define RCC_LSE_TIMEOUT                 10000
#define RCC_LSI_TIMEOUT                 5000

/*============================================================================
 * Flash Latency Configuration (Based on SYSCLK and Vdd)
 *============================================================================*/

/**
 * @brief  Flash Latency (Wait States)
 * @note   For STM32F401 with Vdd = 2.7V to 3.6V:
 *         - 0 WS: 0 < SYSCLK ≤ 30 MHz
 *         - 1 WS: 30 MHz < SYSCLK ≤ 64 MHz
 *         - 2 WS: 64 MHz < SYSCLK ≤ 84 MHz
 *         
 *         This is automatically calculated but can be overridden
 */
#if (RCC_SYSCLK_FREQ <= 30000000)
    #define RCC_FLASH_LATENCY           0
#elif (RCC_SYSCLK_FREQ <= 64000000)
    #define RCC_FLASH_LATENCY           1
#else
    #define RCC_FLASH_LATENCY           2
#endif

/*============================================================================
 * Configuration Validation Macros (Do Not Modify)
 *============================================================================*/

/* Validate SYSCLK frequency */
#if (RCC_SYSCLK_FREQ > 84000000)
    #error "SYSCLK frequency exceeds maximum 84 MHz for STM32F401!"
#endif

/* Validate APB1 frequency */
#if ((RCC_SYSCLK_FREQ >> ((RCC_AHB_PRESCALER == 0) ? 0 : (RCC_AHB_PRESCALER - 7))) >> \
     ((RCC_APB1_PRESCALER == 0) ? 0 : (RCC_APB1_PRESCALER - 3)) > 42000000)
    #error "APB1 (PCLK1) frequency exceeds maximum 42 MHz!"
#endif

/* Validate PLLM range */
#if (RCC_PLLM_VALUE < 2) || (RCC_PLLM_VALUE > 63)
    #error "PLLM must be between 2 and 63!"
#endif

/* Validate PLLN range */
#if (RCC_PLLN_VALUE < 50) || (RCC_PLLN_VALUE > 432)
    #error "PLLN must be between 50 and 432!"
#endif

/* Validate PLLQ range */
#if (RCC_PLLQ_VALUE < 2) || (RCC_PLLQ_VALUE > 15)
    #error "PLLQ must be between 2 and 15!"
#endif

/******************************************************************************
 * Pre-calculated Clock Frequencies (Do Not Modify)
 *****************************************************************************/

/* Calculate expected frequencies for validation */
#if (RCC_CLOCK_SOURCE == 2) /* PLL */
    #if (RCC_PLL_SOURCE == 1) /* HSE */
        #define RCC_PLL_INPUT_FREQ      RCC_HSE_FREQ
    #else /* HSI */
        #define RCC_PLL_INPUT_FREQ      RCC_HSI_FREQ
    #endif
    
    #define RCC_VCO_INPUT_FREQ          (RCC_PLL_INPUT_FREQ / RCC_PLLM_VALUE)
    #define RCC_VCO_OUTPUT_FREQ         (RCC_VCO_INPUT_FREQ * RCC_PLLN_VALUE)
    #define RCC_CALCULATED_SYSCLK       (RCC_VCO_OUTPUT_FREQ / (2 << RCC_PLLP_VALUE))
#elif (RCC_CLOCK_SOURCE == 1) /* HSE */
    #define RCC_CALCULATED_SYSCLK       RCC_HSE_FREQ
#else /* HSI */
    #define RCC_CALCULATED_SYSCLK       RCC_HSI_FREQ
#endif

/******************************************************************************
 * Quick Configuration Presets (Comment/Uncomment as needed)
 *****************************************************************************/

/*
 * PRESET 1: 84 MHz from 25 MHz HSE (Maximum Performance)
 * ----------------------------------------------------
 * HSE = 25 MHz
 * PLLM = 25 (VCO input = 1 MHz)
 * PLLN = 336 (VCO output = 336 MHz)
 * PLLP = /4 (SYSCLK = 84 MHz)
 * PLLQ = 7 (USB = 48 MHz)
 * AHB = /1 (84 MHz), APB1 = /2 (42 MHz), APB2 = /1 (84 MHz)
 */

/*
 * PRESET 2: 84 MHz from 8 MHz HSE (Common Crystal)
 * -----------------------------------------------
 * HSE = 8 MHz
 * PLLM = 8 (VCO input = 1 MHz)
 * PLLN = 336 (VCO output = 336 MHz)
 * PLLP = /4 (SYSCLK = 84 MHz)
 * PLLQ = 7 (USB = 48 MHz)
 * Change: RCC_HSE_FREQ = 8000000, RCC_PLLM_VALUE = 8
 */

/*
 * PRESET 3: 50 MHz from HSI (No External Crystal)
 * ----------------------------------------------
 * HSI = 16 MHz
 * PLLM = 16 (VCO input = 1 MHz)
 * PLLN = 200 (VCO output = 200 MHz)
 * PLLP = /4 (SYSCLK = 50 MHz)
 * Change: RCC_PLL_SOURCE = 0, RCC_PLLM_VALUE = 16, RCC_PLLN_VALUE = 200
 */

#endif /* RCC_CFG_H */