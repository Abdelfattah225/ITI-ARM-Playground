/******************************************************************************
 * @file    7seg.c
 * @brief   Seven Segment Display Driver
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

#include "../../../include/interface/HAL/7seg.h"
#include "../../../include/configuration/HAL/7seg_cfg_t.h"  
#include "../../../lib/lib_utii.h"


/******************************************************************************
 * @brief   Segment patterns lookup table
 * @details Bit order: dp g f e d c b a (bit7 to bit0)
 *          Common Anode:   0 = ON, 1 = OFF (Active LOW)
 *          Common Cathode: 1 = ON, 0 = OFF (Active HIGH)
 ******************************************************************************/
#if (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_ANODE)
const uint8_t SEGMENT_PATTERNS[10] = {
    0x40,  // 0: a,b,c,d,e,f ON    -> 0100 0000
    0x79,  // 1: b,c ON            -> 0111 1001
    0x24,  // 2: a,b,d,e,g ON      -> 0010 0100
    0x30,  // 3: a,b,c,d,g ON      -> 0011 0000
    0x19,  // 4: b,c,f,g ON        -> 0001 1001
    0x12,  // 5: a,c,d,f,g ON      -> 0001 0010
    0x02,  // 6: a,c,d,e,f,g ON    -> 0000 0010
    0x78,  // 7: a,b,c ON          -> 0111 1000
    0x00,  // 8: all ON            -> 0000 0000
    0x10   // 9: a,b,c,d,f,g ON    -> 0001 0000
};
#elif (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_CATHODE)
const uint8_t SEGMENT_PATTERNS[10] = {
    0x3F,  // 0: a,b,c,d,e,f ON    -> 0011 1111
    0x06,  // 1: b,c ON            -> 0000 0110
    0x5B,  // 2: a,b,d,e,g ON      -> 0101 1011
    0x4F,  // 3: a,b,c,d,g ON      -> 0100 1111
    0x66,  // 4: b,c,f,g ON        -> 0110 0110
    0x6D,  // 5: a,c,d,f,g ON      -> 0110 1101
    0x7D,  // 6: a,c,d,e,f,g ON    -> 0111 1101
    0x07,  // 7: a,b,c ON          -> 0000 0111
    0x7F,  // 8: all ON            -> 0111 1111
    0x6F   // 9: a,b,c,d,f,g ON    -> 0110 1111
};
#endif


/******************************************************************************
 * @brief   Initialize the seven segment display
 * @param   cfg: Pointer to configuration structure containing port and pin info
 * @return  Seven_SEG_Status_t
 *          - SEVEN_SEGMENT_OK: Initialization successful
 *          - SEVEN_SEGMENT_ERROR: NULL pointer passed
 * @note    Configures all 8 pins (7 segments + DP) as push-pull outputs
 *          Sets initial state to OFF (all segments cleared)
 ******************************************************************************/
Seven_SEG_Status_t Seven_SEG_Init(Seven_SEG_cfg_t * cfg){

     if (cfg == NULL) {
        return SEVEN_SEGMENT_ERROR; 
    }
    GPIO_PinConfig_t SevenSeg_pin = {
        .AlternateFunc = GPIO_AF0_SYSTEM,
        .Mode = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OUTPUT_PUSHPULL,
        .PuPd = GPIO_PUPD_NONE,
        .Speed = GPIO_SPEED_HIGH
    };
    for(uint8_t i=0 ; i<8 ; i++){
   
        
        SevenSeg_pin.Port = cfg->segmentPort[i];
        SevenSeg_pin.Pin = cfg->segmentPins[i];
        GPIO_PinInit(cfg->segmentPort[i],&SevenSeg_pin);
#if (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_ANODE)
        /* Common Anode: HIGH = OFF */
        GPIO_SetPin(cfg->segmentPort[i],cfg->segmentPins[i]);
#elif (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_CATHODE)
        /* Common Cathode: LOW = OFF */
        GPIO_ResetPin(cfg->segmentPort[i],cfg->segmentPins[i]);
#endif
    }       

    return SEVEN_SEGMENT_OK;

}

