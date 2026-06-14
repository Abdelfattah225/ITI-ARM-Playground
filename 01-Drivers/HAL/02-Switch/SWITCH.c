#include "../../../include/interface/HAL/SWITCH.h"
#include "../../../include/interface/MCAL/GPIO_Interface.h"
#include "../../../include/interface/MCAL/SYSTICK_Interface.h"
#include <string.h>

/* Private flags for edge detection */
static uint8_t pressed_event_flags[Switch_LEN];
static uint8_t released_event_flags[Switch_LEN];

/* Temporary storage for GPIO config during initialization */
GPIO_PinConfig_t switch_pin_GPIO[Switch_LEN];

/* Extern declaration must match definition (no const) */
extern SWITCH_cfg_t SWITCH_cfg[Switch_LEN];

switch_status_t Switch_Init(void)
{
    for (uint8_t i = 0; i < Switch_LEN; i++)
    {
        switch_pin_GPIO[i].Port = SWITCH_cfg[i].Port;
        switch_pin_GPIO[i].Pin = SWITCH_cfg[i].Pin;
        switch_pin_GPIO[i].OutputType = GPIO_OUTPUT_PUSHPULL;
        switch_pin_GPIO[i].AlternateFunc = GPIO_AF0_SYSTEM;
        switch_pin_GPIO[i].Mode = GPIO_MODE_INPUT;
        switch_pin_GPIO[i].Speed = GPIO_SPEED_HIGH;

        if (SWITCH_cfg[i].resistor_cfg == Internal_PD)
        {
            switch_pin_GPIO[i].PuPd = GPIO_PUPD_PULLDOWN;
        }
        else if (SWITCH_cfg[i].resistor_cfg == Internal_PU)
        {
            switch_pin_GPIO[i].PuPd = GPIO_PUPD_PULLUP;
        }
        else
        {
            switch_pin_GPIO[i].PuPd = GPIO_PUPD_NONE;
        }

        GPIO_PinInit(switch_pin_GPIO[i].Port, &switch_pin_GPIO[i]);

        /* Initialize Runtime Debounce State */
        SWITCH_cfg[i].DebounceState.stable_count = 0;
        SWITCH_cfg[i].DebounceState.last_raw_state = 0;
        SWITCH_cfg[i].DebounceState.current_logic_state = NOTPRESSED;
    }
    return SWITCH_Ok;
}
extern void Delay_ms(uint32_t ms); // Replace with HAL_Delay, vTaskDelay, etc.

/* Simple blocking, debounced read */
switch_status_t Switch_ReadState_Blocking(SWITCH_cfg_t *SwitchCx)
{
    if (SwitchCx == NULL)
    {
        return SWITCH_NOT_NULL_PTR;
    }

    uint32_t first_read, second_read;

    /* Wait until we get two identical readings spaced by debounce_duration_ms */
    do
    {
        first_read = GPIO_ReadPin(SwitchCx->Port, SwitchCx->Pin);
        SYSTICK_Delay_ms(SwitchCx->debounce_duration_ms);
        second_read = GPIO_ReadPin(SwitchCx->Port, SwitchCx->Pin);
    } while (first_read != second_read);

    /* Now 'second_read' is stable → convert to PRESSED / NOTPRESSED */
    Return_Button_State_t new_state = NOTPRESSED;

    if ((SwitchCx->resistor_cfg == Internal_PU) || (SwitchCx->resistor_cfg == External_PU))
    {
        /* Active Low */
        new_state = (second_read == LOW) ? PRESSED : NOTPRESSED;
    }
    else if ((SwitchCx->resistor_cfg == Internal_PD) || (SwitchCx->resistor_cfg == External_PD))
    {
        /* Active High */
        new_state = (second_read == HIGH) ? PRESSED : NOTPRESSED;
    }

    /* Store the result (optional but matches your design) */
    SwitchCx->DebounceState.current_logic_state = new_state;

    return SWITCH_Ok;
}

