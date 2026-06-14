/******************************************************************************
 * @file    7seg_cfg.c
 * @brief   Seven Segment Display Hardware Configuration
 * @details Contains the hardware-specific configuration for the 7-segment display
 *          Modify this file to match your hardware connections
 *          
 * @note    Segment to Pin Mapping:
 *          - segmentPort[0], segmentPins[0] = Segment A
 *          - segmentPort[1], segmentPins[1] = Segment B
 *          - segmentPort[2], segmentPins[2] = Segment C
 *          - segmentPort[3], segmentPins[3] = Segment D
 *          - segmentPort[4], segmentPins[4] = Segment E
 *          - segmentPort[5], segmentPins[5] = Segment F
 *          - segmentPort[6], segmentPins[6] = Segment G
 *          - segmentPort[7], segmentPins[7] = Decimal Point (DP)
 *
 *              AAA
 *             F   B
 *              GGG
 *             E   C
 *              DDD  .DP
 *
 ******************************************************************************/

#include "../../../include/interface/HAL/7seg.h"

/******************************************************************************
 * @brief   Seven Segment Display Configuration Instance
 * @details Hardware configuration for the 7-segment display
 *          All segments connected to GPIOA (PA0-PA7)
 *          
 *          Pin Assignment:
 *          - PA0 = Segment A
 *          - PA1 = Segment B
 *          - PA2 = Segment C
 *          - PA3 = Segment D
 *          - PA4 = Segment E
 *          - PA5 = Segment F
 *          - PA6 = Segment G
 *          - PA7 = Decimal Point (DP)
 ******************************************************************************/
Seven_SEG_cfg_t my7Seg_cfg = {
    .segmentPort = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA}, /* Ports for A, B, C, D, E, F, G, DP */
    .segmentPins = {GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, GPIO_PIN6, GPIO_PIN7} /* Pins for A, B, C, D, E, F, G, DP */
};