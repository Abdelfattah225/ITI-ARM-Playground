#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/MCAL/GPIO_Interface.h"
#include "../include/interface/HAL/LED.h"
#include "../include/interface/HAL/LCD.h"
#include "../include/interface/MCAL/SYSTICK_Interface.h"

extern LCD_Config LCDs[LCDs_Max];

// Define a custom character (e.g., a "Smiley Face")
// 5x8 pixel map
uint8_t smiley_face[8] = {
    0b00000,
    0b01010, // Eyes
    0b01010,
    0b00000,
    0b10001, // Smile corners
    0b01110, // Smile bottom
    0b00000,
    0b00000
};

// Define a custom character (e.g., a "Bell")
uint8_t bell_icon[8] = {
    0b00100,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b00000,
    0b00100,
    0b00000
};

int main(void)
{
    RCC_Init();
    SYSTICK_Init();
    LCD_Init(&LCDs[CLCD]);

    LCD_Clear(&LCDs[CLCD]);
    // Write to Line 1
    LCD_WriteString(&LCDs[CLCD], (uint8_t*)"LCD Driver Test");
    LCD_SetCursorPosition(&LCDs[CLCD], 1, 0);
    LCD_WriteString(&LCDs[CLCD], (uint8_t*)"Mode: 8-Bit");


    // Upload Smiley to CGRAM location 0
    LCD_SetCustomChar(&LCDs[CLCD], 0, smiley_face);
    
    // Upload Bell to CGRAM location 1
    LCD_SetCustomChar(&LCDs[CLCD], 1, bell_icon);

    SYSTICK_Delay_ms(2000);
        LCD_Clear(&LCDs[CLCD]);

    // Move Cursor to Line 2
    LCD_SetCursorPosition(&LCDs[CLCD], 1, 5);

    // Display the Custom Characters
    // Note: We use LCD_WriteChar to send the Index (0 and 1)
    LCD_DislayCustomChar(&LCDs[CLCD], 0); // Display Smiley
    LCD_WriteChar(&LCDs[CLCD], ' '); 
    LCD_DislayCustomChar(&LCDs[CLCD], 1); // Display Bell


        while(1){
            }
return 0;
}