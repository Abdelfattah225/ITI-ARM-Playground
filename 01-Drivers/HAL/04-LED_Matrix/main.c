#include "./os/sched.h"
#include "../include/interface/MCAL/rcc_init.h"
#include "../include/interface/HAL/LEDMatrix.h"
#include "../lib/lib_utii.h"
#include "sched.h"

/* LED Matrix refresh task - runs every 2ms */
static Runnable_t LedMatrixTask = {
    .fnc = LMAX_RefreshTask,
    .periodicity_ms =.1,
    .first_delay_ms = 0,
    .arg = NULL,
    .priority = 0
};

/* Test patterns */
static const uint8_t Smile[LMAX_ROWS] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100
};

static const uint8_t Heart[LMAX_ROWS] = {
    0b00000000,
    0b01100110,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00000000
};
/* Smiley face pattern */
static const uint8_t Smiley[LMAX_ROWS] = {
    0b00111100,  /* Row 0:   ○○●●●●○○ */
    0b01000010,  /* Row 1:   ○●○○○○●○ */
    0b10100101,  /* Row 2:   ●○●○○●○● */
    0b10000001,  /* Row 3:   ●○○○○○○● */
    0b10100101,  /* Row 4:   ●○●○○●○● */
    0b10011001,  /* Row 5:   ●○○●●○○● */
    0b01000010,  /* Row 6:   ○●○○○○●○ */
    0b00111100,  /* Row 7:   ○○●●●●○○ */
};
int main(void)
{
    RCC_Init();
    /* Initialize scheduler with 1ms tick */
    Sched_Init(1);
    
    /* Initialize LED Matrix */
    LMAX_Init(LMAX_1);
    
    /* Load a pattern */
    LMAX_SetBuffer(LMAX_1, Smile);
    
    /* Enable the display */
    LMAX_Enable(LMAX_1);
    
    /* Register refresh task with scheduler */
    Sched_Register_Runnable(&LedMatrixTask);
    
    /* Start the scheduler (never returns) */
    Sched_Start();
    
    return 0;
}