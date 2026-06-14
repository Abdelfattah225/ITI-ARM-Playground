/******************************************************************************
 * RCC_private.h
 * RCC Register Definitions for STM32F401CC
 * @author  Abdelfattah moawed & commented by AI
 * 2025
 *****************************************************************************/

#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include <stdint.h>

/******************************************************************************
 * RCC Base Address
 *****************************************************************************/
#define RCC_BASE_ADDRESS    0x40023800UL


/******************************************************************************
 * RCC Register Structure - Contains all RCC registers
 *****************************************************************************/

/******************************************************************************
 * RCC_CR - Clock Control Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t HSION      : 1;    // Bit 0: Internal high-speed clock enable
        uint32_t HSIRDY     : 1;    // Bit 1: Internal high-speed clock ready
        uint32_t            : 1;    // Bit 2: Reserved
        uint32_t HSITRIM    : 5;    // Bits 3-7: Internal high-speed clock trimming
        uint32_t HSICAL     : 8;    // Bits 8-15: Internal high-speed clock calibration
        uint32_t HSEON      : 1;    // Bit 16: HSE clock enable
        uint32_t HSERDY     : 1;    // Bit 17: HSE clock ready
        uint32_t HSEBYP     : 1;    // Bit 18: HSE clock bypass
        uint32_t CSSON      : 1;    // Bit 19: Clock security system enable
        uint32_t            : 4;    // Bits 20-23: Reserved
        uint32_t PLLON      : 1;    // Bit 24: Main PLL enable
        uint32_t PLLRDY     : 1;    // Bit 25: Main PLL clock ready
        uint32_t PLLI2SON   : 1;    // Bit 26: PLLI2S enable
        uint32_t PLLI2SRDY  : 1;    // Bit 27: PLLI2S clock ready
        uint32_t            : 4;    // Bits 28-31: Reserved
    } BIT;
} RCC_CR_t;

/******************************************************************************
 * RCC_PLLCFGR - PLL Configuration Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t PLLM       : 6;    // Bits 0-5: Division factor for PLL input clock
        uint32_t PLLN       : 9;    // Bits 6-14: Main PLL multiplication factor
        uint32_t            : 1;    // Bit 15: Reserved
        uint32_t PLLP       : 2;    // Bits 16-17: Main PLL division factor
        uint32_t            : 4;    // Bits 18-21: Reserved
        uint32_t PLLSRC     : 1;    // Bit 22: Main PLL entry clock source
        uint32_t            : 1;    // Bit 23: Reserved
        uint32_t PLLQ       : 4;    // Bits 24-27: Main PLL division factor for USB OTG FS, SDIO
        uint32_t            : 4;    // Bits 28-31: Reserved
    } BIT;
} RCC_PLLCFGR_t;

/******************************************************************************
 * RCC_CFGR - Clock Configuration Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t SW         : 2;    // Bits 0-1: System clock switch
        uint32_t SWS        : 2;    // Bits 2-3: System clock switch status
        uint32_t HPRE       : 4;    // Bits 4-7: AHB prescaler
        uint32_t            : 2;    // Bits 8-9: Reserved
        uint32_t PPRE1      : 3;    // Bits 10-12: APB Low speed prescaler (APB1)
        uint32_t PPRE2      : 3;    // Bits 13-15: APB high-speed prescaler (APB2)
        uint32_t RTCPRE     : 5;    // Bits 16-20: HSE division factor for RTC clock
        uint32_t MCO1       : 2;    // Bits 21-22: Microcontroller clock output 1
        uint32_t I2SSRC     : 1;    // Bit 23: I2S clock selection
        uint32_t MCO1PRE    : 3;    // Bits 24-26: MCO1 prescaler
        uint32_t MCO2PRE    : 3;    // Bits 27-29: MCO2 prescaler
        uint32_t MCO2       : 2;    // Bits 30-31: Microcontroller clock output 2
    } BIT;
} RCC_CFGR_t;

/******************************************************************************
 * RCC_CIR - Clock Interrupt Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t LSIRDYF    : 1;    // Bit 0: LSI ready interrupt flag
        uint32_t LSERDYF    : 1;    // Bit 1: LSE ready interrupt flag
        uint32_t HSIRDYF    : 1;    // Bit 2: HSI ready interrupt flag
        uint32_t HSERDYF    : 1;    // Bit 3: HSE ready interrupt flag
        uint32_t PLLRDYF    : 1;    // Bit 4: Main PLL ready interrupt flag
        uint32_t PLLI2SRDYF : 1;    // Bit 5: PLLI2S ready interrupt flag
        uint32_t            : 1;    // Bit 6: Reserved
        uint32_t CSSF       : 1;    // Bit 7: Clock security system interrupt flag
        uint32_t LSIRDYIE   : 1;    // Bit 8: LSI ready interrupt enable
        uint32_t LSERDYIE   : 1;    // Bit 9: LSE ready interrupt enable
        uint32_t HSIRDYIE   : 1;    // Bit 10: HSI ready interrupt enable
        uint32_t HSERDYIE   : 1;    // Bit 11: HSE ready interrupt enable
        uint32_t PLLRDYIE   : 1;    // Bit 12: Main PLL ready interrupt enable
        uint32_t PLLI2SRDYIE: 1;    // Bit 13: PLLI2S ready interrupt enable
        uint32_t            : 2;    // Bits 14-15: Reserved
        uint32_t LSIRDYC    : 1;    // Bit 16: LSI ready interrupt clear
        uint32_t LSERDYC    : 1;    // Bit 17: LSE ready interrupt clear
        uint32_t HSIRDYC    : 1;    // Bit 18: HSI ready interrupt clear
        uint32_t HSERDYC    : 1;    // Bit 19: HSE ready interrupt clear
        uint32_t PLLRDYC    : 1;    // Bit 20: Main PLL ready interrupt clear
        uint32_t PLLI2SRDYC : 1;    // Bit 21: PLLI2S ready interrupt clear
        uint32_t            : 1;    // Bit 22: Reserved
        uint32_t CSSC       : 1;    // Bit 23: Clock security system interrupt clear
        uint32_t            : 8;    // Bits 24-31: Reserved
    } BIT;
} RCC_CIR_t;

/******************************************************************************
 * RCC_AHB1RSTR - AHB1 Peripheral Reset Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t GPIOARST   : 1;    // Bit 0: IO port A reset
        uint32_t GPIOBRST   : 1;    // Bit 1: IO port B reset
        uint32_t GPIOCRST   : 1;    // Bit 2: IO port C reset
        uint32_t GPIODRST   : 1;    // Bit 3: IO port D reset
        uint32_t GPIOERST   : 1;    // Bit 4: IO port E reset
        uint32_t            : 2;    // Bits 5-6: Reserved
        uint32_t GPIOHRST   : 1;    // Bit 7: IO port H reset
        uint32_t            : 4;    // Bits 8-11: Reserved
        uint32_t CRCRST     : 1;    // Bit 12: CRC reset
        uint32_t            : 8;    // Bits 13-20: Reserved
        uint32_t DMA1RST    : 1;    // Bit 21: DMA1 reset
        uint32_t DMA2RST    : 1;    // Bit 22: DMA2 reset
        uint32_t            : 9;    // Bits 23-31: Reserved
    } BIT;
} RCC_AHB1RSTR_t;

/******************************************************************************
 * RCC_AHB2RSTR - AHB2 Peripheral Reset Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t            : 7;    // Bits 0-6: Reserved
        uint32_t OTGFSRST   : 1;    // Bit 7: USB OTG FS reset
        uint32_t            : 24;   // Bits 8-31: Reserved
    } BIT;
} RCC_AHB2RSTR_t;

/******************************************************************************
 * RCC_APB1RSTR - APB1 Peripheral Reset Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t TIM2RST    : 1;    // Bit 0: TIM2 reset
        uint32_t TIM3RST    : 1;    // Bit 1: TIM3 reset
        uint32_t TIM4RST    : 1;    // Bit 2: TIM4 reset
        uint32_t TIM5RST    : 1;    // Bit 3: TIM5 reset
        uint32_t            : 7;    // Bits 4-10: Reserved
        uint32_t WWDGRST    : 1;    // Bit 11: Window watchdog reset
        uint32_t            : 2;    // Bits 12-13: Reserved
        uint32_t SPI2RST    : 1;    // Bit 14: SPI2 reset
        uint32_t SPI3RST    : 1;    // Bit 15: SPI3 reset
        uint32_t            : 1;    // Bit 16: Reserved
        uint32_t USART2RST  : 1;    // Bit 17: USART2 reset
        uint32_t            : 3;    // Bits 18-20: Reserved
        uint32_t I2C1RST    : 1;    // Bit 21: I2C1 reset
        uint32_t I2C2RST    : 1;    // Bit 22: I2C2 reset
        uint32_t I2C3RST    : 1;    // Bit 23: I2C3 reset
        uint32_t            : 4;    // Bits 24-27: Reserved
        uint32_t PWRRST     : 1;    // Bit 28: Power interface reset
        uint32_t            : 3;    // Bits 29-31: Reserved
    } BIT;
} RCC_APB1RSTR_t;

/******************************************************************************
 * RCC_APB2RSTR - APB2 Peripheral Reset Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t TIM1RST    : 1;    // Bit 0: TIM1 reset
        uint32_t            : 3;    // Bits 1-3: Reserved
        uint32_t USART1RST  : 1;    // Bit 4: USART1 reset
        uint32_t USART6RST  : 1;    // Bit 5: USART6 reset
        uint32_t            : 2;    // Bits 6-7: Reserved
        uint32_t ADC1RST    : 1;    // Bit 8: ADC1 reset
        uint32_t            : 2;    // Bits 9-10: Reserved
        uint32_t SDIORST    : 1;    // Bit 11: SDIO reset
        uint32_t SPI1RST    : 1;    // Bit 12: SPI1 reset
        uint32_t SPI4RST    : 1;    // Bit 13: SPI4 reset
        uint32_t SYSCFGRST  : 1;    // Bit 14: System configuration controller reset
        uint32_t            : 1;    // Bit 15: Reserved
        uint32_t TIM9RST    : 1;    // Bit 16: TIM9 reset
        uint32_t TIM10RST   : 1;    // Bit 17: TIM10 reset
        uint32_t TIM11RST   : 1;    // Bit 18: TIM11 reset
        uint32_t            : 13;   // Bits 19-31: Reserved
    } BIT;
} RCC_APB2RSTR_t;

/******************************************************************************
 * RCC_AHB1ENR - AHB1 Peripheral Clock Enable Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t GPIOAEN    : 1;    // Bit 0: IO port A clock enable
        uint32_t GPIOBEN    : 1;    // Bit 1: IO port B clock enable
        uint32_t GPIOCEN    : 1;    // Bit 2: IO port C clock enable
        uint32_t GPIODEN    : 1;    // Bit 3: IO port D clock enable
        uint32_t GPIOEEN    : 1;    // Bit 4: IO port E clock enable
        uint32_t            : 2;    // Bits 5-6: Reserved
        uint32_t GPIOHEN    : 1;    // Bit 7: IO port H clock enable
        uint32_t            : 4;    // Bits 8-11: Reserved
        uint32_t CRCEN      : 1;    // Bit 12: CRC clock enable
        uint32_t            : 8;    // Bits 13-20: Reserved
        uint32_t DMA1EN     : 1;    // Bit 21: DMA1 clock enable
        uint32_t DMA2EN     : 1;    // Bit 22: DMA2 clock enable
        uint32_t            : 9;    // Bits 23-31: Reserved
    } BIT;
} RCC_AHB1ENR_t;

/******************************************************************************
 * RCC_AHB2ENR - AHB2 Peripheral Clock Enable Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t            : 7;    // Bits 0-6: Reserved
        uint32_t OTGFSEN    : 1;    // Bit 7: USB OTG FS clock enable
        uint32_t            : 24;   // Bits 8-31: Reserved
    } BIT;
} RCC_AHB2ENR_t;

/******************************************************************************
 * RCC_APB1ENR - APB1 Peripheral Clock Enable Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t TIM2EN     : 1;    // Bit 0: TIM2 clock enable
        uint32_t TIM3EN     : 1;    // Bit 1: TIM3 clock enable
        uint32_t TIM4EN     : 1;    // Bit 2: TIM4 clock enable
        uint32_t TIM5EN     : 1;    // Bit 3: TIM5 clock enable
        uint32_t            : 7;    // Bits 4-10: Reserved
        uint32_t WWDGEN     : 1;    // Bit 11: Window watchdog clock enable
        uint32_t            : 2;    // Bits 12-13: Reserved
        uint32_t SPI2EN     : 1;    // Bit 14: SPI2 clock enable
        uint32_t SPI3EN     : 1;    // Bit 15: SPI3 clock enable
        uint32_t            : 1;    // Bit 16: Reserved
        uint32_t USART2EN   : 1;    // Bit 17: USART2 clock enable
        uint32_t            : 3;    // Bits 18-20: Reserved
        uint32_t I2C1EN     : 1;    // Bit 21: I2C1 clock enable
        uint32_t I2C2EN     : 1;    // Bit 22: I2C2 clock enable
        uint32_t I2C3EN     : 1;    // Bit 23: I2C3 clock enable
        uint32_t            : 4;    // Bits 24-27: Reserved
        uint32_t PWREN      : 1;    // Bit 28: Power interface clock enable
        uint32_t            : 3;    // Bits 29-31: Reserved
    } BIT;
} RCC_APB1ENR_t;

/******************************************************************************
 * RCC_APB2ENR - APB2 Peripheral Clock Enable Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t TIM1EN     : 1;    // Bit 0: TIM1 clock enable
        uint32_t            : 3;    // Bits 1-3: Reserved
        uint32_t USART1EN   : 1;    // Bit 4: USART1 clock enable
        uint32_t USART6EN   : 1;    // Bit 5: USART6 clock enable
        uint32_t            : 2;    // Bits 6-7: Reserved
        uint32_t ADC1EN     : 1;    // Bit 8: ADC1 clock enable
        uint32_t            : 2;    // Bits 9-10: Reserved
        uint32_t SDIOEN     : 1;    // Bit 11: SDIO clock enable
        uint32_t SPI1EN     : 1;    // Bit 12: SPI1 clock enable
        uint32_t SPI4EN     : 1;    // Bit 13: SPI4 clock enable
        uint32_t SYSCFGEN   : 1;    // Bit 14: System configuration controller clock enable
        uint32_t            : 1;    // Bit 15: Reserved
        uint32_t TIM9EN     : 1;    // Bit 16: TIM9 clock enable
        uint32_t TIM10EN    : 1;    // Bit 17: TIM10 clock enable
        uint32_t TIM11EN    : 1;    // Bit 18: TIM11 clock enable
        uint32_t            : 13;   // Bits 19-31: Reserved
    } BIT;
} RCC_APB2ENR_t;

/******************************************************************************
 * RCC_AHB1LPENR - AHB1 Peripheral Clock Enable in Low Power Mode Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t GPIOALPEN  : 1;    // Bit 0: IO port A clock enable during Sleep mode
        uint32_t GPIOBLPEN  : 1;    // Bit 1: IO port B clock enable during Sleep mode
        uint32_t GPIOCLPEN  : 1;    // Bit 2: IO port C clock enable during Sleep mode
        uint32_t GPIODLPEN  : 1;    // Bit 3: IO port D clock enable during Sleep mode
        uint32_t GPIOELPEN  : 1;    // Bit 4: IO port E clock enable during Sleep mode
        uint32_t            : 2;    // Bits 5-6: Reserved
        uint32_t GPIOHLPEN  : 1;    // Bit 7: IO port H clock enable during Sleep mode
        uint32_t            : 4;    // Bits 8-11: Reserved
        uint32_t CRCLPEN    : 1;    // Bit 12: CRC clock enable during Sleep mode
        uint32_t            : 2;    // Bits 13-14: Reserved
        uint32_t FLITFLPEN  : 1;    // Bit 15: Flash interface clock enable during Sleep mode
        uint32_t SRAM1LPEN  : 1;    // Bit 16: SRAM1 interface clock enable during Sleep mode
        uint32_t            : 4;    // Bits 17-20: Reserved
        uint32_t DMA1LPEN   : 1;    // Bit 21: DMA1 clock enable during Sleep mode
        uint32_t DMA2LPEN   : 1;    // Bit 22: DMA2 clock enable during Sleep mode
        uint32_t            : 9;    // Bits 23-31: Reserved
    } BIT;
} RCC_AHB1LPENR_t;

/******************************************************************************
 * RCC_AHB2LPENR - AHB2 Peripheral Clock Enable in Low Power Mode Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t            : 7;    // Bits 0-6: Reserved
        uint32_t OTGFSLPEN  : 1;    // Bit 7: USB OTG FS clock enable during Sleep mode
        uint32_t            : 24;   // Bits 8-31: Reserved
    } BIT;
} RCC_AHB2LPENR_t;

/******************************************************************************
 * RCC_APB1LPENR - APB1 Peripheral Clock Enable in Low Power Mode Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t TIM2LPEN   : 1;    // Bit 0: TIM2 clock enable during Sleep mode
        uint32_t TIM3LPEN   : 1;    // Bit 1: TIM3 clock enable during Sleep mode
        uint32_t TIM4LPEN   : 1;    // Bit 2: TIM4 clock enable during Sleep mode
        uint32_t TIM5LPEN   : 1;    // Bit 3: TIM5 clock enable during Sleep mode
        uint32_t            : 7;    // Bits 4-10: Reserved
        uint32_t WWDGLPEN   : 1;    // Bit 11: Window watchdog clock enable during Sleep mode
        uint32_t            : 2;    // Bits 12-13: Reserved
        uint32_t SPI2LPEN   : 1;    // Bit 14: SPI2 clock enable during Sleep mode
        uint32_t SPI3LPEN   : 1;    // Bit 15: SPI3 clock enable during Sleep mode
        uint32_t            : 1;    // Bit 16: Reserved
        uint32_t USART2LPEN : 1;    // Bit 17: USART2 clock enable during Sleep mode
        uint32_t            : 3;    // Bits 18-20: Reserved
        uint32_t I2C1LPEN   : 1;    // Bit 21: I2C1 clock enable during Sleep mode
        uint32_t I2C2LPEN   : 1;    // Bit 22: I2C2 clock enable during Sleep mode
        uint32_t I2C3LPEN   : 1;    // Bit 23: I2C3 clock enable during Sleep mode
        uint32_t            : 4;    // Bits 24-27: Reserved
        uint32_t PWRLPEN    : 1;    // Bit 28: Power interface clock enable during Sleep mode
        uint32_t            : 3;    // Bits 29-31: Reserved
    } BIT;
} RCC_APB1LPENR_t;

/******************************************************************************
 * RCC_APB2LPENR - APB2 Peripheral Clock Enable in Low Power Mode Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t TIM1LPEN   : 1;    // Bit 0: TIM1 clock enable during Sleep mode
        uint32_t            : 3;    // Bits 1-3: Reserved
        uint32_t USART1LPEN : 1;    // Bit 4: USART1 clock enable during Sleep mode
        uint32_t USART6LPEN : 1;    // Bit 5: USART6 clock enable during Sleep mode
        uint32_t            : 2;    // Bits 6-7: Reserved
        uint32_t ADC1LPEN   : 1;    // Bit 8: ADC1 clock enable during Sleep mode
        uint32_t            : 2;    // Bits 9-10: Reserved
        uint32_t SDIOLPEN   : 1;    // Bit 11: SDIO clock enable during Sleep mode
        uint32_t SPI1LPEN   : 1;    // Bit 12: SPI1 clock enable during Sleep mode
        uint32_t SPI4LPEN   : 1;    // Bit 13: SPI4 clock enable during Sleep mode
        uint32_t SYSCFGLPEN : 1;    // Bit 14: System configuration controller clock enable during Sleep mode
        uint32_t            : 1;    // Bit 15: Reserved
        uint32_t TIM9LPEN   : 1;    // Bit 16: TIM9 clock enable during Sleep mode
        uint32_t TIM10LPEN  : 1;    // Bit 17: TIM10 clock enable during Sleep mode
        uint32_t TIM11LPEN  : 1;    // Bit 18: TIM11 clock enable during Sleep mode
        uint32_t            : 13;   // Bits 19-31: Reserved
    } BIT;
} RCC_APB2LPENR_t;

/******************************************************************************
 * RCC_BDCR - Backup Domain Control Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t LSEON      : 1;    // Bit 0: External low-speed oscillator enable
        uint32_t LSERDY     : 1;    // Bit 1: External low-speed oscillator ready
        uint32_t LSEBYP     : 1;    // Bit 2: External low-speed oscillator bypass
        uint32_t            : 5;    // Bits 3-7: Reserved
        uint32_t RTCSEL     : 2;    // Bits 8-9: RTC clock source selection
        uint32_t            : 5;    // Bits 10-14: Reserved
        uint32_t RTCEN      : 1;    // Bit 15: RTC clock enable
        uint32_t BDRST      : 1;    // Bit 16: Backup domain software reset
        uint32_t            : 15;   // Bits 17-31: Reserved
    } BIT;
} RCC_BDCR_t;

/******************************************************************************
 * RCC_CSR - Control/Status Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t LSION      : 1;    // Bit 0: Internal low-speed oscillator enable
        uint32_t LSIRDY     : 1;    // Bit 1: Internal low-speed oscillator ready
        uint32_t            : 22;   // Bits 2-23: Reserved
        uint32_t RMVF       : 1;    // Bit 24: Remove reset flag
        uint32_t BORRSTF    : 1;    // Bit 25: BOR reset flag
        uint32_t PINRSTF    : 1;    // Bit 26: PIN reset flag
        uint32_t PORRSTF    : 1;    // Bit 27: POR/PDR reset flag
        uint32_t SFTRSTF    : 1;    // Bit 28: Software reset flag
        uint32_t IWDGRSTF   : 1;    // Bit 29: Independent watchdog reset flag
        uint32_t WWDGRSTF   : 1;    // Bit 30: Window watchdog reset flag
        uint32_t LPWRRSTF   : 1;    // Bit 31: Low-power reset flag
    } BIT;
} RCC_CSR_t;

/******************************************************************************
 * RCC_SSCGR - Spread Spectrum Clock Generation Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t MODPER     : 13;   // Bits 0-12: Modulation period
        uint32_t INCSTEP    : 15;   // Bits 13-27: Incrementation step
        uint32_t            : 2;    // Bits 28-29: Reserved
        uint32_t SPREADSEL  : 1;    // Bit 30: Spread Select
        uint32_t SSCGEN     : 1;    // Bit 31: Spread spectrum modulation enable
    } BIT;
} RCC_SSCGR_t;

/******************************************************************************
 * RCC_PLLI2SCFGR - PLLI2S Configuration Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t            : 6;    // Bits 0-5: Reserved
        uint32_t PLLI2SN    : 9;    // Bits 6-14: PLLI2S multiplication factor for VCO
        uint32_t            : 13;   // Bits 15-27: Reserved
        uint32_t PLLI2SR    : 3;    // Bits 28-30: PLLI2S division factor for I2S clocks
        uint32_t            : 1;    // Bit 31: Reserved
    } BIT;
} RCC_PLLI2SCFGR_t;

typedef struct {
    volatile RCC_CR_t           CR;             // 0x00: Clock control register
    volatile RCC_PLLCFGR_t      PLLCFGR;        // 0x04: PLL configuration register
    volatile RCC_CFGR_t         CFGR;           // 0x08: Clock configuration register
    volatile RCC_CIR_t          CIR;            // 0x0C: Clock interrupt register
    volatile RCC_AHB1RSTR_t     AHB1RSTR;       // 0x10: AHB1 peripheral reset register
    volatile RCC_AHB2RSTR_t     AHB2RSTR;       // 0x14: AHB2 peripheral reset register
    uint32_t                    RESERVED0[2];   // 0x18-0x1C: Reserved
    volatile RCC_APB1RSTR_t     APB1RSTR;       // 0x20: APB1 peripheral reset register
    volatile RCC_APB2RSTR_t     APB2RSTR;       // 0x24: APB2 peripheral reset register
    uint32_t                    RESERVED1[2];   // 0x28-0x2C: Reserved
    volatile RCC_AHB1ENR_t      AHB1ENR;        // 0x30: AHB1 peripheral clock enable register
    volatile RCC_AHB2ENR_t      AHB2ENR;        // 0x34: AHB2 peripheral clock enable register
    uint32_t                    RESERVED2[2];   // 0x38-0x3C: Reserved
    volatile RCC_APB1ENR_t      APB1ENR;        // 0x40: APB1 peripheral clock enable register
    volatile RCC_APB2ENR_t      APB2ENR;        // 0x44: APB2 peripheral clock enable register
    uint32_t                    RESERVED3[2];   // 0x48-0x4C: Reserved
    volatile RCC_AHB1LPENR_t    AHB1LPENR;      // 0x50: AHB1 peripheral clock enable in low power mode register
    volatile RCC_AHB2LPENR_t    AHB2LPENR;      // 0x54: AHB2 peripheral clock enable in low power mode register
    uint32_t                    RESERVED4[2];   // 0x58-0x5C: Reserved
    volatile RCC_APB1LPENR_t    APB1LPENR;      // 0x60: APB1 peripheral clock enable in low power mode register
    volatile RCC_APB2LPENR_t    APB2LPENR;      // 0x64: APB2 peripheral clock enable in low power mode register
    uint32_t                    RESERVED5[2];   // 0x68-0x6C: Reserved
    volatile RCC_BDCR_t         BDCR;           // 0x70: Backup domain control register
    volatile RCC_CSR_t          CSR;            // 0x74: Clock control & status register
    uint32_t                    RESERVED6[2];   // 0x78-0x7C: Reserved
    volatile RCC_SSCGR_t        SSCGR;          // 0x80: Spread spectrum clock generation register
    volatile RCC_PLLI2SCFGR_t   PLLI2SCFGR;     // 0x84: PLLI2S configuration register
} RCC_TypeDef;

#define RCC     ((RCC_TypeDef *) RCC_BASE_ADDRESS)
    
#endif