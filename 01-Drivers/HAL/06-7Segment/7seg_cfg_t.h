/******************************************************************************
 * @file    7seg_cfg_t.h
 * @brief   Seven Segment Display Configuration Header
 * @details Contains compile-time configuration options for the 7-segment driver
 *          
 * @note    Modify SEVEN_SEG_TYPE to match your hardware:
 *          - SEVEN_SEG_COMMON_ANODE: Segments are active LOW
 *          - SEVEN_SEG_COMMON_CATHODE: Segments are active HIGH
 *
 ******************************************************************************/

#ifndef Seven_SEG_CFG_T_H
#define Seven_SEG_CFG_T_H

/******************************************************************************
 * @brief   Decimal Point State Definitions
 ******************************************************************************/
#define SEVEN_SEG_DP_ON   1    /* Turn decimal point ON */
#define SEVEN_SEG_DP_OFF  0    /* Turn decimal point OFF */

/******************************************************************************
 * @brief   Display Type Definitions
 * @details Common Anode: All anodes connected together to VCC
 *          - Segments turn ON when pin is LOW (Active LOW)
 *          
 *          Common Cathode: All cathodes connected together to GND
 *          - Segments turn ON when pin is HIGH (Active HIGH)
 ******************************************************************************/
#define SEVEN_SEG_COMMON_ANODE    0
#define SEVEN_SEG_COMMON_CATHODE  1

/******************************************************************************
 * @brief   Display Type Selection
 * @details Select your seven segment display type here
 *          Options:
 *          - SEVEN_SEG_COMMON_ANODE
 *          - SEVEN_SEG_COMMON_CATHODE
 ******************************************************************************/
#define SEVEN_SEG_TYPE    SEVEN_SEG_COMMON_ANODE


#endif