
#include <stdio.h>
unsigned int __attribute__((section(".led_cfg_section"))) LED_CFG =100;

extern unsigned int _sled_cfg;
extern unsigned int _eled_cfg;

int main(void)
{
    LED_CFG = 100;


   
    printf("%d",_sled_cfg);
    printf("0x%x",&_sled_cfg);

    printf("%d",LED_CFG);
    printf("0x%x",&LED_CFG);
    while (1)
    {
        
    }
    
    return 0;
}