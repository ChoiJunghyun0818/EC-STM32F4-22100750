/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 09-14-2026
Modified         : 05-03-2026 [WRITE THE DATE!!!!]
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

/*---------------------------------------------------------------- 
				[Optional EXERCISE]
---------------------------------------------------------------- */
// void pinMode(PinName_t pin, uint32_t mode){}
// void digitalWrite(PinName_t pin, uint32_t value){}
// uint32_t  digitalRead(PinName_t pin){}