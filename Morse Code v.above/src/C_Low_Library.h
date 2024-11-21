#ifndef __CPP_LOW_LIBRARY__
#define __CPP_LOW_LIBRARY__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>
#include "cusLib/pin_control.h"
#include "cusLib/compare_trait.h"
#include "Threading/pt.h"
//#include "Set_Pins.h"

/*********************************************************************************************
*
*
* \note ~ flips the bit(s), ex. 1101 -> ~(1101) -> 0011 + 1 = 0010
*
*
*********************************************************************************************/
#define setInput(ddr, pin)     (ddr &= ~(1 << pin))
#define setOutput(ddr, pin)    (ddr |= (1 << pin))
#define writeHigh(port, pin)   (port |= (1 << pin))
#define writeLow(port, pin)    (port &= ~(1 << pin))
#define checkPin(pinreg, pin)  ((pinreg & (1 << pin)) != 0)

#define BYTE_BIT_LENGTH          8

//template<typename TYPE_C> 
//using function_ptr = void(TYPE_C::*)();
typedef void(*f_ptr)();
typedef void* voidPtr;

extern __attribute__((section(".noinit"))) uint8_t initialized_start;

/***********************************************************************************************************************
*   \note always inline -> replaces function with the code itself
*   \note optimize("O#") -> fastest speed here, O0 - least optimization, 03 most optimization
*   \note hot -> tells the compiler the function is frequenty used, most aggressive optimization
************************************************************************************************************************/
void setTimer1(void) __attribute__((cold));                               // sets up timer one in MEGA 2560 (optional)
/***********************************************************************************************************************/
void delay_ms(uint16_t duration) noexcept; // commonly used, doesn't need optimization
#endif