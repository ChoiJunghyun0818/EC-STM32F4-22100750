/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 09-14-2026
Modified         : 09-21-2026 
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/


#include "ecGPIO2.h"

/*---------------------------------------------------------------- 
				[EXERCISE]
---------------------------------------------------------------- */
void GPIO_init(PinName_t pinName, uint32_t mode){     
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	
	// mode  : Input(0), Output(1), AlterFunc(2), Analog(3)   
	if (Port == GPIOA)
		RCC_GPIOA_enable();
	else if (Port == GPIOB)
		RCC_GPIOB_enable();
	else if (Port == GPIOC)
		RCC_GPIOC_enable();
	/*else if (Port == GPIOD)
		RCC_GPIOD_enable();
	else if (Port == GPIOE)
		RCC_GPIOE_enable();*/
	else if (Port == GPIOH)
		RCC_GPIOH_enable();

	GPIO_mode(pinName, mode);
}


// GPIO Mode          : Input(00), Output(01), AlterFunc(10), Analog(11)
void GPIO_mode(PinName_t pinName, uint32_t mode){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	Port->MODER &= ~(3UL << (2 * pin));     
	Port->MODER |= (mode << (2 * pin));    
}


// GPIO Speed         : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(PinName_t pinName, uint32_t speed){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	Port->OSPEEDR &= ~(3UL << (2 * pin));
	Port->OSPEEDR |= (speed << (2 * pin));
}


// GPIO Output Type   : Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(PinName_t pinName, uint32_t type){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	Port->OTYPER &= ~(1UL << pin);
	Port->OTYPER |= (type << pin);
}


// GPIO Push-Pull     : No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
void GPIO_pupd(PinName_t pinName, uint32_t pupd){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	Port->PUPDR &= ~(3UL << (2 * pin));
	Port->PUPDR |= (pupd << (2 * pin));
}


void GPIO_write(PinName_t pinName, uint32_t Output){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	if (Output == 0) {
		Port->ODR &= ~(1UL << pin);  // Write 0 to ODR bit (or use BSRR: Port->BSRR = (1UL << (pin + 16)))
	} else {
		Port->ODR |= (1UL << pin);   // Write 1 to ODR bit (or use BSRR: Port->BSRR = (1UL << pin))
	}
}


uint32_t GPIO_read(PinName_t pinName){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	uint32_t val = (Port->IDR >> pin) & 1UL;
	return val;
}

// FND - JKIT board static pin tracking
static PinName_t _fndPins[8];
static PinName_t _fndSel[4];

// Lookup table for 7-segment LED states (Common Cathode: 1 = ON, 0 = OFF)
// Pin order: {a, b, c, d, e, f, g, dp}
int numberFND[12][8] = {
    {1, 1, 1, 1, 1, 1, 0, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1, 0}, // 2
    {1, 1, 1, 1, 0, 0, 1, 0}, // 3
    {0, 1, 1, 0, 0, 1, 1, 0}, // 4
    {1, 0, 1, 1, 0, 1, 1, 0}, // 5
    {1, 0, 1, 1, 1, 1, 1, 0}, // 6
    {1, 1, 1, 0, 0, 1, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1, 0}, // 8
    {1, 1, 1, 1, 0, 1, 1, 0}, // 9
    {0, 0, 0, 0, 0, 0, 0, 1}, // dot
    {0, 0, 0, 0, 0, 0, 0, 0}  // blank
};

// Initialize DOUT pins for 7-segment LEDs (PB_0 to PB_7)
void FND_init(PinName_t *pinFND) {
    for (int i = 0; i < 8; i++) {
        _fndPins[i] = pinFND[i];
        GPIO_init(_fndPins[i], OUTPUT);
        GPIO_otype(_fndPins[i], EC_PUSH_PULL);
        GPIO_pupd(_fndPins[i], EC_NONE);
        GPIO_ospeed(_fndPins[i], EC_MEDIUM_SPEED);
    }
}

// Initialize digit select DOUT pins (PA_10, PA_11, PC_4, PC_3)
void FND_select_init(PinName_t *selectFND) {    
    for (int i = 0; i < 4; i++) {
        _fndSel[i] = selectFND[i];
        GPIO_init(_fndSel[i], OUTPUT);
        GPIO_otype(_fndSel[i], EC_PUSH_PULL);
        GPIO_pupd(_fndSel[i], EC_NONE);
    }
}

// Write segment data to active digit
void FND_display(uint8_t num) {
    for (int i = 0; i < 8; i++) {
        GPIO_write(_fndPins[i], numberFND[num][i]);
    }
}

// Select active digit (FND0 ~ FND3)
// Note: On JKIT, Digit select lines are Active LOW (0 = ON, 1 = OFF)
void FND_select(uint8_t digit) {
    for (int i = 0; i < 4; i++) {
        if (i == digit) {
            GPIO_write(_fndSel[i], HIGH);   // Turn ON target digit
        } else {
            GPIO_write(_fndSel[i], LOW);  // Turn OFF all other digits
        }
    }
}

void led_toggle(uint32_t pin) {
    GPIOA->ODR ^= (1UL << pin);
}

/*---------------------------------------------------------------- 
				[Optional EXERCISE]
---------------------------------------------------------------- */
// void pinMode(PinName_t pin, uint32_t mode){}
// void digitalWrite(PinName_t pin, uint32_t value){}
// uint32_t  digitalRead(PinName_t pin){}