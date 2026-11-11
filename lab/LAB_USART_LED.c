/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Choi JungHyun
Created          : 10-08-2026
Modified         : 10-19-2026
Language/ver     : C in VS Code

Description      : usart1(pc), usart2(pc) connection + led2 control(problem 1)
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h" // uploaded to include ecUART2.h, ecGPIO2.h, ecRCC2.h

#define LD2_PIN PA_5 // pin selection

static volatile uint8_t PC_Data = 0;
static volatile uint8_t BT_Data = 0;
uint8_t PC_string[] = "Loop:\r\n";

void setup(void){
    RCC_PLL_init();
    SysTick_init();
    
    // Led2 Pin(PA_5) as output
    GPIO_init(LD2_PIN, OUTPUT);
    GPIO_otype(LD2_PIN, EC_PUSH_PULL); // push pull
    GPIO_pupd(LD2_PIN, EC_NONE);
    GPIO_ospeed(LD2_PIN, EC_HIGH_SPEED); // pin speed
    GPIO_write(LD2_PIN, LOW); // start with Led2 off(low)
    
    // USART2: USB serial
    USART2_init(); // default 9600
    //baud at 38400;
    
    // USART1: differenct pc serial
    USART1_init(); // default 9600
    //baud at 38400;
}

int main(void){ 
    setup();
    printf("MCU Initialized\r\n");  
    while(1){
        // use interrupt only for the recieve
        // polling for the transmit
        // PC_string at 7
        delay_ms(2000);        
    }
}

// USART2 RX interrupt is the input from PC keyboard
void USART2_IRQHandler(void){ 
    if(is_USART2_RXNE()){
        PC_Data = USART2_read();        // RX from UART2
        USART2_write(&PC_Data, 1);      // TX to USART2(PC_Data is from the keybaord)
        USART1_write(&PC_Data, 1);      // TX to Other MCU over USART1
    }
}

// USART1 RX interrupt is the input from PC keyboard
void USART1_IRQHandler(void){ 
    if(is_USART1_RXNE()){
        BT_Data = USART1_read();            // RX from other MCU    
        printf("RX: %c \r\n", BT_Data);    // TX to USART2
        
        // Control Led2 on this MCU based on received command from other MCU
        if (BT_Data == 'H' || BT_Data == 'h') {
            GPIO_write(LD2_PIN, HIGH);      // Turn ON Led2
            printf("LD2: ON\r\n");
        } 
        else if (BT_Data == 'L' || BT_Data == 'l') {
            GPIO_write(LD2_PIN, LOW);       // Turn OFF Led2
            printf("LD2: OFF\r\n");
        }
    }
}