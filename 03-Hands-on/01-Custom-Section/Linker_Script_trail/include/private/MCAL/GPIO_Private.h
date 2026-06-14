
#ifdef __cplusplus
namespace MCAL{
    namespace GPIO{
#endif



#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include <stdint.h>
// 1. base address of each port

#define GPIOA_BASE_ADDRESS 0x40020000UL
#define GPIOB_BASE_ADDRESS 0x40020400UL
#define GPIOC_BASE_ADDRESS 0x40020800UL
#define GPIOD_BASE_ADDRESS 0x40020C00UL
#define GPIOE_BASE_ADDRESS 0x40021000UL


/******************************************************************************
 * GPIO_MODER - GPIO Port Mode Register
 *****************************************************************************/
typedef union
{
        uint32_t REG;
    struct 
    {
        uint32_t MODER0     : 2;    // Bits 0-1: Port x configuration bits (y = 0)
        uint32_t MODER1     : 2;    // Bits 2-3
        uint32_t MODER2     : 2;    // Bits 4-5
        uint32_t MODER3     : 2;    // Bits 6-7
        uint32_t MODER4     : 2;    // Bits 8-9
        uint32_t MODER5     : 2;    // Bits 10-11
        uint32_t MODER6     : 2;    // Bits 12-13
        uint32_t MODER7     : 2;    // Bits 14-15
        uint32_t MODER8     : 2;    // Bits 16-17
        uint32_t MODER9     : 2;    // Bits 18-19
        uint32_t MODER10    : 2;    // Bits 20-21
        uint32_t MODER11    : 2;    // Bits 22-23
        uint32_t MODER12    : 2;    // Bits 24-25
        uint32_t MODER13    : 2;    // Bits 26-27
        uint32_t MODER14    : 2;    // Bits 28-29
        uint32_t MODER15    : 2;    // Bits 30-31
    }BIT;
    
}GPIO_MODER_t;


/******************************************************************************
 * GPIO_OTYPER - GPIO Port Output Type Register ==> used when pin mode OUTPUT OR Alternative function
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t OT0        : 1;    // Bit 0: Port x configuration bit 0
        uint32_t OT1        : 1;    // Bit 1
        uint32_t OT2        : 1;    // Bit 2
        uint32_t OT3        : 1;    // Bit 3
        uint32_t OT4        : 1;    // Bit 4
        uint32_t OT5        : 1;    // Bit 5
        uint32_t OT6        : 1;    // Bit 6
        uint32_t OT7        : 1;    // Bit 7
        uint32_t OT8        : 1;    // Bit 8
        uint32_t OT9        : 1;    // Bit 9
        uint32_t OT10       : 1;    // Bit 10
        uint32_t OT11       : 1;    // Bit 11
        uint32_t OT12       : 1;    // Bit 12
        uint32_t OT13       : 1;    // Bit 13
        uint32_t OT14       : 1;    // Bit 14
        uint32_t OT15       : 1;    // Bit 15
        uint32_t            : 16;   // Bits 16-31: Reserved
    } BIT;
} GPIO_OTYPER_t;


/******************************************************************************
 * GPIO_OSPEEDR - GPIO Port Output Speed Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t OSPEEDR0   : 2;    // Bits 0-1: Port x configuration bits (y = 0)
        uint32_t OSPEEDR1   : 2;    // Bits 2-3
        uint32_t OSPEEDR2   : 2;    // Bits 4-5
        uint32_t OSPEEDR3   : 2;    // Bits 6-7
        uint32_t OSPEEDR4   : 2;    // Bits 8-9
        uint32_t OSPEEDR5   : 2;    // Bits 10-11
        uint32_t OSPEEDR6   : 2;    // Bits 12-13
        uint32_t OSPEEDR7   : 2;    // Bits 14-15
        uint32_t OSPEEDR8   : 2;    // Bits 16-17
        uint32_t OSPEEDR9   : 2;    // Bits 18-19
        uint32_t OSPEEDR10  : 2;    // Bits 20-21
        uint32_t OSPEEDR11  : 2;    // Bits 22-23
        uint32_t OSPEEDR12  : 2;    // Bits 24-25
        uint32_t OSPEEDR13  : 2;    // Bits 26-27
        uint32_t OSPEEDR14  : 2;    // Bits 28-29
        uint32_t OSPEEDR15  : 2;    // Bits 30-31
    } BIT;
} GPIO_OSPEEDR_t;


/******************************************************************************
 * GPIO_PUPDR - GPIO Port Pull-up/Pull-down Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t PUPDR0     : 2;    // Bits 0-1: Port x configuration bits (y = 0)
        uint32_t PUPDR1     : 2;    // Bits 2-3
        uint32_t PUPDR2     : 2;    // Bits 4-5
        uint32_t PUPDR3     : 2;    // Bits 6-7
        uint32_t PUPDR4     : 2;    // Bits 8-9
        uint32_t PUPDR5     : 2;    // Bits 10-11
        uint32_t PUPDR6     : 2;    // Bits 12-13
        uint32_t PUPDR7     : 2;    // Bits 14-15
        uint32_t PUPDR8     : 2;    // Bits 16-17
        uint32_t PUPDR9     : 2;    // Bits 18-19
        uint32_t PUPDR10    : 2;    // Bits 20-21
        uint32_t PUPDR11    : 2;    // Bits 22-23
        uint32_t PUPDR12    : 2;    // Bits 24-25
        uint32_t PUPDR13    : 2;    // Bits 26-27
        uint32_t PUPDR14    : 2;    // Bits 28-29
        uint32_t PUPDR15    : 2;    // Bits 30-31
    } BIT;
} GPIO_PUPDR_t;


/******************************************************************************
 * GPIO_IDR - GPIO Port Input Data Register ==> used when pin mode is INPUT 
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t IDR0       : 1;    // Bit 0: Port input data (y = 0)
        uint32_t IDR1       : 1;    // Bit 1
        uint32_t IDR2       : 1;    // Bit 2
        uint32_t IDR3       : 1;    // Bit 3
        uint32_t IDR4       : 1;    // Bit 4
        uint32_t IDR5       : 1;    // Bit 5
        uint32_t IDR6       : 1;    // Bit 6
        uint32_t IDR7       : 1;    // Bit 7
        uint32_t IDR8       : 1;    // Bit 8
        uint32_t IDR9       : 1;    // Bit 9
        uint32_t IDR10      : 1;    // Bit 10
        uint32_t IDR11      : 1;    // Bit 11
        uint32_t IDR12      : 1;    // Bit 12
        uint32_t IDR13      : 1;    // Bit 13
        uint32_t IDR14      : 1;    // Bit 14
        uint32_t IDR15      : 1;    // Bit 15
        uint32_t            : 16;   // Bits 16-31: Reserved
    } BIT;
} GPIO_IDR_t;


/******************************************************************************
 * GPIO_ODR - GPIO Port Output Data Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t ODR0       : 1;    // Bit 0: Port output data (y = 0)
        uint32_t ODR1       : 1;    // Bit 1
        uint32_t ODR2       : 1;    // Bit 2
        uint32_t ODR3       : 1;    // Bit 3
        uint32_t ODR4       : 1;    // Bit 4
        uint32_t ODR5       : 1;    // Bit 5
        uint32_t ODR6       : 1;    // Bit 6
        uint32_t ODR7       : 1;    // Bit 7
        uint32_t ODR8       : 1;    // Bit 8
        uint32_t ODR9       : 1;    // Bit 9
        uint32_t ODR10      : 1;    // Bit 10
        uint32_t ODR11      : 1;    // Bit 11
        uint32_t ODR12      : 1;    // Bit 12
        uint32_t ODR13      : 1;    // Bit 13
        uint32_t ODR14      : 1;    // Bit 14
        uint32_t ODR15      : 1;    // Bit 15
        uint32_t            : 16;   // Bits 16-31: Reserved
    } BIT;
} GPIO_ODR_t;



/******************************************************************************
 * GPIO_BSRR - GPIO Port Bit Set/Reset Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t BS0        : 1;    // Bit 0: Port x set bit 0
        uint32_t BS1        : 1;    // Bit 1
        uint32_t BS2        : 1;    // Bit 2
        uint32_t BS3        : 1;    // Bit 3
        uint32_t BS4        : 1;    // Bit 4
        uint32_t BS5        : 1;    // Bit 5
        uint32_t BS6        : 1;    // Bit 6
        uint32_t BS7        : 1;    // Bit 7
        uint32_t BS8        : 1;    // Bit 8
        uint32_t BS9        : 1;    // Bit 9
        uint32_t BS10       : 1;    // Bit 10
        uint32_t BS11       : 1;    // Bit 11
        uint32_t BS12       : 1;    // Bit 12
        uint32_t BS13       : 1;    // Bit 13
        uint32_t BS14       : 1;    // Bit 14
        uint32_t BS15       : 1;    // Bit 15
        uint32_t BR0        : 1;    // Bit 16: Port x reset bit 0
        uint32_t BR1        : 1;    // Bit 17
        uint32_t BR2        : 1;    // Bit 18
        uint32_t BR3        : 1;    // Bit 19
        uint32_t BR4        : 1;    // Bit 20
        uint32_t BR5        : 1;    // Bit 21
        uint32_t BR6        : 1;    // Bit 22
        uint32_t BR7        : 1;    // Bit 23
        uint32_t BR8        : 1;    // Bit 24
        uint32_t BR9        : 1;    // Bit 25
        uint32_t BR10       : 1;    // Bit 26
        uint32_t BR11       : 1;    // Bit 27
        uint32_t BR12       : 1;    // Bit 28
        uint32_t BR13       : 1;    // Bit 29
        uint32_t BR14       : 1;    // Bit 30
        uint32_t BR15       : 1;    // Bit 31
    } BIT;
} GPIO_BSRR_t;



/******************************************************************************
 * GPIO_LCKR - GPIO Port Configuration Lock Register
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t LCK0       : 1;    // Bit 0: Port x lock bit 0
        uint32_t LCK1       : 1;    // Bit 1
        uint32_t LCK2       : 1;    // Bit 2
        uint32_t LCK3       : 1;    // Bit 3
        uint32_t LCK4       : 1;    // Bit 4
        uint32_t LCK5       : 1;    // Bit 5
        uint32_t LCK6       : 1;    // Bit 6
        uint32_t LCK7       : 1;    // Bit 7
        uint32_t LCK8       : 1;    // Bit 8
        uint32_t LCK9       : 1;    // Bit 9
        uint32_t LCK10      : 1;    // Bit 10
        uint32_t LCK11      : 1;    // Bit 11
        uint32_t LCK12      : 1;    // Bit 12
        uint32_t LCK13      : 1;    // Bit 13
        uint32_t LCK14      : 1;    // Bit 14
        uint32_t LCK15      : 1;    // Bit 15
        uint32_t LCKK       : 1;    // Bit 16: Lock key
        uint32_t            : 15;   // Bits 17-31: Reserved
    } BIT;
} GPIO_LCKR_t;



/******************************************************************************
 * GPIO_AFR - GPIO Alternate Function Register (Low )
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t AFR0       : 4;    // Bits 0-3: Alternate function selection for port x bit 0
        uint32_t AFR1       : 4;    // Bits 4-7
        uint32_t AFR2       : 4;    // Bits 8-11
        uint32_t AFR3       : 4;    // Bits 12-15
        uint32_t AFR4       : 4;    // Bits 16-19
        uint32_t AFR5       : 4;    // Bits 20-23
        uint32_t AFR6       : 4;    // Bits 24-27
        uint32_t AFR7       : 4;    // Bits 28-31
    } BIT;
} GPIO_AFRL_t;

/******************************************************************************
 * GPIO_AFR - GPIO Alternate Function Register (High)
 *****************************************************************************/
typedef union {
    uint32_t REG;
    struct {
        uint32_t AFR8          : 4;    // Bits 0-3: Alternate function selection for port x bit 0
        uint32_t AFR9          : 4;    // Bits 4-7
        uint32_t AFR10         : 4;    // Bits 8-11
        uint32_t AFR11         : 4;    // Bits 12-15
        uint32_t AFR12         : 4;    // Bits 16-19
        uint32_t AFR13        : 4;    // Bits 20-23
        uint32_t AFR14         : 4;    // Bits 24-27
        uint32_t AFR15        : 4;    // Bits 28-31
    } BIT;
} GPIO_AFRH_t;


/******************************************************************************
 * GPIO Register Structure
 *****************************************************************************/
typedef struct {
    volatile GPIO_MODER_t           MODER;      // 0x00: GPIO port mode register
    volatile GPIO_OTYPER_t         OTYPER;      // 0x04: GPIO port output type register
    volatile GPIO_OSPEEDR_t       OSPEEDR;      // 0x08: GPIO port output speed register
    volatile GPIO_PUPDR_t           PUPDR;      // 0x0C: GPIO port pull-up/pull-down register
    volatile GPIO_IDR_t               IDR;      // 0x10: GPIO port input data register
    volatile GPIO_ODR_t               ODR;      // 0x14: GPIO port output data register
    volatile GPIO_BSRR_t             BSRR;      // 0x18: GPIO port bit set/reset register
    volatile GPIO_LCKR_t             LCKR;      // 0x1C: GPIO port configuration lock register
    volatile GPIO_AFRL_t             AFRL;      // 0x20: GPIO alternate function low register (pins 0-7)
    volatile GPIO_AFRH_t             AFRH;      // 0x24: GPIO alternate function high register (pins 8-15)
} GPIO_TypeDef;



/******************************************************************************
 * GPIO Peripheral Declarations
 *****************************************************************************/

