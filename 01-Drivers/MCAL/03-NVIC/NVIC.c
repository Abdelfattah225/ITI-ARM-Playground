/******************************************************************************
 * @file    NVIC_program.c
 * @brief   NVIC Driver Implementation for STM32F401CC
 * @details Contains function implementations for NVIC driver
 * @author  Abdeflattah Moawed
 * @date    2025
 ******************************************************************************/

#include "../../../include/interface/MCAL/NVIC_Interface.h"
#include "../../../include/configuration/MCAL/NVIC_Cfg.h"
#include "../../../include/private/MCAL/NVIC_Private.h"

#define NULL (void*)0

/******************************************************************************
 * Function Impelementation
 ******************************************************************************/

/**
 * @brief  Enable a specific interrupt in the NVIC
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_EnableIRQ(IRQn_Type IRQn){
    if(IRQn >= NVIC_MAX_IRQn || IRQn < NVIC_MIN_IRQn )
    {
        return NVIC_INVALID_IRQ;
    }
    else{
        uint8_t RegIndex = IRQn/32 ;
        uint8_t BitPos = IRQn%32;

        NVIC_ISER(RegIndex) = (1U<<BitPos);

        return NVIC_OK;
    }
}
/**
 * @brief  Disable a specific interrupt in the NVIC
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_DisableIRQ(IRQn_Type IRQn){
     if(IRQn >= NVIC_MAX_IRQn || IRQn < NVIC_MIN_IRQn )
    {
        return NVIC_INVALID_IRQ;
    }
    else{
        uint8_t RegIndex = IRQn/32 ;
        uint8_t BitPos = IRQn%32;

        NVIC_ICER(RegIndex) = (1U<<BitPos);

        return NVIC_OK;
    }
}

/**
 * @brief  Set pending status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_SetPendingIRQ(IRQn_Type IRQn){
     if(IRQn >= NVIC_MAX_IRQn || IRQn < NVIC_MIN_IRQn )
    {
        return NVIC_INVALID_IRQ;
    }
    else{
        uint8_t RegIndex = IRQn/32 ;
        uint8_t BitPos = IRQn%32;
        NVIC_ISPR(RegIndex) = (1U<<BitPos);

        return NVIC_OK;
    }
}

/**
 * @brief  Clear pending status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_ClearPendingIRQ(IRQn_Type IRQn){
        if(IRQn >= NVIC_MAX_IRQn || IRQn < NVIC_MIN_IRQn )
    {
        return NVIC_INVALID_IRQ;
    }
    else{
        uint8_t RegIndex = IRQn/32 ;
        uint8_t BitPos = IRQn%32;
        NVIC_ICPR(RegIndex) = (1U<<BitPos);

        return NVIC_OK;
    }
}

/**
 * @brief  Get pending status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval uint8_t: 1 if interrupt is pending, 0 otherwise
 */
uint8_t NVIC_GetPendingIRQ(IRQn_Type IRQn){
    
        uint8_t RegIndex = IRQn/32 ;
        uint8_t BitPos = IRQn%32;
        if(NVIC_ISPR(RegIndex) &= (1U<<BitPos)){
                return PENDING;
        }
        else
        {
                return NOTPENDING;
        }

    }


/**
 * @brief  Get active status for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval uint8_t: 1 if interrupt is active, 0 otherwise
 */
uint8_t NVIC_GetActiveIRQ(IRQn_Type IRQn){
        uint8_t RegIndex = IRQn/32 ;
        uint8_t BitPos = IRQn%32;
        if(NVIC_IABR(RegIndex) &= (1U<<BitPos) ){
                return ACTIVE;
        }
        else
        {
                return NOTACTIVE;
        }
}

/**
 * @brief  Set priority for a specific interrupt
 * @param  IRQn: InPinPosterrupt number (use IRQn_Type from NVIC_private.h)
 * @param  priority: Priority value (0-15, where 0 is highest priority)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */
NVIC_ErrorStatus_t NVIC_SetPriority(IRQn_Type IRQn, NVIC_Priority_t priority){
    if(IRQn >= NVIC_MAX_IRQn || IRQn < NVIC_MIN_IRQn )
    {
        return NVIC_INVALID_IRQ;
    }
    else if(priority > NVIC_PRIORITY_15)
    {
        return NVIC_INVALID_PRIORITY;
    }
    else
    {
        NVIC_IPR(IRQn) = (priority << 4) & NVIC_PRIORITY_MASK ;
        return NVIC_OK;
    }
}

/**
 * @brief  Get priority for a specific interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval uint8_t: Priority value (0-15)
 */
NVIC_Priority_t NVIC_GetPriority(IRQn_Type IRQn){
    
    return (NVIC_Priority_t)((NVIC_IPR(IRQn) >> 4) & 0x0F);
}

