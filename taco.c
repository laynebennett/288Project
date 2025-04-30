#include "movement.h"
#include "open_interface.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "ping.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"
#include<stdio.h>
#include <stdbool.h>
#include "adc.h"
#include "servo.h"

int tacos = 0;
bool curb = false;
bool customerFound = false;

int void supply()
{
    // At start fill up on 3, display 3 Tacos to the LCD
    tacos = 3;
    lcd_printf("Tacos: " + tacos);

    // Play resupply jingle
    music(0,1);

    // SEND TO PUTTY: Tacos stocked, begin selling

int void supplyCustomers()
{
    while(customerFound && tacos > 0)
    {
        // Scan for another customer

        if(customerFound)
        {
            // When a customer is scanned in, decrement 1 taco
            tacos - 1;
            lcd_printf("Tacos: " + tacos);

            if(tacos == 0)
            {
                // if there is 0 tacos remaining, display, "Returning for re-supply"
                lcd_printf("Returning for re-supply")

                // SEND TO PUTTY: "return for re-supply"
            }
        }
        else
        {
            // SEND TO PUTTY: "No customers found, do one more rescan to confirm"

            // Scan once more

            if(!customerFound)
            {
                // SEND TO PUTTY: "No customers confirmed, return to end your shift"
                break;
            }
        }
    }
}

void music(int play_despacito, int play_home_jingle) {
    // "Despacito" melody snippet (16-note limit)
    unsigned char despacito_notes[16] = {
        76, 74, 72, 71, 72, 74, 76, 74,
        72, 71, 72, 74, 76, 76, 74, 72
    };

    unsigned char despacito_durations[16] = {
        16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 8, 8, 16
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
