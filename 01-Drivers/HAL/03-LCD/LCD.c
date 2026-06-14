#include "../../../include/interface/HAL/LCD.h"
#include "../../../include/configuration/HAL/LCD_Cfg.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"
#include "../../../include/interface/MCAL/SYSTICK_Interface.h"
#include "../../../src/os/sched.h"

#define NULL ((void*)0)

static LCD_State_t LCD_State = NO_ACTION;
static LCD_Init_Seq_t LCD_Init_Seq = NO_ACTION;
static LCD_Write_Str_Seq_t LCD_Write_Str_Seq =Str_SendData1;
static LCD_Write_Char_Seq_t LCD_Write_Char_Seq = Char_SendData1;
static LCD_Write_cmd_t LCD_Send_CMD = SendCmd1; 
static uint8_t *Saved_String;
static uint8_t *Saved_Char;
static uint8_t Saved_CMD;
static LCD_Config *Global_LCD;
static uint8_t is_writing_Custom = 0;

static uint8_t CustomChar_Buffer[8]; // Buffer to hold the pattern
static uint8_t CustomChar_Index = 0; // To count 0-7 bytes
static uint8_t CustomChar_Location = 0; // To hold the address (0-7)

// Define the Runnable Object
static Runnable_t LCD_Runnable_Obj = {
    .fnc = (cbf_t)LCD_AsynchRunnable, // Cast to generic callback type
    .periodicity_ms = 2,              // Run every 2ms
    .first_delay_ms = 0,
    .priority = 1,
    .arg = NULL                     // Will be set in Init
};

/* -------- Basic Commands -------- */
#define LCD_CLEAR_DISPLAY 0x01 // Clear display
#define LCD_RETURN_HOME 0x02   // Return cursor to home

/* -------- Address Commands -------- */
#define LCD_SET_CGRAM_ADDR 0x40 // Set CGRAM address (+ 6-bit addr)
#define LCD_SET_DDRAM_ADDR 0x80 // Set DDRAM address (+ 7-bit addr)

/* -------- DDRAM Line Addresses -------- */
#define LCD_LINE1 0x00 // Line 1 start address
#define LCD_LINE2 0x40 // Line 2 start address
#define LCD_LINE3 0x14 // Line 3 (20x4 LCD)
#define LCD_LINE4 0x54 // Line 4 (20x4 LCD)

/*============================================
 * COMMONLY USED COMBINED COMMANDS
 *============================================*/

/* Entry Mode */
#define LCD_ENTRY_MODE_INC_NOSHIFT 0x06 // Increment, no shift
#define LCD_ENTRY_MODE_DEC_NOSHIFT 0x04 // Decrement, no shift
#define LCD_ENTRY_MODE_INC_SHIFT 0x07   // Increment with shift

/* Display Control */
#define LCD_DISPLAY_ON_CURSOR_OFF 0x0C   // Display ON, cursor OFF
#define LCD_DISPLAY_ON_CURSOR_ON 0x0E    // Display ON, cursor ON
#define LCD_DISPLAY_ON_CURSOR_BLINK 0x0F // Display ON, cursor blink

/* Function Set */
#define LCD_FUNCTION_8BIT_2LINE 0x38 // 8-bit, 2 lines, 5x8 font
#define LCD_FUNCTION_8BIT_1LINE 0x30 // 8-bit, 1 line, 5x8 font
#define LCD_FUNCTION_4BIT_2LINE 0x28 // 4-bit, 2 lines, 5x8 font
#define LCD_FUNCTION_4BIT_1LINE 0x20 // 4-bit, 1 line, 5x8 font

/* Cursor/Display Shift */
#define LCD_SHIFT_CURSOR_LEFT 0x10   // Move cursor left
#define LCD_SHIFT_CURSOR_RIGHT 0x14  // Move cursor right
#define LCD_SHIFT_DISPLAY_LEFT 0x18  // Shift display left
#define LCD_SHIFT_DISPLAY_RIGHT 0x1C // Shift display right

#define NULL ((void *)0)

// helper functions
static LCD_Status_t LCD_SendCommand(LCD_Config *config, uint8_t command);

