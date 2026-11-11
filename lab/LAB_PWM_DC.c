/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Choi Junghyun
Created          : 11-07-2026
Modified         : 11-07-2026
Language/ver     : C / C++ in VS Code
Description      : DC motor fan control (Lab Problem 2 - Smart Mini-Fan)
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecPWM2.h"
#include "ecTIM2.h"
#include "ecEXTI2.h"
#include "ecUART2.h"  
#include <stdio.h>

#define PWM_PIN       PA_0       // TIM2_CH1
#define DIR_PIN       PC_2       // motor direction
#define BUTTON_PIN    PC_13      // speed button B1
#define LED_PIN       PA_5       // Onboard LED
#define IR_PIN        PB_0       // IR sensor

#define IR_DETECTED_LEVEL  0     // LOW(0) = Object detected (Face nearby)

typedef enum {
    FAN_OFF,
    FAN_MID,
    FAN_HIGH,
    FAN_PAUSE
} FanState_t;

volatile FanState_t fanState = FAN_OFF;
volatile FanState_t previousState = FAN_OFF; // Stores active mode before stopping

volatile uint8_t button_event = 0;
volatile uint8_t button_lockout = 0;

volatile uint8_t blink_event = 0;
volatile uint8_t serial_event = 0;

volatile int blink_counter = 0;
volatile int serial_counter = 0;

uint8_t led_state = 0;
float currentDuty = 0.0f;

// retarget standard C stdout printf to UART2 for Terra Term for display
int __io_putchar(int ch) {
    USART2_write((uint8_t*)&ch, 1);
    return ch;
}


void setup(void);
void applyOutputs(void);
void changeMode(void);
const char* getStateName(FanState_t state);

void TIM3_IRQHandler(void);
void EXTI15_10_IRQHandler(void);


int main(void) {
    setup();

    while (1) {
        // check IR sensor
        int objectDetected = (GPIO_read(IR_PIN) == IR_DETECTED_LEVEL);

        // Object detected -> Transition to PAUSE
        if (objectDetected && fanState != FAN_PAUSE) {
            previousState = fanState;       // Save active running state
            fanState = FAN_PAUSE;
            
            button_event = 0;               // Clear pending button events
            led_state = 0;
            GPIO_write(LED_PIN, 0);

            applyOutputs();
        }
        // Object away -> RESUME previous mode
        else if (!objectDetected && fanState == FAN_PAUSE) {
            fanState = previousState;       // Restore saved state
            led_state = 0;

            applyOutputs();
        }

        // Button event
        if (button_event) {
            button_event = 0;

            // Button B1 MUST NOT change mode during PAUSE
            if (fanState != FAN_PAUSE) {
                changeMode();
                applyOutputs();
            }
        }

        
        // LED blinking
        if (blink_event) {
            blink_event = 0;

            if (fanState == FAN_PAUSE) {
                led_state = !led_state;
                GPIO_write(LED_PIN, led_state);
            }
        }

        // terra form displayu
        static FanState_t lastPrintedState = (FanState_t)-1;
        
        if (serial_event) {
            serial_event = 0;

            if (fanState != FAN_OFF || fanState != lastPrintedState) {
                printf("State: %-5s | PWM Duty: %3.0f%% | IR Sensor: %s\r\n",
                   getStateName(fanState),
                   currentDuty * 100.0f,
                   (fanState == FAN_PAUSE) ? "DETECTED (PAUSED)" : "CLEAR");

                lastPrintedState = fanState; // Update last printed state
            }
        }
    }
}


void setup(void) {
    // System Clock
    RCC_PLL_init();

    // UART Initialization for Terra Term (9600 Baud)
    USART2_init();

    // GPIO Configuration
    GPIO_init(DIR_PIN, OUTPUT);
    GPIO_write(DIR_PIN, 0);// Set forward direction

    GPIO_init(LED_PIN, OUTPUT);
    GPIO_write(LED_PIN, 0);// Initial LED state OFF

    GPIO_init(IR_PIN,0);
    GPIO_pupd(IR_PIN, EC_PU);              
    // Pull-up for IR proximity digital pin

    GPIO_init(BUTTON_PIN, 0);
    GPIO_pupd(BUTTON_PIN, EC_PU);          
    // Pull-up for User Button PC13

    // PWM Configuration (1kHz Frequency on PA0)
    PWM_init(PWM_PIN, 1);// 1ms period = 1kHz
    PWM_duty(PWM_PIN, 0.0f);// Initial duty 0%

    //Timer Interrupt Setup(TIM3 - 500ms Update Period)
    TIM_UI_init(TIM3, 500);
    TIM_UI_enable(TIM3);
    NVIC_SetPriority(TIM3_IRQn, 1);
    NVIC_EnableIRQ(TIM3_IRQn);

    //External Interrupt Setup for PC13 Button
    EXTI_init(BUTTON_PIN, FALL, 0);
    NVIC_SetPriority(EXTI15_10_IRQn, 0);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}
// changing fan speed from 0-> mid -> max
void changeMode(void) {
    switch (fanState) {
        case FAN_OFF:
            fanState = FAN_MID;
            break;
        case FAN_MID:
            fanState = FAN_HIGH;
            break;
        case FAN_HIGH:
            fanState = FAN_OFF;
            break;
        default:
            break;
    }
}

void applyOutputs(void) {
    switch (fanState) {
        case FAN_OFF:
            currentDuty = 0.0f;
            PWM_duty(PWM_PIN, currentDuty);
            GPIO_write(LED_PIN, 0);
            break;

        case FAN_MID:
            currentDuty = 0.5f;
            PWM_duty(PWM_PIN, currentDuty);
            GPIO_write(LED_PIN, 1);
            break;

        case FAN_HIGH:
            currentDuty = 1.0f;
            PWM_duty(PWM_PIN, currentDuty);
            GPIO_write(LED_PIN, 1);
            break;

        case FAN_PAUSE:
            currentDuty = 0.0f;
            PWM_duty(PWM_PIN, currentDuty);
            // LED blinking is handled dynamically in main loop via blink_event
            break;
    }
}


// get state names for display

const char* getStateName(FanState_t state) {
    switch (state) {
        case FAN_OFF:   return "OFF";
        case FAN_MID:   return "MID";
        case FAN_HIGH:  return "HIGH";
        case FAN_PAUSE: return "PAUSE";
        default:        return "UNKNOWN";
    }
}

// TIM3 interrupt
void TIM3_IRQHandler(void) {
    if (is_UIF(TIM3)) {
        clear_UIF(TIM3);

        // Button debounce lockout counter
        if (button_lockout > 0) {
            button_lockout--;
        }

        // LED toggle flag every 1 second(2*500ms)
        blink_counter++;
        if (blink_counter >= 2) {
            blink_counter = 0;
            blink_event = 1;
        }

        //Serial print flag every 2 seconds (4*500ms)
        serial_counter++;
        if (serial_counter >= 4) {
            serial_counter = 0;
            serial_event = 1;
        }
    }
}

// EXTI interrupt handler
void EXTI15_10_IRQHandler(void) {
    if (is_pending_EXTI(BUTTON_PIN)) {
        clear_pending_EXTI(BUTTON_PIN);

        // Debounce lock: Ignore additional edges within lockout window
        if (button_lockout == 0) {
            button_lockout = 1; // Locked out for 500ms

            // Ignore button presses during PAUSE state
            if (fanState != FAN_PAUSE) {
                button_event = 1;
            }
        }
    }
}