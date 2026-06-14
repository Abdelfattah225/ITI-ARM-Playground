

#ifdef __cplusplus
namespace MCAL{
    namespace GPIO{
#endif



/******************************************************************************
 * File: GPIO_cfg.h
 * Description: GPIO Driver Configuration for STM32F401CC
 *****************************************************************************/
#ifndef GPIO_CFG_H
#define GPIO_CFG_H

#include "../../private/MCAL/GPIO_Private.h"


/******************************************************************************
 * Pin Configuration Presets
 *****************************************************************************/

/* Example: LED Configuration (PC13 - typical onboard LED) */
// #define LED_PORT                GPIO_PORTC
// #define LED_PIN                 GPIO_PIN13

// GPIO_PinConfig_t LED = {
//     .Port = LED_PORT,                      
//     .Pin = GPIO_PIN13,                        
//     .Mode = GPIO_MODE_OUTPUT,                
//     .OutputType = GPIO_OUTPUT_PUSHPULL,      
//     .Speed = GPIO_SPEED_MEDIUM,              
//     .PuPd = GPIO_PUPD_NONE,                  
//     .AlternateFunc = GPIO_AF0_SYSTEM     
// }
/* Example: Button Configuration (PA0 - typical user button) */
#define BUTTON_PORT             GPIO_PORTA
#define BUTTON_PIN              GPIO_PIN0

/* Example: USART1 TX/RX Pins */
#define USART1_TX_PORT          GPIO_PORTA
#define USART1_TX_PIN           GPIO_PIN9
#define USART1_TX_AF            GPIO_AF7

#define USART1_RX_PORT          GPIO_PORTA
#define USART1_RX_PIN           GPIO_PIN10
#define USART1_RX_AF            GPIO_AF7

/* Example: SPI1 Pins */
#define SPI1_SCK_PORT           GPIO_PORTA
#define SPI1_SCK_PIN            GPIO_PIN5
#define SPI1_SCK_AF             GPIO_AF5

#define SPI1_MISO_PORT          GPIO_PORTA
#define SPI1_MISO_PIN           GPIO_PIN6
#define SPI1_MISO_AF            GPIO_AF5

#define SPI1_MOSI_PORT          GPIO_PORTA
#define SPI1_MOSI_PIN           GPIO_PIN7
#define SPI1_MOSI_AF            GPIO_AF5

/* Example: I2C1 Pins */
#define I2C1_SCL_PORT           GPIO_PORTB
#define I2C1_SCL_PIN            GPIO_PIN6
#define I2C1_SCL_AF             GPIO_AF4

#define I2C1_SDA_PORT           GPIO_PORTB
#define I2C1_SDA_PIN            GPIO_PIN7
#define I2C1_SDA_AF             GPIO_AF4




/******************************************************************************
 * Configuration Options
 *****************************************************************************/

/* Enable/Disable parameter checking */
#define GPIO_PARAM_CHECK_ENABLE         1  // 0=Disabled, 1=Enabled

#endif /* GPIO_CFG_H */


#ifdef __cplusplus

    }
}
#endif