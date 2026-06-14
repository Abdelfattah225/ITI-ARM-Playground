#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/MCAL/GPIO_Interface.h"
#include "../include/interface/HAL/LED.h"
#include "../include/interface/HAL/LCD.h"
#include "../include/interface/MCAL/SYSTICK_Interface.h"
#include "./os/sched.h"

/* External LCD configurations */
extern LCD_Config LCDs[LCDs_Max];

/* Custom character - Heart symbol */
uint8_t heart_char[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000
};

/* Custom character - Smiley face */
uint8_t smiley_char[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

// Define a Custom Character (e.g., a Smiley Face)
uint8_t SmileyFace[8] = {
    0x00, 0x0A, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00
};

// --------------------------------------------------------
// Test Application Runnable
// --------------------------------------------------------
void App_Test_LCD(void *arg)
{
    static uint8_t step = 0;
    LCD_Status_t status;

    switch (step)
    {
    case 0:
        // 1. Initialize the LCD
        status = LCD_AsynchInit(&LCDs[CLCD]);
        if (status == LCD_IN_PROGRESS) {
            step++;
        }
        break;

    case 1:
        // 2. Write first string
        // We keep retrying every 5ms until the LCD is ready (not BUSY)
        status = LCD_AsynchWriteString(&LCDs[CLCD], (uint8_t *)"Hello Kishk ");
        if (status == LCD_IN_PROGRESS) {
            step++;
        }
        break;

    case 2:
        // 3. Move Cursor to 2nd Line
        status = LCD_AsynchSetCursorPosition(&LCDs[CLCD], 1, 0);
        if (status == LCD_OK || status == LCD_IN_PROGRESS) { // Check both depending on logic
            step++;
        }
        break;

    case 3:
        // 4. Write second string
        status = LCD_AsynchWriteString(&LCDs[CLCD], (uint8_t *)"Status: ");
        if (status == LCD_IN_PROGRESS) {
            step++;
        }
        break;

    case 4:
        // 5. Define Custom Char at Location 0
        status = LCD_AsynchSetCustomChar(&LCDs[CLCD], 0, SmileyFace);
        if (status == LCD_IN_PROGRESS) {
            step++;
        }
        break;

    case 5:
        // 6. Display the Custom Char
        status = LCD_AsynchDislayCustomChar(&LCDs[CLCD], 0);
        if (status == LCD_OK || status == LCD_IN_PROGRESS) {
            step++; // Test Finished
        }
        break;

    case 6:
        // Idle - Test Complete
        break;
    }
}

// Create the Runnable Object for the App
Runnable_t App_Runnable = {
    .fnc = App_Test_LCD,
    .periodicity_ms = 1,  // Check application logic every 5ms
    .first_delay_ms = 0, // Small delay at start
    .priority = 2,        // Priority 2 (LCD Driver is Priority 1)
    .arg = NULL
};

int main(void)
{
    // 1. Initialize System Clock (Enable GPIOA and GPIOB for LCD)
    RCC_Init();
    
    // 2. Initialize Scheduler (Tick time = 1ms)
    Sched_Init(1);

    // 3. Register our Test Application
    Sched_Register_Runnable(&App_Runnable);

    // 4. Start the Scheduler (This function never returns)
    Sched_Start();

    while (1)
    {
        // Code should never reach here
    }
    return 0;
}