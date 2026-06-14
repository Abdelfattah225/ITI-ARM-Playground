#include "../../../include/configuration/HAL/LCD_Cfg.h"
#include "../../../include/interface/HAL/LCD.h"

#include "../../../include/interface/MCAL/GPIO_Interface.h"

LCD_Config LCDs[LCDs_Max];

LCD_Config LCDs[LCDs_Max]={
    [CLCD]={
        .RS_Port = GPIOA,
        .RS_Pin = GPIO_PIN0,
        .E_Port = GPIOA,
        .E_Pin = GPIO_PIN2,
        .Data_Port[0]=GPIOB,
        .Data_Pin[0]=GPIO_PIN0,
        .Data_Port[1]=GPIOB,
        .Data_Pin[1]=GPIO_PIN1,
        .Data_Port[2]=GPIOB,
        .Data_Pin[2]=GPIO_PIN2,
        .Data_Port[3]=GPIOB,
        .Data_Pin[3]=GPIO_PIN3,
        .Data_Port[4]=GPIOB,
        .Data_Pin[4]=GPIO_PIN4,
        .Data_Port[5]=GPIOB,
        .Data_Pin[5]=GPIO_PIN5,
        .Data_Port[6]=GPIOB,
        .Data_Pin[6]=GPIO_PIN6,
        .Data_Port[7]=GPIOB,
        .Data_Pin[7]=GPIO_PIN7,

        .mode = LCD_MODE_4BIT,
    }
};

GPIO_PinConfig_t LCD_CTRLPINS[5]={
   
    [LCD_RS]={
        .Port = GPIOA,
            .Pin = GPIO_PIN0,
            .Mode = GPIO_MODE_OUTPUT,
            .OutputType = GPIO_OUTPUT_PUSHPULL,
            .Speed = GPIO_SPEED_HIGH,
            .PuPd = GPIO_PUPD_NONE
    },
   
    [LCD_EN]={
        .Port = GPIOA,
            .Pin = GPIO_PIN2,
            .Mode = GPIO_MODE_OUTPUT,
            .OutputType = GPIO_OUTPUT_PUSHPULL,
            .Speed = GPIO_SPEED_HIGH,
            .PuPd = GPIO_PUPD_NONE
    }
};