static LCD_Status_t LCD_SendData(LCD_Config *config, uint8_t data);

static LCD_Status_t LCD_TriggerEnablePin(LCD_Config *config);

extern GPIO_PinConfig_t LCD_CTRLPINS[2];

LCD_Status_t LCD_Init(LCD_Config *config)
{
    for (uint8_t i = 0; i <= LCD_EN; i++)
    {
        GPIO_PinInit(LCD_CTRLPINS[i].Port, &LCD_CTRLPINS[i]);
    }

    if (config == NULL)
    {
        return LCD_ERROR;
    }

    // Initialization sequence based on mode
    if (config->mode == LCD_MODE_8BIT)
    {

        // Initialize data pins as output

        for (uint8_t i = 0; i < 8; i++)
        {
            GPIO_PinConfig_t pinConfig;
            pinConfig.Port = config->Data_Port[i];
            pinConfig.Pin = config->Data_Pin[i];
            pinConfig.Mode = GPIO_MODE_OUTPUT;
            pinConfig.OutputType = GPIO_OUTPUT_PUSHPULL;
            pinConfig.Speed = GPIO_SPEED_HIGH;
            pinConfig.PuPd = GPIO_PUPD_NONE;
            GPIO_PinInit(pinConfig.Port, &pinConfig);
        }

        // Function Set: 8-bit, 2 lines, 5x8 font
        LCD_SendCommand(config, LCD_FUNCTION_8BIT_2LINE);
    }
    else if (config->mode == LCD_MODE_4BIT)
    {
        // Initialize data pins as output
        for (uint8_t i = 0; i < 4; i++)
        {
            GPIO_PinConfig_t pinConfig;
            pinConfig.Port = config->Data_Port[i + 4];
            pinConfig.Pin = config->Data_Pin[i + 4];
            pinConfig.Mode = GPIO_MODE_OUTPUT;
            pinConfig.OutputType = GPIO_OUTPUT_PUSHPULL;
            pinConfig.Speed = GPIO_SPEED_HIGH;
            pinConfig.PuPd = GPIO_PUPD_NONE;
            GPIO_PinInit(pinConfig.Port, &pinConfig);
        }
        // Function Set: 4-bit, 2 lines, 5x8 font
        LCD_SendCommand(config, 0x33); // Initialize
        LCD_SendCommand(config, 0x32); // Set to 4-bit mode
        LCD_SendCommand(config, LCD_FUNCTION_4BIT_2LINE);
    }

    // Display ON, Cursor OFF
    LCD_SendCommand(config, LCD_DISPLAY_ON_CURSOR_OFF);

    // Clear Display
    LCD_SendCommand(config, LCD_CLEAR_DISPLAY);
    // Entry Mode Set: Increment, no shift
    LCD_SendCommand(config, LCD_ENTRY_MODE_INC_NOSHIFT);

    return LCD_OK;
}

LCD_Status_t LCD_WriteChar(LCD_Config *config, uint8_t character)
{

    if (config == NULL)
    {
        return LCD_ERROR;
    }
    LCD_SendData(config, character);
    return LCD_OK;
}

LCD_Status_t LCD_WriteString(LCD_Config *config, const uint8_t *string)
{

    if (config == NULL || string == NULL)
    {
        return LCD_ERROR;
    }
    while (*string)
    {
        LCD_SendData(config, *string++);
    }
    return LCD_OK;
}

LCD_Status_t LCD_Clear(LCD_Config *config)
{
    if (config == NULL)
    {
        return LCD_ERROR;
    }
    LCD_SendCommand(config, LCD_CLEAR_DISPLAY);
    return LCD_OK;
}

LCD_Status_t LCD_SetCursorPosition(LCD_Config *config, uint8_t row, uint8_t column)
{
    if (config == NULL)
    {
        return LCD_ERROR;
    }
    uint8_t address;

    switch (row)
    {
    case 0:
        address = LCD_LINE1 + column;
        break;
    case 1:
        address = LCD_LINE2 + column;
        break;
    case 2:
        address = LCD_LINE3 + column;
        break;
    case 3:
        address = LCD_LINE4 + column;
        break;
    default:
        return LCD_ERROR; // Invalid row
    }

    LCD_SendCommand(config, LCD_SET_DDRAM_ADDR | address);
    return LCD_OK;
}

