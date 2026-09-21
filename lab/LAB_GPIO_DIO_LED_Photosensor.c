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

#define SENSOR_PIN  PA_0    // photodetector input
#define LED_PIN     PB_12   // LED0 output

void setup(void);

int main(void) {
    // main start
    setup();

    int sensorState = 0;

    // main loop
    while (1) {
        sensorState = GPIO_read(SENSOR_PIN);

        // dark condition: sensor output is 0 -> LED ON
        // bright condition: sensor output is 1 -> LED OFF
        if (sensorState == LOW) {
            GPIO_write(LED_PIN, HIGH);
        } else {
            GPIO_write(LED_PIN, LOW);
        }

        delay_ms_HSI(100); // 100 ms delay
    }
}

// initialiization function according to lab configuration table
void setup(void) {
    RCC_HSI_init();

    // 1. configure photodetector (PA_0): Digital IN, Pull-Up
    GPIO_init(SENSOR_PIN, INPUT);
    GPIO_pupd(SENSOR_PIN, EC_PU);

    // 2. configure LED0 (PB_12): digital OUT, Pull-Up, Open-Drain, Medium Speed
    GPIO_init(LED_PIN, OUTPUT);
    GPIO_pupd(LED_PIN, EC_PU);
    GPIO_otype(LED_PIN, EC_OPEN_DRAIN);
    GPIO_ospeed(LED_PIN, EC_MEDIUM_SPEED);
}