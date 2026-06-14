
#ifdef c__cplusplus
namespace MCAL{
    namespace GPIO{
#endif


#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H


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

#include "../../private/MCAL/GPIO_Private.h"

void GPIO_PinInit(GPIO_TypeDef* GPIOx, GPIO_PinConfig_t* PinConfig);
void GPIO_PortInit(GPIO_TypeDef* GPIOx, GPIO_PinConfig_t* PinConfigArray);

/* ============================================
 * Read Functions
 * ============================================ */
uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t PinNumber);
uint16_t GPIO_ReadPort(GPIO_TypeDef* GPIOx);

/* ============================================
 * Write Functions
 * ============================================ */
void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t PinNumber, uint8_t Value);
void GPIO_WritePort(GPIO_TypeDef* GPIOx, uint16_t Value);
void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t PinNumber);

/* ============================================
 * Atomic Operations 
 * ============================================ */
void GPIO_SetPin(GPIO_TypeDef* GPIOx, uint16_t PinNumber);
void GPIO_ResetPin(GPIO_TypeDef* GPIOx, uint16_t PinNumber);

/* ============================================
 * Clock Control
 * ============================================ */
void GPIO_ClockEnable(GPIO_TypeDef* GPIOx);

#endif


#ifdef __cplusplus

    }
}
#endif