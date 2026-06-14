#include "sched.h"
#include "../include/interface/MCAL/SYSTICK_Interface.h"

#define NULL ((void*)0)

static Sched_Task_t SystemTasks[MAX_Sched];
static volatile uint32_t PendingTicks = 0;
static uint32_t TickTime = 1;

// ISR Context: Keep it short!
static void Sched_SysTick_Callback(void)
{
    PendingTicks++;
}

// Main Loop Context
static void Sched_Scheduler(void)
{
    uint32_t i;
    for (i = 0; i < MAX_Sched; i++)
    {
        if (SystemTasks[i].runnable != NULL)
        {
            if (SystemTasks[i].remaining_time_ms == 0)
            {
                // Execute Task
                if (SystemTasks[i].runnable->fnc != NULL)
                {
                    SystemTasks[i].runnable->fnc(SystemTasks[i].runnable->arg);
                }
                // Reset timer to periodicity
                SystemTasks[i].remaining_time_ms = SystemTasks[i].runnable->periodicity_ms;
            }
            else
            {
                SystemTasks[i].remaining_time_ms -= TickTime;
            }
        }
    }
}

enu_err_t Sched_Init(uint32_t tick_time_ms)
{
    TickTime = tick_time_ms;
    
    // Initialize array
    for(int i=0; i<MAX_Sched; i++) {
        SystemTasks[i].runnable = NULL;
    }

    SYSTICK_Init();
    SYSTICK_ConfigDelay_ms(TickTime, SYSTICK_AHB_FREQ_MHZ, SYSTICK_CLOCK_SOURCE);
    SYSTICK_EnableInterrupt();
    SYSTICK_SetCallback(Sched_SysTick_Callback);
    
    return SCED_OK;
}

enu_err_t Sched_Register_Runnable(Runnable_t *Runnable)
{
    if (Runnable == NULL || Runnable->priority >= MAX_Sched)
    {
        return SCED_ERROR;
    }
    
    // Check if priority slot is free
    if (SystemTasks[Runnable->priority].runnable == NULL)
    {
        SystemTasks[Runnable->priority].runnable = Runnable;
        SystemTasks[Runnable->priority].remaining_time_ms = Runnable->first_delay_ms;
        return SCED_OK;
    }
    return SCED_ERROR;
}

enu_err_t Sched_Start(void)
{
    SYSTICK_Enable();
    
    while (1)
    {
        if (PendingTicks > 0)
        {
            PendingTicks--;
            Sched_Scheduler();
        }
    }
    return SCED_OK;
}