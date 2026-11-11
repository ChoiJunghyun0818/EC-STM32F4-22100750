/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Jung Hyun]
Created          : 05-03-2021
Modified         : 10-12-2026
Language/ver     : C++ in VS Code

Description      : Main code for controlling stepper motor
/----------------------------------------------------------------*/


#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper.h"

// ( A, B,  AN,  BN)
PinName_t pinStepper[4]    = {PB_10,PB_4,PB_5,PB_3};

volatile uint32_t motorRunning = 1;

// Initialiization 
void setup(void){	
	RCC_PLL_init();                                 // System Clock = 84MHz
	SysTick_init();                                 // Systick init
	
	GPIO_init(PC_13, INPUT);           		// GPIOC pin13 initialization
    EXTI_init(PC_13, FALL,0);           		// External Interrupt Setting
	// 보통 fall은 버튼을 눌렀을때, rise는 버튼에서 손을 뗐을때

	stepper_init(HALF, pinStepper); 				// Stepper GPIO pin initialization
	// this part is changed based on the full or half -> problem 6
	stepper_speed(15);                          		//  set stepper motor speed

	GPIO_init(PC_13, INPUT);  // Replace BT2_PIN with your actual pin
	EXTI_init(PC_13, FALL, 0); 

}
// problem 2. stepper speed 1 rotates the stepper motor about 1 revolutions with 1 rpm.
// meaning 1 full rotation takes a minute 
// problem 4. the max speed is 14
// problem 6. the max speed is 29 for half


int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	uint32_t rev=1;
	uint32_t steps=STEP_PER_REV*2; // this 2 is aslo changed when half(problem 6)
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		stepper_step(2048, DIR_CW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		delay_ms(2000);
		stepper_step(2048, DIR_CCW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		delay_ms(2000);
	}
}

// problem 1. 2048 steps are needed for the 1 rotation full stepper
// problem 3. doing problem 2 the opposite way around is poosible with DIR_CCW with a 2 second delay



void EXTI15_10_IRQHandler(void)
{
    if (is_pending_EXTI(PC_13)) {
        if (motorRunning) {
            motorRunning = 0;
            stepper_emergency_stop();
        } else {
            motorRunning = 1;
            stepper_resume();
        }

        clear_pending_EXTI(PC_13);
    }
}