LCD_Status_t LCD_SetCustomChar(LCD_Config *config, uint8_t location, uint8_t charmap[])
{
    if (config == NULL || charmap == NULL || location > 7)
    {
        return LCD_ERROR;
    }
    // Set CGRAM address
    LCD_SendCommand(config, LCD_SET_CGRAM_ADDR | (location << 3));
    for (uint8_t i = 0; i < 8; i++)
    {
        LCD_SendData(config, charmap[i]);
    }
    return LCD_OK;
}

LCD_Status_t LCD_DislayCustomChar(LCD_Config *config, uint8_t location)
{
    if (config == NULL || location > 7)
    {
        return LCD_ERROR;
    }
    LCD_SendData(config, location);
    return LCD_OK;
}

// helper functions
static LCD_Status_t LCD_SendCommand(LCD_Config *config, uint8_t command)
{
    if (config == NULL)
    {
        return LCD_ERROR;
    }
    if (config->mode == LCD_MODE_8BIT)
    {
        // Rs = 0 for command
        GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);

        // Send full byte in 8-bit mode
        for (uint8_t i = 0; i < 8; i++)
        {
            GPIO_WritePin(config->Data_Port[i], config->Data_Pin[i], (command >> i) & 0x01);
        }
        // En
        LCD_TriggerEnablePin(config);
    }
    else if (config->mode == LCD_MODE_4BIT)
    {
        // Rs = 0 for command
        GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);

        // Send higher nibble
        for (uint8_t i = 0; i < 4; i++)
        {
            GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (command >> (i + 4) & 0x01));
        }
        // En
        LCD_TriggerEnablePin(config);
        SYSTICK_Delay_ms(2);
        // Send lower nibble
        for (uint8_t i = 0; i < 4; i++)
        {
            GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (command >> i & 0x01));
        }
        // En
        LCD_TriggerEnablePin(config);
    }
    return LCD_OK;
}

static LCD_Status_t LCD_SendData(LCD_Config *config, uint8_t data)
{
    if (config == NULL)
    {
        return LCD_ERROR;
    }
    if (config->mode == LCD_MODE_8BIT)
    {
        // Send full byte in 8-bit mode
        for (uint8_t i = 0; i < 8; i++)
        {
            GPIO_WritePin(config->Data_Port[i], config->Data_Pin[i], (data >> i) & 0x01);
        }
        // Rs = 1 for data
        GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_SET);

        // En
        LCD_TriggerEnablePin(config);
    }
    else if (config->mode == LCD_MODE_4BIT)
    {
        // Rs = 1 for data
        GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_SET);

        // Send higher nibble
        for (uint8_t i = 0; i < 4; i++)
        {
            GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (data >> (i + 4) & 0x01));
        }
        // En
        LCD_TriggerEnablePin(config);
        SYSTICK_Delay_ms(1);
        // Send lower nibble
        for (uint8_t i = 0; i < 4; i++)
        {
            GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (data >> i & 0x01));
        }
        // En
        LCD_TriggerEnablePin(config);
    }
    return LCD_OK;
}

static LCD_Status_t LCD_TriggerEnablePin(LCD_Config *config)
{
    if (config == NULL)
    {
        return LCD_ERROR;
    }
    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
    SYSTICK_Delay_ms(2);
    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
    SYSTICK_Delay_ms(2);
    return LCD_OK;
}

