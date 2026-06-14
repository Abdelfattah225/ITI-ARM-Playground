#ifndef SWITCH_H
#define SWITCH_H

#include "../../configuration/HAL/SWITCH_cfg.h"
#include "../../interface/MCAL/GPIO_Interface.h"
#include <stdint.h>

/* Removed global debounce define, as it is now inside the struct */

typedef enum
{
    Internal_PU,
    External_PU,
    Internal_PD,
    External_PD,
    floating
} Switch_Resistor_Config_t;

typedef enum {
    LOW,
    HIGH
} Button_State_t;

typedef enum {
    NOTPRESSED,
    PRESSED
} Return_Button_State_t; /* Fixed typo: Resturn -> Return */

/* Debounce state tracking */
typedef struct
{
    
    uint32_t last_read_time;
    uint8_t stable_count;
    uint8_t last_raw_state; 
    Return_Button_State_t current_logic_state; 
    Return_Button_State_t previous_logic_state;
} Switch_Debounce_t;

typedef struct 
{
    void * Port;
    GPIO_Pin_t Pin;
    Switch_Resistor_Config_t resistor_cfg;
    
    /* --- Debounce Configuration (Settings) --- */
    uint32_t debounce_duration_ms; 

    /* --- Debounce State (Runtime) --- */
    Switch_Debounce_t DebounceState;  

    uint8_t CurrentStatus;
    uint8_t LastStatus;
} SWITCH_cfg_t;

typedef enum
{
    SWITCH_Ok,
    SWITCH_NOT_NULL_PTR,
    SWITCH_INVALID
} switch_status_t; /* Fixed typo: swtich -> switch */

switch_status_t Switch_Init(void);

/* Updated: Only takes 1 argument now */
switch_status_t Switch_ReadState_Blocking(SWITCH_cfg_t *SwitchCx);



// Asynch Function


/* Function prototypes */
switch_status_t Switch_AsynchInit(void);
void Switch_AsynchRunnable(void);                           /* Call from scheduler */
uint8_t Switch_AsynchGetState(Switch_names_t switch_id);
uint8_t Switch_AsynchGetPressedEvent(Switch_names_t switch_id);
uint8_t Switch_AsynchGetReleasedEvent(Switch_names_t switch_id);

#endif