#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>

typedef void (*cbf_t)(void *);

#define MAX_Sched 5

typedef struct
{
    const cbf_t fnc;
    uint32_t periodicity_ms;
    uint32_t first_delay_ms;
    void *arg;
    uint32_t priority;      
} Runnable_t;

// Structure to hold runtime status
typedef struct
{
    Runnable_t *runnable;
    uint32_t remaining_time_ms;
} Sched_Task_t;

typedef enum
{
    SCED_OK,
    SCED_ERROR,
} enu_err_t;

enu_err_t Sched_Init(uint32_t tick_time_ms);
enu_err_t Sched_Register_Runnable(Runnable_t *Runnable);
enu_err_t Sched_Start(void);

#endif