// Asynchronous functions
void LCD_AsynchRunnable(LCD_Config *config)
{

    switch (LCD_State)
    {
    case LCD_INIT:
        switch (LCD_Init_Seq)
        {
        case Function_Set_Init1:
            GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);
            // Send higher nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (0x33 >> (i + 4) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
            LCD_Init_Seq++;
            break;
        case Function_Set_Init2:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;
        case Function_Set_Init3:
            // Send lower nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], ((0x33 >> i) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

            LCD_Init_Seq++;
            break;
        case Function_Set_Init4:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;
        case Function_Set_4_bit_mode1:
            GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);
            // Send higher nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (0x32 >> (i + 4) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
            LCD_Init_Seq++;
            break;
        case Function_Set_4_bit_mode2:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;
        case Function_Set_4_bit_mode3:
            // Send lower nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], ((0x32 >> i) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

            LCD_Init_Seq++;
            break;
        case Function_Set_4_bit_mode4:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;

        case Function_Set_4BIT_2LINE1:
            GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);
            // Send higher nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (LCD_FUNCTION_4BIT_2LINE >> (i + 4) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
            LCD_Init_Seq++;
            break;
        case Function_Set_4BIT_2LINE2:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;
        case Function_Set_4BIT_2LINE3:
            // Send lower nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], ((LCD_FUNCTION_4BIT_2LINE >> i) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

            LCD_Init_Seq++;
            break;
        case Function_Set_4BIT_2LINE4:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;

            /*Display ON */

        case DISPLAY_ON_CURSOR_OFF1:
            GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);
            // Send higher nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (LCD_DISPLAY_ON_CURSOR_OFF >> (i + 4) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
            LCD_Init_Seq++;
            break;
        case DISPLAY_ON_CURSOR_OFF2:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;
        case DISPLAY_ON_CURSOR_OFF3:
            // Send lower nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], ((LCD_DISPLAY_ON_CURSOR_OFF >> i) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

            LCD_Init_Seq++;
            break;
        case DISPLAY_ON_CURSOR_OFF4:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;

            /* Clear LCD*/

        case CLEAR_DISPLAY1:
            GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);
            // Send higher nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (LCD_CLEAR_DISPLAY >> (i + 4) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
            LCD_Init_Seq++;
            break;
        case CLEAR_DISPLAY2:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;
        case CLEAR_DISPLAY3:
            // Send lower nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], ((LCD_CLEAR_DISPLAY >> i) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

            LCD_Init_Seq++;
            break;
        case CLEAR_DISPLAY4:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;

            /* Clear LCD*/

        case ENTRY_MODE_INC_NOSHIFT1:
            GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);
            // Send higher nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (LCD_CLEAR_DISPLAY >> (i + 4) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
            LCD_Init_Seq++;
            break;
        case ENTRY_MODE_INC_NOSHIFT2:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq++;
            break;
        case ENTRY_MODE_INC_NOSHIFT3:
            // Send lower nibble
            for (uint8_t i = 0; i < 4; i++)
            {
                GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], ((LCD_CLEAR_DISPLAY >> i) & 0x01));
            }
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

            LCD_Init_Seq++;
            break;
        case ENTRY_MODE_INC_NOSHIFT4:
            GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
            LCD_Init_Seq = INITDONE;
            LCD_State = NO_ACTION;
            break;
        default:
            break;
        }
        break;

    case LCD_WRITE_STR:

         if(Saved_String == NULL || *Saved_String == '\0') {
        LCD_State = NO_ACTION;
        break;
         }
        switch (LCD_Write_Str_Seq)
        {
                case Str_SendData1:
                    // Rs = 1 for data
                    GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_SET);

                    // Send higher nibble
                    for (uint8_t i = 0; i < 4; i++)
                    {
                        GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (*Saved_String >> (i + 4) & 0x01));
                    }
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

                    LCD_Write_Str_Seq++;
                    break;
                case Str_SendData2:
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
                    LCD_Write_Str_Seq++;

                    break;
                case Str_SendData3:
                    // Send lower nibble
                    for (uint8_t i = 0; i < 4; i++)
                    {
                        GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (*Saved_String >> i & 0x01));
                    }
                    // En
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
                    LCD_Write_Str_Seq++;
                
                    break;
                case Str_SendData4:
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
                    LCD_Write_Str_Seq = Str_SendData1;
                    Saved_String++;
                    break;
                default:
                    break;
        }  
        break;
    case LCD_WRITE_CHAR:
            switch (LCD_Write_Char_Seq)
            {
                 case Char_SendData1:
                    // Rs = 1 for data
                    GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_SET);

                    // Send higher nibble
                    for (uint8_t i = 0; i < 4; i++)
                    {
                        GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (*Saved_Char >> (i + 4) & 0x01));
                    }
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);

                    LCD_Write_Char_Seq++;
                    break;
                case Char_SendData2:
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
                    LCD_Write_Char_Seq++;

                    break;
                case Char_SendData3:
                    // Send lower nibble
                    for (uint8_t i = 0; i < 4; i++)
                    {
                        GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (*Saved_Char >> i & 0x01));
                    }
                    // En
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
                    LCD_Write_Char_Seq++;
                
                    break;
                case Char_SendData4:
                    GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
                    LCD_Write_Char_Seq = Char_SendData1;
                    if(is_writing_Custom == 1)
                    {
                        LCD_State = LCD_SET_CUSTOM;
                    }
                    else{
                        LCD_State = NO_ACTION;
                    }
                    break;
                default:
                    break;
            }
        break;
    case LCD_SENDCMD:
            
            switch (LCD_Send_CMD)
                {
                    case SendCmd1:
                        // Rs = 0 for command
                        GPIO_WritePin(config->RS_Port, config->RS_Pin, GPIO_PIN_RESET);

                        // Send higher nibble
                        for (uint8_t i = 0; i < 4; i++)
                        {
                            GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (Saved_CMD >> (i + 4) & 0x01));
                        }
                        // En
                        GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
                        LCD_Send_CMD++;
                        break;
                    case SendCmd2:
                        GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
                        LCD_Send_CMD++;

                        break;
                    case SendCmd3:
                        // Send lower nibble
                        for (uint8_t i = 0; i < 4; i++)
                        {
                            GPIO_WritePin(config->Data_Port[i + 4], config->Data_Pin[i + 4], (Saved_CMD >> i & 0x01));
                        }
                        // En
                        GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_SET);
                        LCD_Send_CMD++;
                    
                        break;
                    case SendCmd4:
                        GPIO_WritePin(config->E_Port, config->E_Pin, GPIO_PIN_RESET);
                        LCD_Send_CMD = SendCmd1;
                        if(is_writing_Custom)
                        {
                            LCD_State = LCD_SET_CUSTOM;
                        }
                        else{
                        LCD_State = NO_ACTION;
                        }
                        break;
                    default:
                        break;
                }
         break;
        case LCD_SET_CUSTOM:
                if(!is_writing_Custom){
                is_writing_Custom = 1;
                LCD_AsynchSendCommand(LCD_SET_CGRAM_ADDR | (CustomChar_Location << 3));      
                }
                
                else if(CustomChar_Index <=7)
                {
                LCD_AsynchWriteChar(Global_LCD,&CustomChar_Buffer[CustomChar_Index++]);
                }
                else{
                    is_writing_Custom = 0;
                    LCD_State = NO_ACTION;
                }
                break;
        case DoNothing:
                LCD_State = NO_ACTION;
                break;
        default:
        break;
    }
}
LCD_Status_t LCD_AsynchInit(LCD_Config *config)
{
    if (config == NULL) return LCD_ERROR;

    // 1. Initialize Pins (Keep your existing loop)
    for (uint8_t i = 0; i <= LCD_EN; i++) {
        GPIO_PinInit(LCD_CTRLPINS[i].Port, &LCD_CTRLPINS[i]);
    }

    if (config->mode == LCD_MODE_4BIT)
    {
        // Initialize data pins (Keep your existing loop)
        for (uint8_t i = 0; i < 4; i++) {
            GPIO_PinConfig_t pinConfig;
            pinConfig.Port = config->Data_Port[i + 4];
            pinConfig.Pin = config->Data_Pin[i + 4];
            pinConfig.Mode = GPIO_MODE_OUTPUT;
            pinConfig.OutputType = GPIO_OUTPUT_PUSHPULL;
            pinConfig.Speed = GPIO_SPEED_HIGH;
            pinConfig.PuPd = GPIO_PUPD_NONE;
            GPIO_PinInit(pinConfig.Port, &pinConfig);
        }

        // 2. Setup State
        Global_LCD = config;       // Save config globally
        LCD_State = LCD_INIT;
        LCD_Init_Seq = Function_Set_Init1;

        // 3. Register Runnable (CORRECTED FOR NEW SCHEDULER)
        LCD_Runnable_Obj.arg = (void *)Global_LCD; // Pass config as argument
        Sched_Register_Runnable(&LCD_Runnable_Obj);
      
        return LCD_IN_PROGRESS;
    }
    return LCD_ERROR;
}

