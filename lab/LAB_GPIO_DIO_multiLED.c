/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 05-03-2021
Modified         : 09-21-2026
Language/ver     : C++ in VS Code
Description      : LAB GPIO Digital InOut - Problem 3 Toggle multiple LEDs with a button 
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"

// Segment pins: a, b, c, d, e, f, g, dp
PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
// pinName_t -> the data type
// pinFND -> array that holds 8 pin names and those are = 0~7

// Digit selection pins: FND0, FND1, FND2, FND3
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};
// the second array that holds only 4 pins = 10 11 4 3

void setup(void){
    RCC_HSI_init();         // Initialize System Clock -> HSI clock is needed
    FND_init(pinFND);       // Configure PB_0 ~ PB_7 as Output
    FND_select_init(selectFND); // Configure PA_10, PA_11, PC_4, PC_3 as Output
}

// FND_init and FND_select_init both have alot of functions within them in
    // ecGPIO2.c source file. instead od using each function and then adding them
    // we can call all of them by using FND_init and FND_selecr_init
//FND_init and FND_select_init all have speeds, pupdr and I/O selection within




int main(void) {
    setup();
    
    uint8_t numDisplay1 = 0; // Show '0'
    //unsigned 8-bit integer data type = uint8_t
    uint8_t numDisplay2 = 5; // Show '5'
    
    uint8_t selectFND1  = 0; // Digit 0
    uint8_t selectFND2  = 1; // Digit 1

    while (1) {
        FND_select(selectFND1);
        // turn on digit 0
        FND_display(numDisplay1);
        // display 0 on 7 seg
        delay_ms_HSI(1);        
        // keep degit for this time

        FND_select(selectFND2);
        FND_display(numDisplay2);
        delay_ms_HSI(1);    
    }
}