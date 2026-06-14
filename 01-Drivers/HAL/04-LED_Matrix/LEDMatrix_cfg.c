#include "../../../include/interface/HAL/LEDMatrix.h"

/* ============================================
 * LED Matrix Pin Configuration
 * ============================================
 * Modify this array to match your hardware connections
 */
const LMAX_Cfg_t LMAX_Cfg[LED_MATRIX_MAX] = 
{
    /* Matrix 0 (LMAX_1) */
    [LMAX_1] = {
        .type = LMAX_COMMON_CATHODE,
        
        /* Row pins - accent rows (accent select in common cathode) */
        .Row_ports = {
            GPIOA, GPIOA, GPIOA, GPIOA,
            GPIOA, GPIOA, GPIOA, GPIOA
        },
        .Row_pins = {0, 1, 2, 3, 4, 5, 6, 7},
        
        /* Column pins - data lines */
        .Column_ports = {
            GPIOB, GPIOB, GPIOB, GPIOB,
            GPIOB, GPIOB, GPIOB, GPIOB
        },
        .Column_pins = {0, 1, 2, 3, 4, 5, 6, 7}
    }
    
    /* Add more matrices here if LED_MATRIX_MAX > 1 */
    /*
    [LMAX_2] = {
        .type = LMAX_COMMON_CATHODE,
        .Row_ports = {...},
        .Row_pins = {...},
        .Column_ports = {...},
        .Column_pins = {...}
    }
    */
};