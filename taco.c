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

int supply()
{
    // At start fill up on 3, display 3 Tacos to the LCD
    tacos += 3;
    lcd_printf("Tacos: " + tacos);
    // Wait until the curb is found
    if(curb)
    {
        // Scan for customers
        if(customerFound)
        {
            while(customerFound && tacos > 0)
            {
                if(customerFound)
                {
                    // When a customer is scanned in, decrement 1 taco
                    tacos - 1;
                    lcd_printf("Tacos: " + tacos);

                    if(tacos == 0)
                    {
                        // if there is 0 tacos remaining, display, "Returning for re-supply"
                        lcd_printf("Returning for re-supply")

                        // Execute re-supply measure
                    }
                }
            }
        }

        if(!customerFound)
        {
            lcd_printf("No customers found, scanning again for customers");
        }
    }

    // if a cliff is detected, display "Re-supplying tacos..."
     
    // Play a confirmation sound once taco's are refilled

    // Set taco count to 3 on LCD
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
