/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Jung Hyun]
Created          : 05-03-2021
Modified         : 10-12-2026
Language/ver     : C++ in VS Code

Description      : Stepper Library for the stepper motor lab
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecStepper.h"


// Stepper Motor variable
static PinName_t stepperPins[4];
static volatile uint32_t _steps;
static volatile uint32_t _step_delay = 100;     
static const StateStepper_t *fsm;               // points at fsm_full or fsm_half (chosen in stepper_init)
static volatile uint32_t state = 0;             // current FSM state
static volatile uint32_t emergencyStop = 0;


// Output: A B A' B'
static const StateStepper_t fsm_Full[4] = { 
    {{S1, S3}, {HIGH, HIGH, LOW, LOW}},   // S0: DIR=0 -> S1, DIR=1 -> S3, Output: 1100
    {{S2, S0}, {LOW, HIGH, HIGH, LOW}},   // S1: DIR=0 -> S2, DIR=1 -> S0, Output: 0110
    {{S3, S1}, {LOW, LOW, HIGH, HIGH}},   // S2: DIR=0 -> S3, DIR=1 -> S1, Output: 0011
    {{S0, S2}, {HIGH, LOW, LOW, HIGH}}    // S3: DIR=0 -> S0, DIR=1 -> S2, Output: 1001
};


static const StateStepper_t fsm_Half[8] = {  
    {{S1, S7}, {HIGH, LOW, LOW, LOW}},    // S0
    {{S2, S0}, {HIGH, HIGH, LOW, LOW}},   // S1
    {{S3, S1}, {LOW, HIGH, LOW, LOW}},    // S2
    {{S4, S2}, {LOW, HIGH, HIGH, LOW}},   // S3
    {{S5, S3}, {LOW, LOW, HIGH, LOW}},    // S4
    {{S6, S4}, {LOW, LOW, HIGH, HIGH}},   // S5
    {{S7, S5}, {LOW, LOW, LOW, HIGH}},    // S6
    {{S0, S6}, {HIGH, LOW, LOW, HIGH}}    // S7
};


// Initialization of Stepper Motor
void stepper_init(uint32_t mode, PinName_t *pinStepper){
    if (mode == HALF) 
        fsm = fsm_Half;
    else
        fsm = fsm_Full;

    // GPIO Digital Out Initiation
    // For A, B, AN, BN
    for (int i = 0; i < 4; i++){
        stepperPins[i] = pinStepper[i];
        GPIO_init(stepperPins[i], OUTPUT);
        GPIO_pupd(stepperPins[i], EC_NONE); // Fixed function and macro name for ecGPIO2
    }
}

// Converts Motor [rpm] to step delay in [msec]
void stepper_speed(uint32_t speedRPM){      // rpm
    _step_delay = 60 * 1000 / (STEP_PER_REV * speedRPM);      // in [msec] 
    // [usec] can be used for a higher accuracy
    // But needs delay_us() 
}

void stepper_step(uint32_t steps, uint32_t direction)
{
    if (fsm == 0 || emergencyStop) return;

    _steps = steps;

    for (uint32_t i = 0; i < _steps; i++) {
        if (emergencyStop) {
            stepper_stop();
            return;
        }

        state = fsm[state].next[direction];
        stepper_out(state);
        delay_ms(_step_delay);
    }
}

// Stepper Motor Output for given state
void stepper_out(uint32_t state){
    // stepperPins[0]=A, stepperPins[1]=B, stepperPins[2]=A', stepperPins[3]=B'
    for (int i = 0; i < 4; i++){
        GPIO_write(stepperPins[i], fsm[state].out[i]);
    }
}


// Stop Stepper Motor
void stepper_stop(void) {
    _steps = 0;    
    
    // All pins DigitalOut '0'
    for (int i = 0; i < 4; i++){
        GPIO_write(stepperPins[i], LOW);
    }
}

void stepper_emergency_stop(void)
{
    emergencyStop = 1;
    stepper_stop();
}

void stepper_resume(void)
{
    emergencyStop = 0;
}

