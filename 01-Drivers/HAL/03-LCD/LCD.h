#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "../../configuration/HAL/LCD_Cfg.h"

typedef enum
{
    LCD_OK,
    LCD_ERROR,
    LCD_BUSY,
    LCD_IN_PROGRESS
} LCD_Status_t;

typedef enum
{
    LCD_MODE_4BIT,
    LCD_MODE_8BIT
} LCD_Mode_t;

typedef struct
{
    void *RS_Port;
    uint8_t RS_Pin;
    void *E_Port;
    uint8_t E_Pin;

    /* Data Pins */
    void *Data_Port[8];  // Array of ports for data pins D0-D7
    uint8_t Data_Pin[8]; // Array of pin numbers for data pins D0-D7

    LCD_Mode_t mode; // 4-bit or 8-bit mode

} LCD_Config;

typedef enum
{

    LCD_RS,
    LCD_EN
} LCD_CTRL_t;

typedef enum
{
    NO_ACTION,
    LCD_INIT,
    LCD_WRITE_STR,
    LCD_WRITE_CHAR,
    LCD_SENDCMD,
    LCD_SET_CUSTOM,
    DoNothing
} LCD_State_t;

typedef enum
{
   
    /* Function set */
    Function_Set_Init1,
    Function_Set_Init2,
    Function_Set_Init3,
    Function_Set_Init4,

    Function_Set_4_bit_mode1,
    Function_Set_4_bit_mode2,
    Function_Set_4_bit_mode3,
    Function_Set_4_bit_mode4,

    Function_Set_4BIT_2LINE1,
    Function_Set_4BIT_2LINE2,
    Function_Set_4BIT_2LINE3,
    Function_Set_4BIT_2LINE4,
    /* Display on*/

    DISPLAY_ON_CURSOR_OFF1,
    DISPLAY_ON_CURSOR_OFF2,
    DISPLAY_ON_CURSOR_OFF3,
    DISPLAY_ON_CURSOR_OFF4,

    CLEAR_DISPLAY1,
    CLEAR_DISPLAY2,
    CLEAR_DISPLAY3,
    CLEAR_DISPLAY4,


    ENTRY_MODE_INC_NOSHIFT1,
    ENTRY_MODE_INC_NOSHIFT2,
    ENTRY_MODE_INC_NOSHIFT3,
    ENTRY_MODE_INC_NOSHIFT4,


    INITDONE
} LCD_Init_Seq_t;

typedef enum{
    Str_SendData1,
    Str_SendData2,
    Str_SendData3,
    Str_SendData4
}LCD_Write_Str_Seq_t;



typedef enum{
    Char_SendData1,
    Char_SendData2,
    Char_SendData3,
    Char_SendData4
}LCD_Write_Char_Seq_t;


typedef enum{
    SendCmd1,
    SendCmd2,
    SendCmd3,
    SendCmd4
}LCD_Write_cmd_t;



// Synchronous functions
LCD_Status_t LCD_Init(LCD_Config *config);
LCD_Status_t LCD_WriteChar(LCD_Config *config, uint8_t character);
LCD_Status_t LCD_WriteString(LCD_Config *config, const uint8_t *string);
LCD_Status_t LCD_Clear(LCD_Config *config);
LCD_Status_t LCD_SetCursorPosition(LCD_Config *config, uint8_t row, uint8_t column);
LCD_Status_t LCD_SetCustomChar(LCD_Config *config, uint8_t location, uint8_t charmap[]);
LCD_Status_t LCD_DislayCustomChar(LCD_Config *config, uint8_t location);

// Asynchronous functions
void LCD_AsynchRunnable(LCD_Config *config);
LCD_Status_t LCD_AsynchInit(LCD_Config *config);
LCD_Status_t LCD_AsynchWriteString(LCD_Config *config, const uint8_t *string);
LCD_Status_t LCD_AsynchWriteChar(LCD_Config *config, const uint8_t *chr);
LCD_Status_t LCD_AsynchSendCommand(uint8_t command);
LCD_Status_t LCD_AsynchClear(LCD_Config *config);
LCD_Status_t LCD_AsynchSetCursorPosition(LCD_Config *config, uint8_t row, uint8_t column);
LCD_Status_t LCD_AsynchSetCustomChar(LCD_Config *config, uint8_t location, uint8_t charmap[]);
LCD_Status_t LCD_AsynchDoNothing(void);
LCD_Status_t LCD_AsynchDislayCustomChar(LCD_Config *config, uint8_t location);

#endif