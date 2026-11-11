/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 05-03-2021
Modified         : 09-23-2026
Language/ver     : C++ in VS Code
Description      : LAB GPIO 7segement - Problem 2
/----------------------------------------------------------------*/


#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

#define BUTTON_PIN PA_4

PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

void setup(void);

int main(void) {
    setup();

    uint8_t count = 0;
    int btnState = HIGH;
    int prevBtnState = HIGH;

    while (1) {
        // read button current number
        btnState = GPIO_read(BUTTON_PIN);

        // falling edge detection
        if (prevBtnState == HIGH && btnState == LOW) {
            // high = button not pressed, btnstate = low -> button pressed
            // USUALLY low is pressed. depends on code

            count = (count + 1) % 20;  // counts 0 ~ 19 using %20
            // %20 calcualtes the remainder and gives us the 나머지.
            //ex) 22%20 = 1...2 -> 나머지 2 -> shows 2
            delay_ms_HSI(50);          // software debouncing delay
        }
        
        prevBtnState = btnState;

        // get digits
        uint8_t tens  = count / 10;
        uint8_t units = count % 10;
        // tens + unit is the displayed number when we press the button
        // this is needed becasue we need to display the number after we press it

        // multiplex display logic
        // display units on very left(FND0)
        FND_select(0);
        // 0 -> 일의 자리 1 -> 십의자리 2-> 백의자리 3 -> 천의자리
        FND_display(units);
        // this displays the units. if i put 3 in the () it displays 3
        delay_ms_HSI(2);

        // display tens on FND1
        if (count >= 10) {
            FND_select(1);
            FND_display(tens);
            delay_ms_HSI(2);
        } else {
            // turn off FND1 for single digit
            FND_select(1);
            FND_display(11);
            delay_ms_HSI(2);
        }
    }
}

void setup(void) {
    RCC_HSI_init();

    // SW2 Button is Input with Pull-Up
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, EC_PU);

    // segment pins and select pins
    FND_init(pinFND);
    FND_select_init(selectFND);
}