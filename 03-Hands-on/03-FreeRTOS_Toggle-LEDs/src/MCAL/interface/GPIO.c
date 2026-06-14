
#ifdef __cplusplus
namespace MCAL{
    namespace GPIO{
#endif



#include "../../../include/interface/MCAL/GPIO_Interface.h"
// RCC dirver 
#include "../../../include/interface/MCAL/rcc_init.h"


void GPIO_PinInit(GPIO_TypeDef * GPIOx, GPIO_PinConfig_t* PinConfig){
/*Enable clock for the port */
GPIO_ClockEnable(GPIOx);

/** MODER Register  */
    uint32_t temp;
    // read current value
    temp = GPIOx->MODER.REG;
    // clear two bits
    temp &= ~(0x3 << (PinConfig->Pin *2));
    // set new mode
    temp |= (PinConfig->Mode<< (PinConfig->Pin *2));
    // write back
    GPIOx->MODER.REG = temp;

   


    /* PUPUR */
    // read register
    temp = GPIOx->PUPDR.REG ;
    // clear two bits
    temp &= ~(0x3 << (PinConfig->Pin *2));
     // set new value
    temp |= (PinConfig->PuPd << (PinConfig->Pin *2));
    // write back
    GPIOx->PUPDR.REG = temp ;

   if ((PinConfig->Mode == GPIO_MODE_OUTPUT) || (PinConfig->Mode == GPIO_MODE_ALTERNATVE) )
   {
    /** SOPEEDR */
    // read register
    temp = GPIOx->OSPEEDR.REG ;
     // clear two bits
    temp &= ~(0x3 << (PinConfig->Pin *2));
    // set new speed
    temp |= (PinConfig->Speed<< (PinConfig->Pin *2));
    // write back
    GPIOx->OSPEEDR.REG = temp ;



    
    /*OTYPER*/
        if(PinConfig->OutputType == GPIO_OUTPUT_PUSHPULL)
        {
            // clear the bit
            GPIOx->OTYPER.REG &= ~(1<<PinConfig->Pin);
        }
        else if(PinConfig->OutputType == GPIO_OUTPUT_OPENDRAIN)
        {
            //set bit
            GPIOx->OTYPER.REG |= (1<<PinConfig->Pin);
        }


   }
if(PinConfig->Mode == GPIO_MODE_ALTERNATVE)
    {
        if(PinConfig->Pin <= 7 )
        {
            temp = GPIOx->AFRL.REG ;
            temp &= ~(0xF << (PinConfig->Pin * 4));
            temp |= (PinConfig->AlternateFunc << (PinConfig->Pin * 4));
            GPIOx->AFRL.REG = temp;

        }
        else 
        {
            temp = GPIOx->AFRH.REG ;
            temp &= ~(0xF << ((PinConfig->Pin -8) * 4));
            temp |= (PinConfig->AlternateFunc << ((PinConfig->Pin -8)* 4));
            GPIOx->AFRH.REG = temp;
        }
    }

    /**/

}

void GPIO_PortInit(GPIO_TypeDef* GPIOx, GPIO_PinConfig_t* PinConfigArray)
{
   uint8_t i;
    
    for (i = 0; i <= 15; i++)
    {
        GPIO_PinInit(GPIOx, &PinConfigArray[i]);
    }
    
}



/* ============================================
 * Read Functions
 * ============================================ */

uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t PinNumber)
{
    return (GPIOx->IDR.REG >> PinNumber) & 0x1;
}

uint16_t GPIO_ReadPort(GPIO_TypeDef* GPIOx)
{
    return (uint16_t)(GPIOx->IDR.REG);
}


/* ============================================
 * Write Functions
 * ============================================ */

void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t PinNumber, uint8_t Value){

    if(Value)
    {
         // set bit
        GPIOx->ODR.REG |= (1<< PinNumber);
    }
    else
    {
          // clear bit
        GPIOx->ODR.REG &= ~(1<< PinNumber);
    }
}

void GPIO_WritePort(GPIO_TypeDef* GPIOx, uint16_t Value){
            GPIOx->ODR.REG = Value;
}

void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t PinNumber)
{
        GPIOx->ODR.REG ^= (1<< PinNumber);
}


/* ============================================
 * Atomic Operations 
 * ============================================ */

void GPIO_SetPin(GPIO_TypeDef* GPIOx, uint16_t PinNumber){
      // set bit
        GPIOx->BSRR.REG = (1<< PinNumber);
}

void GPIO_ResetPin(GPIO_TypeDef* GPIOx, uint16_t PinNumber){
        // reset bit
        GPIOx->BSRR.REG = (1<<( PinNumber +16));
}

/* ============================================
 * Clock Control
 * ============================================ */
void GPIO_ClockEnable(GPIO_TypeDef* GPIOx){

 
    if(GPIOx == GPIOA)
    {
        RCC->AHB1ENR.REG |= (1 << 0);
    }
    else if(GPIOx == GPIOB)
    {
        RCC->AHB1ENR.REG |= (1 << 1);
    }
    else if(GPIOx == GPIOC)
    {
        RCC->AHB1ENR.REG |= (1 << 2);
    }
    else if(GPIOx == GPIOD)
    {
        RCC->AHB1ENR.REG |= (1 << 3);
    }
    else if(GPIOx == GPIOE)
    {
        RCC->AHB1ENR.REG |= (1 << 4);
    }
   

}


#ifdef __cplusplus

    } // namespace GPIO
}// namespace MCAL

#endif