/* Function prototypes */
switch_status_t Switch_AsynchInit(void)
{

    for (uint8_t i = 0; i < Switch_LEN; i++)
    {
        /* Configure GPIO */
        switch_pin_GPIO[i].Port = SWITCH_cfg[i].Port;
        switch_pin_GPIO[i].Pin = SWITCH_cfg[i].Pin;
        switch_pin_GPIO[i].Mode = GPIO_MODE_INPUT;
        switch_pin_GPIO[i].OutputType = GPIO_OUTPUT_PUSHPULL;
        switch_pin_GPIO[i].Speed = GPIO_SPEED_HIGH;
        switch_pin_GPIO[i].AlternateFunc = GPIO_AF0_SYSTEM;

        /* Set pull-up/pull-down based on config */
        switch (SWITCH_cfg[i].resistor_cfg)
        {
        case Internal_PU:
            switch_pin_GPIO[i].PuPd = GPIO_PUPD_PULLUP;
            break;
        case Internal_PD:
            switch_pin_GPIO[i].PuPd = GPIO_PUPD_PULLDOWN;
            break;
        default:
            switch_pin_GPIO[i].PuPd = GPIO_PUPD_NONE;
            break;
        }

        GPIO_PinInit(switch_pin_GPIO[i].Port, &switch_pin_GPIO[i]);

        /* Initialize debounce state */
        SWITCH_cfg[i].DebounceState.stable_count = 0;
        SWITCH_cfg[i].DebounceState.last_raw_state = NOTPRESSED;
        SWITCH_cfg[i].DebounceState.current_logic_state = NOTPRESSED;
        SWITCH_cfg[i].DebounceState.previous_logic_state = NOTPRESSED;

        /* Clear event flags */
        pressed_event_flags[i] = 0;
        released_event_flags[i] = 0;
    }

    return SWITCH_Ok;
}

void Switch_AsynchRunnable(void)
{
    uint8_t raw_reading;
    uint8_t logical_state;

    for (uint8_t i = 0; i < Switch_LEN; i++)
    {
        /* 1. Read raw GPIO state */
        raw_reading = GPIO_ReadPin(SWITCH_cfg[i].Port, SWITCH_cfg[i].Pin);

        /* 2. Convert to logical state based on pull-up/pull-down */
        if (SWITCH_cfg[i].resistor_cfg == Internal_PU ||
            SWITCH_cfg[i].resistor_cfg == External_PU)
        {
            /* Active LOW: pressed when pin is LOW */
            logical_state = (raw_reading == 0) ? PRESSED : NOTPRESSED;
        }
        else
        {
            /* Active HIGH: pressed when pin is HIGH */
            logical_state = (raw_reading == 1) ? PRESSED : NOTPRESSED;
        }

        /* 3. Debounce logic */
        if (logical_state == SWITCH_cfg[i].DebounceState.last_raw_state)
        {
            /* State is same as last reading - increment counter */
            if (SWITCH_cfg[i].DebounceState.stable_count < SWITCH_cfg[i].debounce_duration_ms)
            {
                SWITCH_cfg[i].DebounceState.stable_count++;

                /* Check if we've JUST reached threshold (== not >=) */
                if (SWITCH_cfg[i].DebounceState.stable_count == SWITCH_cfg[i].debounce_duration_ms)
                {
                    /* State is now stable - check for edge ONCE */
                    
                    /* Detect edges (transitions) BEFORE updating current state */
                    if (SWITCH_cfg[i].DebounceState.current_logic_state == NOTPRESSED &&
                        logical_state == PRESSED)
                    {
                        /* Rising edge: button just pressed */
                        pressed_event_flags[i] = 1;
                    }
                    else if (SWITCH_cfg[i].DebounceState.current_logic_state == PRESSED &&
                             logical_state == NOTPRESSED)
                    {
                        /* Falling edge: button just released */
                        released_event_flags[i] = 1;
                    }

                    /* NOW update the current state */
                    SWITCH_cfg[i].DebounceState.previous_logic_state =
                        SWITCH_cfg[i].DebounceState.current_logic_state;
                    SWITCH_cfg[i].DebounceState.current_logic_state = logical_state;
                }
            }
            /* If already at threshold, do nothing - just maintain state */
        }
        else
        {
            /* State changed - reset counter */
            SWITCH_cfg[i].DebounceState.stable_count = 0;
        }

        /* 4. Save current reading for next iteration */
        SWITCH_cfg[i].DebounceState.last_raw_state = logical_state;
    }
}

uint8_t Switch_AsynchGetState(Switch_names_t switch_id)
{
    if (switch_id >= Switch_LEN)
    {
        return NOTPRESSED;
    }
    return SWITCH_cfg[switch_id].DebounceState.current_logic_state;
}

uint8_t Switch_AsynchGetPressedEvent(Switch_names_t switch_id)
{
    if (switch_id >= Switch_LEN)
    {
        return SWITCH_INVALID;
    }
    
    uint8_t event = pressed_event_flags[switch_id];
    pressed_event_flags[switch_id] = 0;
    return event;
}

uint8_t Switch_AsynchGetReleasedEvent(Switch_names_t switch_id)
{
    if (switch_id >= Switch_LEN)
    {
        return SWITCH_INVALID;
    }
    
    uint8_t event = released_event_flags[switch_id];
    released_event_flags[switch_id] = 0;
    return event;
}