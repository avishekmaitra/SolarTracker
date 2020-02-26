#include "msp.h"
#include"delay.h"
#include"LCD.h"
#include<stdint.h>

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;    // stop watch dog timer
    set_DCO(FREQ_48_MHZ);
    init_LCD();
    clear_LCD();
    while(1)
    {
        clear_LCD();
        delay_ms(1000, FREQ_48_MHZ);  //delay
        write_LCD_L1("This now kinda works");          // issue where T in line 1 and 2 are deleted after all 4 lines write
        write_LCD_L2("This now kindb works");
        write_LCD_L3("This now kindc works");
        write_LCD_L4("This now kindd works");

        delay_ms(2000, FREQ_48_MHZ);
    }
}
