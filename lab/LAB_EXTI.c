/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Choi Junghyun
Created          : 2026-09-02
Modified         : 2026-10-08
Language/ver     : C (CMSIS / EC_HAL)

Description      : Problem 1 - 7-Segment Counter using EXTI & SysTick
/----------------------------------------------------------------*/

#include "ecRCC2.h"                         
#include "ecGPIO2.h"                      
#include "ecSysTick2.h"                     
#include "ecEXTI2.h"// needed headers

// pin definitions based on gitbook lab
#define SW1_PIN     PD_2 // define SW1 pin as port D pin 2 (reset button)
#define SW2_PIN     PA_4 // define SW2 pin as port A pin 4 (stop, go button)
#define FND_SEL0    PA_10 // define 7-segment digit enable pin as port A pin 10

// global state variables 
volatile uint8_t count = 0;                 
volatile uint8_t is_paused = 0;         
// volatile is used in interupts because it rereads the function again.
    // used for functions that arent fixed and are bound to change

// function prottypes
void setup(void);                           
void sevensegment_display(uint8_t num);     

// setup
void setup(void) {                          
    RCC_PLL_init(); // system clock to 84MHz
    SysTick_init(); // initialize sysick timer module for millisecond delays

    // initialize 7-Segment data Pins(PB0 to PB7) as output
    for (int i = 0; i < 8; i++) { // loop through 8 data pins, the 8 here is the number of loops
        GPIO_init(PB_0 + i, OUTPUT); // since i is 0~7 all the PB_0~7 are all outputs
        GPIO_pupd(PB_0 + i, EC_NONE); // set pupdr resistor setting to none
        GPIO_ospeed(PB_0 + i, EC_MEDIUM_SPEED); // set GPIO output speed mode to medium speed
    } // end of loop 

    // initialize 7-Segment digit selector pin(PA10)
    GPIO_init(FND_SEL0, OUTPUT); // PA10 digit enable pin as output
    GPIO_write(FND_SEL0, HIGH);  // PA10 pin HIGH to enable the FND_0 digit display

    // Initialize SW1(PD2) for reset button EXTI
    GPIO_init(SW1_PIN, INPUT); // configure PD2 pin as digital input
    GPIO_pupd(SW1_PIN, EC_PU); // enable internal pull-up resistor for active low button SW1
    EXTI_init(SW1_PIN, FALL, 2); // Enable EXTI on PD2, trigger on FALLING edge, set priority to 2

    // initialize PA4 for pause
    GPIO_init(SW2_PIN, INPUT); // PA4 pin as digital input
    GPIO_pupd(SW2_PIN, EC_PU); // pullup resistor for active low button SW2
    EXTI_init(SW2_PIN, FALL, 1); // enable EXTI on PA4, trigger on FALLING edge, set priority to 1
}   // End

// Main
int main(void) {  // program starting point
    setup();                                

    while (1) {  // infinite loop
        // display current number on 7Seg
        sevensegment_display(count); // output the current count digit

        // 1-second interval counting logic
        delay_ms(1000); // pause execution for 1000 milliseconds(1 second) using SysTick

        if (!is_paused) { // check if the timer is not in a paused state
            count = (count + 1) % 10; // increment count and wrap back to 0 after reaching 9
        }                                   
    }                                       
}                                           

// 7seg
void sevensegment_display(uint8_t num) {  // function to convert number to 7seg led
    // 7seg LED bit patterns for digits 0-9 (a, b, c, d, e, f, g, dot)
    const uint8_t number_map[10] = { // array mapping numbers 0-9 to their segment bit patterns
        0b00111111,// bit pattern to display digit 0
        0b00000110, // bit pattern to display digit 1
        0b01011011,// bit pattern to display digit 2
        0b01001111,// bit pattern to display digit 3
        0b01100110,// bit pattern to display digit 4
        0b01101101,// bit pattern to display digit 5
        0b01111101, // bit pattern to display digit 6
        0b00000111,// bit pattern to display digit 7
        0b01111111,// bit pattern to display digit 8
        0b01101111 // bit pattern to display digit 9
    };                                      

    if (num > 9) return;   // limit condition, ignores inputs greater than 9

    uint8_t pattern = number_map[num];      // translate the number to 8bit digit

    // write bits to PB0 through PB7
    for (int i = 0; i < 8; i++) { // loop through each bit from index 0 to 7
        uint8_t bit_val = (pattern >> i) & 0x01; // get the bit value for the current position
        GPIO_write(PB_0 + i, bit_val); // set the corresponding GPIO pin to output
    }                                       
}                                           

// EXTI2 interrupt handler
void EXTI2_IRQHandler(void) { 
    if (is_pending_EXTI(SW1_PIN)) { // check if the interrupt request is from SW1
        count = 0; // reset to 0
        clear_pending_EXTI(SW1_PIN); // clear the interrupt flag
    }                                       
}                                           

// EXTI4 interrupt handler
void EXTI4_IRQHandler(void) { 
    if (is_pending_EXTI(SW2_PIN)) { // check if the interrupt request is from SW2
        is_paused = !is_paused; // Toggle flag
        clear_pending_EXTI(SW2_PIN); // clear 
    }                                       
}                                           