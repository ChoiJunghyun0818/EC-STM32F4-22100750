/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 05-03-2021
Modified         : 09-21-2026
Language/ver     : C++ in VS Code
Description      : LAB GPIO Digital InOut - Problem 3 Toggle multiple LEDs with a button 
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

// pin definitions accodring to site
#define BUTTON_PIN  PA_4    

#define NUM_LEDS    4
PinName_t ledPins[NUM_LEDS] = {PB_12, PB_13, PB_14, PB_15}; // LED0 to LED3

void setup(void);

int main(void) {
    // start
    setup();

    int state = 0;              // current active LED index
    int btnState = 1;          // current button state
    int prevBtnState = 1;      // previous button state for edge detection

    // turn on initial LED and turn off all others
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i == state) {
            GPIO_write(ledPins[i], HIGH);
        } else {
            GPIO_write(ledPins[i], LOW);
        }
    }

    // infinite loop
    while (1) {
        btnState = GPIO_read(BUTTON_PIN);

        // falling edge detection: detect when button is newly pressed
        if (prevBtnState == HIGH && btnState == LOW) {
            // move to the next LED in sequence
            state = (state + 1) % NUM_LEDS;

            // update LED: turn ON active LED, turn OFF others
            for (int i = 0; i < NUM_LEDS; i++) {
                if (i == state) {
                    GPIO_write(ledPins[i], HIGH);
                } else {
                    GPIO_write(ledPins[i], LOW);
                }
            }
        }

        prevBtnState = btnState; // save current state for next turn
        delay_ms_HSI(50);        // 50 ms delay 
    }
}

// entire function
void setup(void) {
    RCC_HSI_init();

    // configure switch SW2 (PA_4): Digital IN, Pull-Up
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, EC_PU);

    // configure LED_0 ~ LED_3 (PB_12, PB_13, PB_14, PB_15):
    // digital OUT, Pull-Up, Open-Drain, Medium Speed
    for (int i = 0; i < NUM_LEDS; i++) {
        GPIO_init(ledPins[i], OUTPUT);
        GPIO_pupd(ledPins[i], EC_PU);
        GPIO_otype(ledPins[i], EC_OPEN_DRAIN);
        GPIO_ospeed(ledPins[i], EC_MEDIUM_SPEED);
    }
}