/******************************************************************************
 * @brief   Display a single digit (0-9) on the seven segment display
 * @param   cfg: Pointer to configuration structure
 * @param   digit: Digit to display (0-9)
 * @return  Seven_SEG_Status_t
 *          - SEVEN_SEGMENT_OK: Digit displayed successfully
 *          - SEVEN_SEGMENT_ERROR: NULL pointer or invalid digit (>9)
 * @note    Does not affect the decimal point (DP) state
 ******************************************************************************/
Seven_SEG_Status_t Seven_SEG_DisplayDigit(Seven_SEG_cfg_t * cfg, uint8_t digit){
   
    if (cfg == NULL) {
        return SEVEN_SEGMENT_ERROR; 
    } 
    if(digit > 9 )
    {
        return SEVEN_SEGMENT_ERROR;
    }
   
    /* Get pattern from lookup table */
    uint8_t pattern = SEGMENT_PATTERNS[digit];  
    
    /* Set each segment according to pattern (segments A-G only) */
    for(uint8_t i=0 ; i<7 ; i++){
       
        if((pattern>>i) & 0x01 ){
            GPIO_SetPin(cfg->segmentPort[i],cfg->segmentPins[i]);
        }
        else{
            GPIO_ResetPin(cfg->segmentPort[i],cfg->segmentPins[i]);
        }
    }

    return SEVEN_SEGMENT_OK;
}

/******************************************************************************
 * @brief   Clear the seven segment display (turn off all segments)
 * @param   cfg: Pointer to configuration structure
 * @return  Seven_SEG_Status_t
 *          - SEVEN_SEGMENT_OK: Display cleared successfully
 *          - SEVEN_SEGMENT_ERROR: NULL pointer passed
 * @note    Clears all 8 segments including decimal point (DP)
 ******************************************************************************/
Seven_SEG_Status_t Seven_SEG_Clear(Seven_SEG_cfg_t * cfg){
      if (cfg == NULL) {
        return SEVEN_SEGMENT_ERROR; 
        } 
       for(uint8_t i=0 ; i<8 ; i++){
#if (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_ANODE)
            /* Common Anode: HIGH = OFF */
            GPIO_SetPin(cfg->segmentPort[i],cfg->segmentPins[i]);
#elif (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_CATHODE)
            /* Common Cathode: LOW = OFF */
            GPIO_ResetPin(cfg->segmentPort[i],cfg->segmentPins[i]);
#endif
         }
         return SEVEN_SEGMENT_OK;
}

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
Seven_SEG_Status_t Seven_SEG_DisplayDot(Seven_SEG_cfg_t * cfg, uint8_t state){
    if (cfg == NULL) {
        return SEVEN_SEGMENT_ERROR; 
    } 
#if (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_ANODE)
    /* Common Anode: LOW = ON, HIGH = OFF */
    if(state == SEVEN_SEG_DP_ON){
        GPIO_ResetPin(cfg->segmentPort[7],cfg->segmentPins[7]);
    }
    if(state == SEVEN_SEG_DP_OFF)
    {
        GPIO_SetPin(cfg->segmentPort[7],cfg->segmentPins[7]);
    }
#elif (SEVEN_SEG_TYPE == SEVEN_SEG_COMMON_CATHODE)
    /* Common Cathode: HIGH = ON, LOW = OFF */
    if(state == SEVEN_SEG_DP_ON){
        GPIO_SetPin(cfg->segmentPort[7],cfg->segmentPins[7]);
    }
    if(state == SEVEN_SEG_DP_OFF)
    {
        GPIO_ResetPin(cfg->segmentPort[7],cfg->segmentPins[7]);
    }
#endif
    return SEVEN_SEGMENT_OK;
}