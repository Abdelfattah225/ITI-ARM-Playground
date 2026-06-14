
#ifdef c__cplusplus
namespace MCAL{
    namespace GPIO{
#endif


#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

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