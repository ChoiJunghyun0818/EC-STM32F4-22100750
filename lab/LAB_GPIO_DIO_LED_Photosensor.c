/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 05-03-2021
Modified         : 09-21-2026
Language/ver     : C++ in VS Code
Description      : LAB GPIO Digital InOut - Problem 2 Photosensor LED
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"
// include: tells the main file which header files i will use

#define SENSOR_PIN  PA_0    // photodetector input
#define LED_PIN     PB_12   // LED0 output
// define: A text search-and-replace command. PA_0 is already defined
        // in the header, but by using a friendly name like sensor_pin 
        // we can code more comfortably
        // 형식: #define (the name you want) (the already defined part) 

void setup(void);
// hey, theres gonna be a function names "setup" later. dont freak.
    // 일단 setup 만들고 비워놓기

int main(void) {
    // main start
    // int -> returns integral code when finished

    setup();
    // calls the setup from void setup(void); -> NOW we gon deal with this shit

    int sensorState = 0;
    // for any input, sensot, tool device we need to start it with 0

    // main loop
    while (1) {
        sensorState = GPIO_read(SENSOR_PIN);
        // okay now the sensor should read what is on the pin

        // dark condition: sensor output is 0 -> LED ON
        // bright condition: sensor output is 1 -> LED OFF
        if (sensorState == LOW) {
            GPIO_write(LED_PIN, LOW);
        } else {
            GPIO_write(LED_PIN, HIGH);
        }

        delay_ms_HSI(100); // 100 ms delay
    }
}

// initialiization function according to lab configuration table
void setup(void) {
    RCC_HSI_init();
    //Turns on HSI clock generator.

    // 1. configure photodetector (PA_0): Digital IN, Pull-Up
    GPIO_init(SENSOR_PIN, INPUT);
    // sensor pin = PA_0, this pin is now the input
    GPIO_pupd(SENSOR_PIN, EC_PU);
    // the sensor pin is a pull up register

    // 2. configure LED0 (PB_12): digital OUT, Pull-Up, Open-Drain, Medium Speed
    GPIO_init(LED_PIN, OUTPUT);
    GPIO_pupd(LED_PIN, EC_PU);
    GPIO_otype(LED_PIN, EC_OPEN_DRAIN);
    GPIO_ospeed(LED_PIN, EC_MEDIUM_SPEED);
}