/**
 * @brief  Initialize NVIC with configuration from NVIC_config.h
 * @param  None
 * @retval NVIC_ErrorStatus_t: Status of the operation
 */

 NVIC_ErrorStatus_t NVIC_Init(void)
{
    NVIC_ErrorStatus_t status = NVIC_OK;
    
    /* Configure and enable interrupts based on configuration */
    #if EXTI0_ENABLE == 1
    #ifdef EXTI0_PRIORITY
    status |= NVIC_SetPriority(EXTI0_IRQn, EXTI0_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(EXTI0_IRQn);
    #endif
    
    #if EXTI1_ENABLE == 1
    #ifdef EXTI1_PRIORITY
    status |= NVIC_SetPriority(EXTI1_IRQn, EXTI1_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(EXTI1_IRQn);
    #endif
    
    #if EXTI2_ENABLE == 1
    #ifdef EXTI2_PRIORITY
    status |= NVIC_SetPriority(EXTI2_IRQn, EXTI2_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(EXTI2_IRQn);
    #endif
    
    #if EXTI3_ENABLE == 1
    #ifdef EXTI3_PRIORITY
    status |= NVIC_SetPriority(EXTI3_IRQn, EXTI3_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(EXTI3_IRQn);
    #endif
    
    #if EXTI4_ENABLE == 1
    #ifdef EXTI4_PRIORITY
    status |= NVIC_SetPriority(EXTI4_IRQn, EXTI4_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(EXTI4_IRQn);
    #endif
    
    #if EXTI9_5_ENABLE == 1
    #ifdef EXTI9_5_PRIORITY
    status |= NVIC_SetPriority(EXTI9_5_IRQn, EXTI9_5_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(EXTI9_5_IRQn);
    #endif
    
    #if EXTI15_10_ENABLE == 1
    #ifdef EXTI15_10_PRIORITY
    status |= NVIC_SetPriority(EXTI15_10_IRQn, EXTI15_10_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(EXTI15_10_IRQn);
    #endif
    
    #if TIM2_ENABLE == 1
    #ifdef TIM2_PRIORITY
    status |= NVIC_SetPriority(TIM2_IRQn, TIM2_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(TIM2_IRQn);
    #endif
    
    #if TIM3_ENABLE == 1
    #ifdef TIM3_PRIORITY
    status |= NVIC_SetPriority(TIM3_IRQn, TIM3_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(TIM3_IRQn);
    #endif
    
    #if TIM4_ENABLE == 1
    #ifdef TIM4_PRIORITY
    status |= NVIC_SetPriority(TIM4_IRQn, TIM4_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(TIM4_IRQn);
    #endif
    
    #if TIM5_ENABLE == 1
    #ifdef TIM5_PRIORITY
    status |= NVIC_SetPriority(TIM5_IRQn, TIM5_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(TIM5_IRQn);
    #endif
    
    #if USART1_ENABLE == 1
    #ifdef USART1_PRIORITY
    status |= NVIC_SetPriority(USART1_IRQn, USART1_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(USART1_IRQn);
    #endif
    
    #if USART2_ENABLE == 1
    #ifdef USART2_PRIORITY
    status |= NVIC_SetPriority(USART2_IRQn, USART2_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(USART2_IRQn);
    #endif
    
    #if USART6_ENABLE == 1
    #ifdef USART6_PRIORITY
    status |= NVIC_SetPriority(USART6_IRQn, USART6_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(USART6_IRQn);
    #endif
    
    #if SPI1_ENABLE == 1
    #ifdef SPI1_PRIORITY
    status |= NVIC_SetPriority(SPI1_IRQn, SPI1_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(SPI1_IRQn);
    #endif
    
    #if SPI2_ENABLE == 1
    #ifdef SPI2_PRIORITY
    status |= NVIC_SetPriority(SPI2_IRQn, SPI2_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(SPI2_IRQn);
    #endif
    
    #if SPI3_ENABLE == 1
    #ifdef SPI3_PRIORITY
    status |= NVIC_SetPriority(SPI3_IRQn, SPI3_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(SPI3_IRQn);
    #endif
    
    #if SPI4_ENABLE == 1
    #ifdef SPI4_PRIORITY
    status |= NVIC_SetPriority(SPI4_IRQn, SPI4_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(SPI4_IRQn);
    #endif
    
    #if I2C1_EV_ENABLE == 1
    #ifdef I2C1_EV_PRIORITY
    status |= NVIC_SetPriority(I2C1_EV_IRQn, I2C1_EV_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(I2C1_EV_IRQn);
    #endif
    
    #if I2C1_ER_ENABLE == 1
    #ifdef I2C1_ER_PRIORITY
    status |= NVIC_SetPriority(I2C1_ER_IRQn, I2C1_ER_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(I2C1_ER_IRQn);
    #endif
    
    #if I2C2_EV_ENABLE == 1
    #ifdef I2C2_EV_PRIORITY
    status |= NVIC_SetPriority(I2C2_EV_IRQn, I2C2_EV_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(I2C2_EV_IRQn);
    #endif
    
    #if I2C2_ER_ENABLE == 1
    #ifdef I2C2_ER_PRIORITY
    status |= NVIC_SetPriority(I2C2_ER_IRQn, I2C2_ER_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(I2C2_ER_IRQn);
    #endif
    
    #if ADC_ENABLE == 1
    #ifdef ADC_PRIORITY
    status |= NVIC_SetPriority(ADC_IRQn, ADC_PRIORITY);
    #endif
    status |= NVIC_EnableIRQ(ADC_IRQn);
    #endif
    
    return (status == NVIC_OK) ? NVIC_OK : NVIC_ERROR;
}

/**
 * @brief  Trigger a software interrupt
 * @param  IRQn: Interrupt number (use IRQn_Type from NVIC_private.h)
 * @retval NVIC_ErrorStatus_t: Status of the operation
 * @note   This is equivalent to NVIC_SetPendingIRQ
 */
NVIC_ErrorStatus_t NVIC_TriggerSWI(IRQn_Type IRQn){
    return NVIC_SetPendingIRQ(IRQn);
}


