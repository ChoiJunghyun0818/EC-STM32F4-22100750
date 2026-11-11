/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 05-03-2021
Modified         : 09-23-2026
Language/ver     : C++ in VS Code
Description      : LAB GPIO 7segement - Problem 1
/----------------------------------------------------------------*/


#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

// define FND LED segment pins (a ~ g, dot)
PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

// define FND digit select pins (FND_0 to FND_3)
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

void setup(void) {
    // use system clock
    RCC_HSI_init();
    
    // initialize FND segment pins and digit selection pins
    FND_init(pinFND);
    FND_select_init(selectFND);
}

int main(void) {
    setup();

    // digits display part
    uint8_t numDisplay1 = 0;  // display '0'
    uint8_t numDisplay2 = 5;  // display '5'
    uint8_t selectFND1 = 0;   // display on FND_0 (PA_10)
    uint8_t selectFND2 = 1;   // display on FND_1 (PA_11)

    while (1) {
        // display digit on FND0
        FND_select(selectFND1);
        FND_display(numDisplay1);
        delay_ms_HSI(1);      // Short delay to prevent flickering

        // Display FND1
        FND_select(selectFND2);
        FND_display(numDisplay2);
        delay_ms_HSI(1);      
    }
}

