#ifndef LED_CFG_H
#define LED_CFG_H


typedef enum{
    LED_WARNING,
    LED_DOOROPEN,
    LED_ERROR,
    // LED num => always have the number of element in enum
    LED_LEN
}LED_names_t;



typedef enum
{
    ACTIVE_LOW,
    ACTIVE_HIGH
}LED_active_state_t;

typedef enum
{
    PUSH_PULL,
    OPEN_DRAIN
}LED_IsPP_t;

typedef enum
{
   LED_LOW,
   LED_HIGH
}LED_OUTPUT_t;


#endif
