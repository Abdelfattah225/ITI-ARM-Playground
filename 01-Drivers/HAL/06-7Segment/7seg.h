/******************************************************************************
 * @file    7seg.h
 * @brief   Seven Segment Display Driver Interface
 * @details This driver supports both Common Anode and Common Cathode displays.
 *          The display type can be configured in 7seg_cfg_t.h
 *          
 * @note    Segment mapping: A(0), B(1), C(2), D(3), E(4), F(5), G(6), DP(7)
 * 
 *              AAA
 *             F   B
 *              GGG
 *             E   C
 *              DDD  .DP
 *
 ******************************************************************************/

#ifndef Seven_SEG_H
#define Seven_SEG_H

#include <stdint.h>
#include "../MCAL/GPIO_Interface.h"


/******************************************************************************
 * @brief   Seven Segment Configuration Structure
 * @details Holds GPIO port and pin configuration for all 8 segments
 *          Index mapping:
 *          - [0] = Segment A
 *          - [1] = Segment B
 *          - [2] = Segment C
 *          - [3] = Segment D
 *          - [4] = Segment E
 *          - [5] = Segment F
 *          - [6] = Segment G
 *          - [7] = Decimal Point (DP)
 ******************************************************************************/
typedef struct 
{
    void * segmentPort[8];     /* GPIO port for segments (A-G, DP) */
    GPIO_Pin_t segmentPins[8]; /* GPIO pins for segments (A-G, DP) */    
}Seven_SEG_cfg_t;

/******************************************************************************
 * @brief   Seven Segment Status Enumeration
 * @details Return status for all driver functions
 ******************************************************************************/
typedef enum 
{
    SEVEN_SEGMENT_OK = 0,      /* Operation completed successfully */
    SEVEN_SEGMENT_ERROR        /* Operation failed (NULL pointer or invalid input) */
}Seven_SEG_Status_t;


/******************************************************************************
 * @brief   Initialize the seven segment display
 * @param   cfg: Pointer to configuration structure containing port and pin info
 * @return  Seven_SEG_Status_t
 *          - SEVEN_SEGMENT_OK: Initialization successful
 *          - SEVEN_SEGMENT_ERROR: NULL pointer passed
 * @note    Configures all 8 pins (7 segments + DP) as push-pull outputs
 *          Sets initial state to OFF (all segments cleared)
 ******************************************************************************/
Seven_SEG_Status_t Seven_SEG_Init(Seven_SEG_cfg_t * cfg);

/******************************************************************************
 * @brief   Display a single digit (0-9) on the seven segment display
 * @param   cfg: Pointer to configuration structure
 * @param   digit: Digit to display (0-9)
 * @return  Seven_SEG_Status_t
 *          - SEVEN_SEGMENT_OK: Digit displayed successfully
 *          - SEVEN_SEGMENT_ERROR: NULL pointer or invalid digit (>9)
 * @note    Does not affect the decimal point (DP) state
 ******************************************************************************/
Seven_SEG_Status_t Seven_SEG_DisplayDigit(Seven_SEG_cfg_t * cfg, uint8_t digit);

/******************************************************************************
 * @brief   Clear the seven segment display (turn off all segments)
 * @param   cfg: Pointer to configuration structure
 * @return  Seven_SEG_Status_t
 *          - SEVEN_SEGMENT_OK: Display cleared successfully
 *          - SEVEN_SEGMENT_ERROR: NULL pointer passed
 * @note    Clears all 8 segments including decimal point (DP)
 ******************************************************************************/
Seven_SEG_Status_t Seven_SEG_Clear(Seven_SEG_cfg_t * cfg);

/******************************************************************************
 * @brief   Control the decimal point (DP) segment
 * @param   cfg: Pointer to configuration structure
 * @param   state: Desired state for decimal point
 *          - SEVEN_SEG_DP_ON: Turn on decimal point
 *          - SEVEN_SEG_DP_OFF: Turn off decimal point
 * @return  Seven_SEG_Status_t
 *          - SEVEN_SEGMENT_OK: DP state changed successfully
 *          - SEVEN_SEGMENT_ERROR: NULL pointer passed
 ******************************************************************************/
Seven_SEG_Status_t Seven_SEG_DisplayDot(Seven_SEG_cfg_t * cfg, uint8_t state);

#endif