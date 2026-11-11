/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi JungHyun]
Created          : 10-10-2026
Modified         : 10-10-2026
Language/ver     : C in VS Code / Keil uVision

Description      : 11/12 lab assignement, using ultra sonic distance sensor
/----------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecSTM32F4v2.h"

#define TRIG_PIN    PA_6 // TIM3_CH1 : PWM Output(trigger)
#define ECHO_PIN    PB_6 // TIM4_CH1 : Input Capture(echo)

// given numbers in the lab report
#define PWM_PERIOD_MS   50 // 50ms trigger period
#define TRIG_PULSE_US   10 // 10us trigger pulse width
#define IC_TICK_US      10 // 10us counte

volatile uint32_t ovf_cnt = 0; // Overflows count during pulse measurement
volatile uint32_t cap_rise = 0; // get value from rising edge
volatile uint32_t cap_fall = 0; // get value from falling edge
volatile uint32_t timer_arr = 65536; // default ARR + 1 count(16bit timer)

volatile uint32_t pulse_ticks = 0; // count difference between edges
volatile float duration_us = 0.0f; // Pulse width in microseconds
volatile float distance_cm = 0.0f; // Calculated distance in cm

void setup(void) {
    RCC_PLL_init();// Set System Clock to 84 MHz
    SysTick_init(); // initialize SysTick
    GPIO_init(LED_PIN, OUTPUT); // Initialize status LED
    USART_init(USART2, PA_2, PA_3, 9600); //Initialize UART2 at 9600 baud rate

    // trigger pulse(PWM) on PA6(TIM3_CH1)
    PWM_init(TRIG_PIN, PWM_PERIOD_MS);
    PWM_duty(TRIG_PIN, (float)TRIG_PULSE_US / (PWM_PERIOD_MS * 1000.0f));

    // echo pulse
    // IC1 captures rising edge, IC2 captures falling edge
    ICAP_init(ECHO_PIN, IC_RISE); 
    ICAP_ICn_init(ECHO_PIN, IC_FALL, IC_2);// direct TI1 -> IC2 on falling edge
    ICAP_tick(ECHO_PIN, IC_TICK_US); //Set counter clock resolution to 10us

    // Store ARR + 1 for overflow handling
    timer_arr = TIM4->ARR + 1;
}

int main(void) {
    setup();
    printf("\r\n HC-SR04 Ultrasonic Distance Sensor Lab \r\n");
    printf("=========================================\r\n");

    while (1) {
        // print measured distance and raw pulse every 500ms
        printf("Duration: %.2f us | Distance: %.2f cm\r\n", duration_us, distance_cm);
        // toggle LED to show activity
        led_toggle(LED_PIN);
        // delay between serial outputs
        delay_ms(500);
    }
}

// TIM4 Interrupt Service Routine (Echo Pulse Measurement)
void TIM4_IRQHandler(void) {
    // Counter overflow check
    if (is_UIF(TIM4)) {
        ovf_cnt++;
        clear_UIF(TIM4);
    }

    // echo rising edge captured(IC1)
    if (is_CCIF(TIM4, IC_1)) {
        cap_rise = ICAP_read(ECHO_PIN, IC_1); // read counter on rising edge
        ovf_cnt = 0; // reset overflow count for this pulse
        clear_CCIF(TIM4, IC_1);
    }

    // echo falling edge(IC2)
    if (is_CCIF(TIM4, IC_2)) {
        cap_fall = ICAP_read(ECHO_PIN, IC_2); //read counter on falling edge
        // calculate total pulse ticks considering any timer overflows
        pulse_ticks = (cap_fall - cap_rise) + (ovf_cnt * timer_arr);
        // Calculate in microseconds
        duration_us = (float)pulse_ticks * IC_TICK_US;
        // calculate distance in cm = Pulse duration [us] / 58.0
        distance_cm = duration_us / 58.0f;

        clear_CCIF(TIM4, IC_2);
    }
}