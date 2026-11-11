/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [Choi Junghyun]
Created          : 09-14-2024
Modified         : 09-21-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

// Pin Modes
#define INPUT   0x00
#define OUTPUT  0x01
#define AF      0x02
#define ANALOG  0x03

// Pin States
#define HIGH    1
#define LOW     0

// Board Pin Definitions
#define LED_PIN     PA_5
#define BUTTON_PIN  PC_13

/*----------------------------------------------------------------
[EXERCISE] GPIO Configurations & Macros
---------------------------------------------------------------- */
// Resistor Mode
#define EC_NONE       0x00
#define EC_PU         0x01
#define EC_PD         0x02
#define EC_RESERVED   0x03

// Output Type
#define EC_PUSH_PULL  0x00
#define EC_OPEN_DRAIN 0x01

// Output Speed
#define EC_LOW_SPEED    0x00
#define EC_MEDIUM_SPEED 0x01
#define EC_FAST_SPEED   0x02
#define EC_HIGH_SPEED   0x03

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// Core GPIO Functions
void GPIO_init(PinName_t pinName, uint32_t mode);
void GPIO_mode(PinName_t pinName, uint32_t mode);
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
void GPIO_otype(PinName_t pinName, uint32_t type);
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
void GPIO_write(PinName_t pinName, uint32_t Output);
uint32_t GPIO_read(PinName_t pinName);

// Arduino Syntax (Optional)
void pinMode(PinName_t pin, uint32_t mode);
void digitalWrite(PinName_t pin, uint32_t value);
uint32_t digitalRead(PinName_t pin);

// FND for JKIT board 
extern int numberFND[12][8];
void FND_init(PinName_t *pinFND);
void FND_select_init(PinName_t *selectFND);
void FND_display(uint8_t num);
void FND_select(uint8_t digit);

void led_toggle(uint32_t pin);

#include "ecGPIO2.h"





#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H