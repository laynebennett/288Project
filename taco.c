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
    music(0,1,0,0);

    // SEND TO PUTTY: Tacos stocked, begin selling
    uart_sendStr("Tacos stocked, begin selling \n");
}

void supplyCustomers()
{


    while(tacos > 0)
    {
        // Scan for another customer to ensure it is there

        if(fullScan(1, 180, 2, 100)){
            customerFound = true;
        }else{
            customerFound = false;
        }

        if(customerFound)
        {

            music(1, 0, 0, 0);
            timer_waitMillis(3630);
            music(0, 0, 1,0);
            timer_waitMillis(2665);
            music(0, 0, 0,1);

            // When a customer is scanned in, decrement 1 taco
            tacos--;
            lcd_printf("Tacos: %i", tacos);
            //timer_waitMillis(1000);

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

            if(fullScan(1, 180, 2, 100)){
                customerFound = true;
            }else{
                customerFound = false;
            }

            if(!customerFound)
            {
                // SEND TO PUTTY: "No customers confirmed, return to end your shift"
                uart_sendStr("No customers confirmed, return to end your shift ");
                break;
            }
        }



    }
}

void music(int play_despacito, int play_home_jingle, int play_despacito2, int play_despacito3) {
    // "Despacito" melody snippet (16-note limit)
    unsigned char despacito_notes[16] = {
        74, 73, 71, 66, 66, 66, 66, 66,
        66, 71, 71, 71, 71, 69, 71, 67
    };

    unsigned char despacito_durations[16] = {
        32, 32, 16, 16, 8, 8, 8, 8,
        8, 8, 8, 8, 16, 8, 16, 24
    };

    unsigned char despacito_notes2[16] = {
        67, 67, 67, 67, 71, 71, 71, 71,
        71, 73, 74, 69, 69, 69, 69, 69
    };

    unsigned char despacito_durations2[16] = {
        8, 8, 8, 8, 8, 8, 8, 8,
        16, 8, 16, 32, 8, 8, 8, 8
    };

    unsigned char despacito_notes3[8] = {
        69, 74, 74, 74, 74, 76, 76, 73,
    };

    unsigned char despacito_durations3[8] = {
        8, 8, 8, 8, 16, 8, 16, 32,
    };

    // Fun home-base jingle (6 notes)
    unsigned char home_notes[16] = {
        60, 60, 65, 69, 60, 60, 60,
        65, 69, 65, 65, 64, 64, 62, 62, 60
    };

    unsigned char home_durations[16] = {
       16, 8, 16, 16, 8, 8, 8, 16, 32,
        8, 8, 8, 8, 8, 8, 8
    };

    // Load both songs into Roomba memory
    oi_loadSong(0, 16, despacito_notes, despacito_durations);
    oi_loadSong(1, 16, home_notes, home_durations);
    oi_loadSong(2, 16, despacito_notes2, despacito_durations2);
    oi_loadSong(3, 16, despacito_notes3, despacito_durations3);

    // Conditionally play one or both songs
    if (play_despacito) {
        oi_play_song(0); // play "Despacito" snippet
    }

    if (play_despacito2) {
        oi_play_song(2); // play "Despacito2" snippet
    }

    if (play_despacito3) {
        oi_play_song(3); // play "Despacito2" snippet
    }


    if (play_home_jingle) {
        oi_play_song(1); // play home-base jingle
    }
}
