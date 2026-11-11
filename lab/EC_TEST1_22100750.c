/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Jung Hyun]
Student ID       : [22100750]
Created          : 10-22-2026
Language/ver     : C in VS Code

Description      : EC Test 1 (2026) Programming Test
/----------------------------------------------------------------*/

//==================================================================
//  EC Library Include
//==================================================================
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper.h"

#define SPEEDINCREASE_PIN PD_2
#define SPEEDDECREASE_PIN PA_4

#define SENSOR_PIN  PA_0    // photodetector input
#define LED_PIN     PB_12   // LED0 output


//==================================================================
// User GPIO Pin definitions
//==================================================================
PinName_t pinStepper[4]    = {PC_5,PC_6,PC_7,PC_8}; // A B A' B'

volatile uint32_t motorRunning = 0;

static volatile uint8_t cmd_flag = 0; // 0: stop, 1: CW, 2: CCW
// Initialiization 

// define FND LED segment pins (a ~ g, dot)
PinName_t pinFND[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

// define FND digit select pins (FND_0 to FND_3)
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

void setup(void);

void setup(void){	
	RCC_PLL_init();                                 // System Clock = 84MHz
	SysTick_init();                                 // Systick init
	
	GPIO_init(PD_2, INPUT);           		
    EXTI_init(PA_4, FALL,0);           		

	GPIO_init(SPEEDINCREASE_PIN, INPUT);
    GPIO_pupd(SPEEDINCREASE_PIN, EC_PU);

	stepper_init(FULL, pinStepper); 				// Stepper GPIO pin initialization
	stepper_speed(0);                          		//  set stepper motor speed
	
	RCC_PLL_init();            // 84 MHz
    SysTick_init();            // delay timer

    // pb12 is led 0 -> we need to turn it on
    GPIO_init(PB_12, OUTPUT);
    GPIO_otype(PB_12, EC_PUSH_PULL);
    GPIO_pupd(PB_12, EC_NONE);
    GPIO_ospeed(PB_12, EC_HIGH_SPEED);
    GPIO_write(PB_12, LOW); 

	 // use system clock
    RCC_HSI_init();
    
    // initialize FND segment pins and digit selection pins
    FND_init(pinFND);
    FND_select_init(selectFND);

	RCC_HSI_init();
    //Turns on HSI clock generator.

    // 1. configure photodetector (PA_0): Digital IN, Pull-Up
    GPIO_init(SENSOR_PIN, INPUT);
    // sensor pin = PA_0, this pin is now the input
    GPIO_pupd(SENSOR_PIN, EC_PU);
    // the sensor pin is a pull up register

    // 2. configure LED0 (PB_12): digital OUT, Pull-Up, Open-Drain, Medium Speed
    GPIO_init(LED_PIN, OUTPUT);
    GPIO_pupd(LED_PIN, EC_PU);
    GPIO_otype(LED_PIN, EC_OPEN_DRAIN);
    GPIO_ospeed(LED_PIN, EC_MEDIUM_SPEED);

}

// Global Variables
// use   volatile uint32_t   instead of uint32_t if used in Interrupt ISR
volatile uint32_t motor_mode   = 0;	
volatile uint32_t motor_dir    = 0;	
volatile uint32_t motor_speed  = 7;	
// YOUR CODE GOES HERE
// YOUR CODE GOES HERE
// YOUR CODE GOES HERE

//==================================================================
//  Setup 
//==================================================================
void setup(void){
	RCC_PLL_init();					// System Clock = 84MHz
	SysTick_init();					// SysTick, 1 msec
	

	uint8_t count = 0;
    int sw1 = HIGH;
    int sw2 = HIGH;

    while (1) {
        // read button current number
        sw1 = GPIO_read(SPEEDINCREASE_PIN);
		sw2 = GPIO_read(SPEEDDECREASE_PIN);

        // falling edge detection
        if (sw1 == LOW && sw2 == HIGH) {
            // button sw1 is pressed
            stepper_step(motor_speed +500, DIR_CW);
			delay_ms(1000);
        }
        
        if (sw1 == HIGH && sw2 == LOW) {
            // button sw2 is pressed
            stepper_step(motor_speed -500, DIR_CW);
			delay_ms(1000);


        }
	}
}
	// USART2, 9600 baud
	// YOUR CODE GOES HERE			
	// YOUR CODE GOES HERE			

	// GPIO Inputs
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

	// EXTI 
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	
	// GPIO Outputs	
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

	// Other Setup
	// YOUR CODE GOES HERE



//==================================================================
//  Main
//==================================================================
int main(void){
	setup();

	uint32_t rev=1;
	uint32_t steps=STEP_PER_REV*2; // this 2 is aslo changed when half(problem 6)
	
	// Inifinite Loop ----------------------------------------------------------
	volatile uint32_t motorRunning = 1;
	stepper_step(2048, DIR_CW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
	delay_ms(2000);
		
    // calls the setup from void setup(void); -> NOW we gon deal with this shit

    int sensorState = 0;
    // for any input, sensot, tool device we need to start it with 0

    // main loop
    while (1) {
        sensorState = GPIO_read(SENSOR_PIN);
        // okay now the sensor should read what is on the pin

        // dark condition: sensor output is 0 -> LED ON
        // bright condition: sensor output is 1 -> LED OFF
        if (sensorState == LOW) {
            GPIO_write(LED_PIN, LOW);
        } else {
            GPIO_write(LED_PIN, HIGH);
        }

        //delay_ms_HSI(100); // 100 ms delay

		
		
	
		
		// Check PAUSE/RESUME 
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE

		// Related to controlling motor speed/dir etc
		// HINT:  use  stepper_speed(vel);	only when vel>0
		//stepper_step(1, motor_dir);	 // Keep step=1 or a low value to stop blocking other code
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE


		// Print to the serial monitor every 2 sec
		// PAUSE, blink at 2 sec
		//     e.g.  printf("Mode=%lu Speed=%lu DIR=%lu\r\n", ... );
		// HINT:  msTicks in ecSysTick2.h  is increasing at a 1 msec rate accurately.
		// You can use msTicks  to count every 2000 msec 
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE

			
	delay_ms(1);  // use this delay
		}
	}



//==================================================================
//  EXTI Interrupt Service Routines
//==================================================================

	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE


//==================================================================
//  UART2 Interrupt Service Routines
//==================================================================
//void USART2_IRQHandler(void){
//	if(is_USART2_RXNE()){
	
	// HINT: 	comparing a character:  
	//  		uint8_t ch = USART2_read();		
	//			if(ch == 'C' || ch == 'c')	 etc..
	//
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
//	}
//}