LCD_Status_t LCD_AsynchWriteString(LCD_Config *config, const uint8_t *string)
{

    if (LCD_Init_Seq == INITDONE && LCD_State == NO_ACTION)
    {
        if (config == NULL || string == NULL)
        {
            return LCD_ERROR;
        }
        Saved_String = string;
        LCD_State = LCD_WRITE_STR;
        return LCD_IN_PROGRESS;
    }
    return LCD_BUSY;
}

LCD_Status_t LCD_AsynchWriteChar(LCD_Config *config, const uint8_t *chr)
{
    if (LCD_Init_Seq == INITDONE && (LCD_State == NO_ACTION || LCD_State == LCD_SET_CUSTOM))
    {
        if (config == NULL || chr == NULL)
        {
            return LCD_ERROR;
        }
        Saved_Char = chr;
        LCD_State = LCD_WRITE_CHAR;
        return LCD_IN_PROGRESS;

    }    return LCD_BUSY;

}

// LCD_Status_t LCD_AsynchSetCursorPosition(LCD_Config *config, uint8_t row, uint8_t column);

LCD_Status_t LCD_AsynchSendCommand(uint8_t command){

    if (LCD_Init_Seq == INITDONE && (LCD_State == NO_ACTION || LCD_State == LCD_SET_CUSTOM)){
            Saved_CMD = command;
            LCD_State = LCD_SENDCMD;
            return LCD_IN_PROGRESS;
            }
    return LCD_BUSY;
}

