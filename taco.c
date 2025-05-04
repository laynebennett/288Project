#include "movement.h"
#include "open_interface.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "ping.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"
#include <stdio.h>
#include <stdbool.h>
#include "adc.h"
#include "servo.h"
#include "uart-interrupt.h"
#include "taco.h"
#include "scan.h"

int tacos = 0;
bool customerFound = false;



void supplyBot()
{
    // At start fill up on 3, display 3 Tacos to the LCD
    tacos = 3;
    lcd_printf("Tacos: %i", tacos);

    // Play resupply jingle
    music(0,1);

    // SEND TO PUTTY: Tacos stocked, begin selling
    uart_sendStr("Tacos stocked, begin selling \n");
}

void supplyCustomers()
{

    // Scan for initial customer

    fullScan(1, 180, 2);

    //TEMPORARY----------------------------------------------------------------------------------------------------------------------------
    customerFound = true;
    //REMOVE WHEN CUSTOMER SCANNING IMPLEMENTED--------------------------------------------------------------------------------------

    while(customerFound && tacos > 0)
    {
        // Scan for another customer to ensure it is there

        if(customerFound)
        {

            music(1,0);

            // When a customer is scanned in, decrement 1 taco
            tacos--;
            lcd_printf("Tacos: %i", tacos);
            timer_waitMillis(1000);

            if(tacos <= 0)
            {
                // if there is 0 tacos remaining, display, "Returning for re-supply"
                lcd_printf("Returning for re-supply");

                // SEND TO PUTTY: "return for re-supply"
                uart_sendStr("Tacos = 0. Return for re-supply ");
            }
        }
        else
        {
            // SEND TO PUTTY: "No customers found, do one more rescan to confirm"
            uart_sendStr("No customers found, do one more rescan to confirm ");

            // Scan once more

            fullScan(1, 180, 2);

            if(!customerFound)
            {
                // SEND TO PUTTY: "No customers confirmed, return to end your shift"
                uart_sendStr("No customers confirmed, return to end your shift ");
                break;
            }
        }

        fullScan(1, 180, 2);

    }
}

void music(int play_despacito, int play_home_jingle) {
    // "Despacito" melody snippet (16-note limit)
    unsigned char despacito_notes[16] = {
        74, 73, 71, 66, 66, 66, 66, 66,
        66, 71, 71, 71, 71, 69, 71, 67
    };

    unsigned char despacito_durations[16] = {
        32, 32, 16, 16, 8, 8, 8, 8,
        8, 8, 8, 8, 16, 8, 16, 24
    };

    // Fun home-base jingle (6 notes)
    unsigned char home_notes[6] = {
        60, 64, 67, 72, 76, 72
    };

    unsigned char home_durations[6] = {
        16, 16, 16, 16, 8, 8
    };

    // Load both songs into Roomba memory
    oi_loadSong(0, 16, despacito_notes, despacito_durations);
    oi_loadSong(1, 6, home_notes, home_durations);

    // Conditionally play one or both songs
    if (play_despacito) {
        oi_play_song(0); // play "Despacito" snippet
    }

    if (play_home_jingle) {
        oi_play_song(1); // play home-base jingle
    }
}
