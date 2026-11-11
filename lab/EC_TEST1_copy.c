/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Jung Hyun]
Student ID       : [22100750]
Created          : 10-22-2026
Language/ver     : C in VS Code

Description      : EC Test 1 (2026) Programming Test
                   
                   Rename this file to  EC_Test1_yourID.c
/----------------------------------------------------------------*/


#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper.h"

#define SPEEDINCREASE_PIN PD_2
#define SPEEDDECREASE_PIN PA_4


PinName_t pinStepper[4]    = {PC_5,PC_6,PC_7,PC_8}; // A B A' B'

volatile uint32_t motorRunning = 0; // initially mode 0

// Initialiization 
void setup(void){	
	RCC_PLL_init();                                 // System Clock = 84MHz
	SysTick_init();                                 // Systick init
	
	GPIO_init(PD_2, INPUT);           		
    EXTI_init(PC_13, FALL,0);           		
	// 보통 fall은 버튼을 눌렀을때, rise는 버튼에서 손을 뗐을때

	GPIO_init(SPEEDINCREASE_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, EC_PU);

	stepper_init(FULL, pinStepper); 				// Stepper GPIO pin initialization
	// this part is changed based on the full or half -> problem 6
	stepper_speed(0);                          		//  set stepper motor speed

	GPIO_init(PC_13, INPUT);  // Replace BT2_PIN with your actual pin
	EXTI_init(PC_13, FALL, 0);

}


void setup(void);

int main(void) {
    setup();

    uint8_t count = 0;
    int btnState = HIGH;
    int prevBtnState = HIGH;

    

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