LCD_Status_t LCD_AsynchClear(LCD_Config *config){
    if(config == NULL)

    {
        return LCD_ERROR;
    }
   LCD_Status_t status =  LCD_AsynchSendCommand(LCD_CLEAR_DISPLAY);
    return status;
}

LCD_Status_t LCD_AsynchSetCursorPosition(LCD_Config *config, uint8_t row, uint8_t column){
       if (config == NULL)
    {
        return LCD_ERROR;
    }
    uint8_t address;

    switch (row)
    {
    case 0:
        address = LCD_LINE1 + column;
        break;
    case 1:
        address = LCD_LINE2 + column;
        break;
    case 2:
        address = LCD_LINE3 + column;
        break;
    case 3:
        address = LCD_LINE4 + column;
        break;
    default:
        return LCD_ERROR; // Invalid row
    }
   LCD_Status_t status =   LCD_AsynchSendCommand(LCD_SET_DDRAM_ADDR | address);
    return status;

}

LCD_Status_t LCD_AsynchSetCustomChar(LCD_Config *config, uint8_t location, uint8_t charmap[])
{
    if (config == NULL || charmap == NULL || location > 7)
    {
        return LCD_ERROR;
    }
    
    // Check if driver is idle
    if (LCD_Init_Seq == INITDONE && LCD_State == NO_ACTION)
    {
        // 1. Copy data to static buffer (Safe persistence)
        for(uint8_t i=0; i<8; i++) {
            CustomChar_Buffer[i] = charmap[i];
        }
        
        // 2. Setup internal variables
        CustomChar_Location = location;
        CustomChar_Index = 0; 
        
        // 3. Set State
        LCD_State = LCD_SET_CUSTOM;
        
        return LCD_IN_PROGRESS;
    }
    return LCD_BUSY;
}
LCD_Status_t LCD_AsynchDoNothing(){
    if (LCD_Init_Seq == INITDONE && LCD_State == NO_ACTION)
    {
        LCD_State = DoNothing;
    }
    return LCD_OK;
}

LCD_Status_t LCD_AsynchDislayCustomChar(LCD_Config *config, uint8_t location){
     if (config == NULL || location > 7)
    {
        return LCD_ERROR;
    }
     static uint8_t loc_temp ;
     loc_temp = location;
   LCD_Status_t status =  LCD_AsynchWriteChar(config, &loc_temp);
    return status;
}