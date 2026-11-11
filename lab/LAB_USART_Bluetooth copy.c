/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Choi JungHyun
Created          : 10-08-2026
Modified         : 10-26-2026
Language/ver     : C in VS Code

Description      : Problem 2 - bt chip stepper control(j-clockwise, k-ccw, p - emergency stop) - problem2
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
#include "ecStepper.h"

#define LD2_PIN PA_5

// pins connected to the stepper
PinName_t pinStepper[4] = {PC_5, PC_6, PC_7, PC_8};

static volatile uint8_t BT_Data = 0;
static volatile uint8_t cmd_flag = 0; // 0: stop, 1: CW, 2: CCW

void setup(void){
    RCC_PLL_init();            // 84 MHz
    SysTick_init();            // delay timer

    // pb12 is led 0 -> we need to turn it on
    GPIO_init(PB_12, OUTPUT);
    GPIO_otype(PB_12, EC_PUSH_PULL);
    GPIO_pupd(PB_12, EC_NONE);
    GPIO_ospeed(PB_12, EC_HIGH_SPEED);
    GPIO_write(PB_12, LOW);

    // stepper Motor step mode and speed
    stepper_init(HALF, pinStepper); 
    stepper_speed(15);              
}

int main(void) {
    setup();
    printf("MCU Stepper Control Ready\r\n");

    while (1) {
        // run CW direction 2 Full revolutions
        if (cmd_flag == 1) { 
            cmd_flag = 0;    // Clear flag
            
            // 8192 steps = 2 full rotations in half step mode
            stepper_step(8192, DIR_CW); 
            stepper_speed(15); 

            // in case p was pressed restart motor from that position
            stepper_resume(); 
            printf("CW Motion Completed/Finished.\r\n");
        } 
        
        // CCW Direction 2 full revolutions
        else if (cmd_flag == 2) { 
            cmd_flag = 0;    // clear flag
            printf("Running CCW: 2 Revolutions...\r\n");
            
            // 
            stepper_step(8192, DIR_CCW); 
            
            // Restart motor at stopped position
            stepper_resume(); 
            printf("CCW Motion Completed/Finished.\r\n");
        } 
    }
}


void USART1_IRQHandler(void){
    if (is_USART1_RXNE()) {
        BT_Data = USART1_read();     // read character over bluetooth
        USART1_write(&BT_Data, 1);   // send back to bluetooth Tera

        // led2 toggle feature
        if (BT_Data == 'H' || BT_Data == 'h') {
            GPIO_write(LD2_PIN, HIGH);
        } 
        else if (BT_Data == 'L' || BT_Data == 'l') {
            GPIO_write(LD2_PIN, LOW);
        }

        // stepper control commands
        else if (BT_Data == 'J' || BT_Data == 'j') {
            stepper_resume(); // motor run cw
            cmd_flag = 1;     
        } 
        else if (BT_Data == 'K' || BT_Data == 'k') {
            stepper_resume(); 
            cmd_flag = 2;     // motor run ccw
        } 
        else if (BT_Data == 'P' || BT_Data == 'p') {
            cmd_flag = 0;               // Clear motion commands
            stepper_emergency_stop();   // emergency stop in ISR
            printf("\r\n[BT] EMERGENCY STOP ACTIVATED!\r\n");
        }
    }
}