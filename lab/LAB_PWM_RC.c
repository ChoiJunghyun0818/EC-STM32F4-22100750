/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 11-02-2026
Modified         : 11-07-2026
Language/ver     : C++ in VS Code
Description      : RC motor 10degree angle control, lab problem1
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecPWM2.h"
#include "ecTIM2.h"
#include "ecEXTI2.h"

#define PWM_PIN     PA_1
#define BUTTON_PIN  PC_13

volatile int angle = 0;
volatile int direction = 1;

void setup(void);
void TIM3_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

int main(void) {
    setup();

    while (1) {
        // All control logic handled by interrupts
    }
}

void setup(void) {

    // System clock: 84 MHz
    RCC_PLL_init();

    // Servo mototr PWM: TIM2_CH2, 20ms period
    PWM_init(PA_1, 20);
    PWM_pulsewidth_us(PWM_PIN, 500);

    // TIM3 interrupt: 500ms
    TIM_UI_init(TIM3, 500);
    TIM_UI_enable(TIM3);

    NVIC_SetPriority(TIM3_IRQn, 1);
    NVIC_EnableIRQ(TIM3_IRQn);

    // button PC_13 setting
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, EC_PU);

    EXTI_init(BUTTON_PIN, FALL, 0);

    NVIC_SetPriority(EXTI15_10_IRQn, 0);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}


// TIM3: Every 500ms
void TIM3_IRQHandler(void) {

    if (is_UIF(TIM3)) {

        clear_UIF(TIM3);

        // angle change by 10 degrees
        angle += direction * 10;

        // Reverse direction at boundaries
        if (angle >= 180) {
            angle = 180;
            direction = -1;
        }

        else if (angle <= 0) {
            angle = 0;
            direction = 1;
        }

        // Convert angle to pulse width(500~2500us)
        uint32_t pulse_width = 500 + (angle * 2000) / 180;

        PWM_pulsewidth_us(PWM_PIN, pulse_width);
    }
}


// EXTI: Button pressed
void EXTI15_10_IRQHandler(void) {

    if (is_pending_EXTI(BUTTON_PIN)) {

        // Clear interrupt flag
        clear_pending_EXTI(BUTTON_PIN);

        // Reset motor state
        angle = 0;
        direction = 1;

        // Move motor to 0 degrees
        PWM_pulsewidth_us(PWM_PIN, 500);
    }
}
