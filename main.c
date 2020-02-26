#include "msp.h"
#include"delay.h"
#include"LCD.h"
#include<stdint.h>

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;         // stop watch dog timer
    set_DCO(FREQ_48_MHZ);
    init_LCD();
    clear_LCD();
    while(1)
    {
        clear_LCD();
        delay_ms(1000, FREQ_48_MHZ);                    // delay 1s

        // STARTING SCREEN
        write_LCD_L1("Startup             ");            // move everything up, get rid of this
        write_LCD_L2("Enter Date:__/__/__ ");            // remember: only have 20 char spaces
        write_LCD_L3("Enter Time:__:__    ");            // military time
        write_LCD_L4("*Clear        #Enter");
        SetCursorLocation(0x4B);
        //getkeypress function
        //replace "_" with keypress
        //once key is pressed, will cursor automatically move right, or do we need to define next location?
        //needs to wait for either Clear or Enter press before changing screens

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // HOME SCREEN
        write_LCD_L1("A: Manual Entry     ");
        write_LCD_L2("B: Algorithm Based  ");
        write_LCD_L3("C: Demo Mode        ");
        write_LCD_L4("*Back               ");

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // A_1: MANUAL ENTRY INPUT
        write_LCD_L1("                    ");
        write_LCD_L2("Enter Angle: ___deg ");
        write_LCD_L3("                    ");
        write_LCD_L4("*Back  !Home  #Enter");           // Back deletes input value, Home goes to Home, Enter verifies angle desired

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // A_2: ANGLE UPDATE
        write_LCD_L1("Present Angle:___deg");
        write_LCD_L2("                    ");
        write_LCD_L3("Done.               ");           // only displayed when reach angle
        write_LCD_L4("*Back  !Home        ");

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // B_1: ALGORITHM BASED
        write_LCD_L1("                    ");
        write_LCD_L2("                    ");
        write_LCD_L3("                    ");
        write_LCD_L4("*Back  !Home  #Enter");

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // B_2: ANGLE UPDATE
        write_LCD_L1("Present Time:__:__  ");
        write_LCD_L2("Present Angle:___deg");
        write_LCD_L3("Done.               ");
        write_LCD_L4("*Back  !Home        ");

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // C_1: DEMO
        write_LCD_L1("Choose Demo Speed:  ");
        write_LCD_L2("A, B, C             ");
        write_LCD_L3("                    ");
        write_LCD_L4("       !Home  #Enter");

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // C_2: ANGLE UPDATE
        write_LCD_L1("Percent Complete:__%");
        write_LCD_L2("Present Angle:___deg");
        write_LCD_L3("Done.               ");
        write_LCD_L4("*Back          !Home");

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s

        // write LCD Test
        write_LCD_L1("This now kinda worksz");           // resolved
        write_LCD_L2("This now kindb works");           // resolved
        write_LCD_L3("This now kindc works");
        write_LCD_L4("This now kindd works");

        delay_ms(2000, FREQ_48_MHZ);                    // delay 2s
    }
}