 #define GPIOA    ((GPIO_TypeDef *) GPIOA_BASE_ADDRESS)
 #define GPIOB    ((GPIO_TypeDef *) GPIOB_BASE_ADDRESS)
 #define GPIOC    ((GPIO_TypeDef *) GPIOC_BASE_ADDRESS)
 #define GPIOD    ((GPIO_TypeDef *) GPIOD_BASE_ADDRESS)
 #define GPIOE    ((GPIO_TypeDef *) GPIOE_BASE_ADDRESS)
 #define GPIOH    ((GPIO_TypeDef *) GPIOH_BASE_ADDRESS)




/* GPIO Port Selection */
typedef enum {
    GPIO_PORTA = 0,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD,
    GPIO_PORTE,
    GPIO_PORTH
} GPIO_Port_t;

/* GPIO Pin Number */
typedef enum {
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
    GPIO_PIN8,
    GPIO_PIN9,
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN14,
    GPIO_PIN15
} GPIO_Pin_t;
/* GPIO Pin Mode */
typedef enum{
    GPIO_MODE_INTPUT =0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATVE,
    GPIO_MODE_ANALOG
}GPIO_Mode_t;

/* GPIO Output Type */
typedef enum
{
    GPIO_OUTPUT_PUSHPULL = 0,
    GPIO_OUTPUT_OPENDRAIN
}GPIO_OutputType_t;

/* GPIO Output Speed */
typedef enum{
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERYHIGH
}GPIO_Speed_t;


/* GPIO Pull-up/Pull-down */
typedef enum {
    GPIO_PUPD_NONE = 0,         // No pull-up, pull-down
    GPIO_PUPD_PULLUP,           // Pull-up
    GPIO_PUPD_PULLDOWN          // Pull-down
} GPIO_PuPd_t;


/* GPIO Pin State */
typedef enum {
    GPIO_PIN_RESET = 0,         // Low level
    GPIO_PIN_SET                // High level
} GPIO_PinState_t;


/* GPIO Alternate Function */
typedef enum {
    GPIO_AF0_SYSTEM = 0,
    GPIO_AF1_TIM1_TIM2,    
    GPIO_AF2_TIM3___5,
    GPIO_AF3_TIM9___9,
    GPIO_AF4_I2C1___3,
    GPIO_AF5_SPI1___4,
    GPIO_AF6_SPI3,
    GPIO_AF7_USART1_2,
    GPIO_AF8_USART6,
    GPIO_AF9_I2C2_3,
    GPIO_AF10_OTG_FS,
    GPIO_AF11,
    GPIO_AF12_SDIO,
    GPIO_AF13,
    GPIO_AF14,
    GPIO_AF15_EVENTOUT
}GPIO_AF_t;



/* GPIO Pin Configuration Structure */
typedef struct {
    GPIO_TypeDef *      Port;           // GPIO Port (A, B, C, D, E, H)
    GPIO_Pin_t          Pin;            // GPIO Pin (0-15)
    GPIO_Mode_t         Mode;           // Pin mode
    GPIO_OutputType_t   OutputType;     // Output type (for output mode)
    GPIO_Speed_t        Speed;          // Output speed
    GPIO_PuPd_t         PuPd;           // Pull-up/Pull-down
    GPIO_AF_t           AlternateFunc;  // Alternate function (for AF mode)
} GPIO_PinConfig_t;


/* GPIO PORT configuration strcuture */
// In your header file
typedef struct {
    GPIO_PinConfig_t Pins[16];
} GPIO_PortConfig_t;

/* GPIO Status */
typedef enum {
    GPIO_OK = 0,
    GPIO_ERROR,
    GPIO_INVALID_PARAM
} GPIO_Status_t;


#endif

#ifdef __cplusplus

    }
}
#endif