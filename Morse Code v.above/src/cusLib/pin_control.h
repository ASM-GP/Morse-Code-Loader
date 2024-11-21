#ifndef __PIN_CONTROL_C
#define __PIN_CONTROL_C

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

#define INPUT                  0x0
#define OUTPUT                 0x1
#define INPUT_PULLUP           0x2

#define LOW                    0x0
#define HIGH                   0x1

#define setInput(ddr, pin)     (ddr &= ~(1 << pin))
#define setOutput(ddr, pin)    (ddr |= (1 << pin))
#define writeHigh(port, pin)   (port |= (1 << pin))
#define writeLow(port, pin)    (port &= ~(1 << pin))
#define checkPin(pinreg, pin)  ((pinreg & (1 << pin)) != 0)

#ifdef __cplusplus
extern "C" {
#endif
extern const uint8_t bits[];

void setPin(const uint8_t pin, const uint8_t type) __attribute__((cold)); // only used when starting the microcontroller
bool readPin(const uint8_t pin) __attribute__((hot));                     // frequenty used function in the main program
void set_value_pin(const uint8_t pin, bool val);
void analoginitial(void) __attribute__((cold));
void analogWrite(const uint8_t) __attribute__((hot));
uint16_t analogRead(const uint8_t pin);
#ifdef __cplusplus
}
#endif

#endif