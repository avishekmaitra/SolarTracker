#include "msp.h"
#include"delay.h"
#include"LCD.h"
#include<stdint.h>

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watch dog timer
    set_DCO(FREQ_12_MHZ);

    init_LCD();
    while(1) {
        command_LCD(CLR_DISP);          //clear_LCD();
        delay_us(200000, FREQ_12_MHZ);  //delay
        write_char_LCD('A');
        delay_us(200000, FREQ_12_MHZ);  //delay
    }
    //write_char_LCD('C');
    //write_string_LCD("sweet");
    //command_LCD(0x0F);
    //delay_us(10000, FREQ_12_MHZ);
}
