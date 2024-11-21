#include "Counter.h"

namespace bit_shifter_use {
Clock_Count::Clock_Count(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin) {
  setPin(pin_hc595.latchPin = latchPin, OUTPUT);
  setPin(pin_hc595.clockPin = clockPin, OUTPUT);
  setPin(pin_hc595.dataPin  = dataPin,  OUTPUT);
}

void Clock_Count::turnOff(void) {
  set_value_pin(pin_hc595.latchPin, LOW);
  shift_out(pin_hc595, 0);  
  set_value_pin(pin_hc595.latchPin, HIGH); // no function for this>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
// display a number on the digital segment display
void Clock_Count::sevenSegWrite(byte digit) {
  // set the latchPin to low potential, before sending data
  set_value_pin(pin_hc595.latchPin, LOW);
     
  // the original data (bit pattern)
  shift_out(pin_hc595, seven_seg_digits[digit]);  
  // set the latchPin to high potential, after sending data
  set_value_pin(pin_hc595.latchPin, HIGH);
}

LED_bar::LED_bar(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin) {
  setPin(pin_hc595.latchPin = latchPin, OUTPUT);
  setPin(pin_hc595.clockPin = clockPin, OUTPUT);
  setPin(pin_hc595.dataPin  = dataPin,  OUTPUT);
}

void LED_bar::setBar(uint8_t bar_length) {
  // set the latchPin to low potential, before sending data
  set_value_pin(pin_hc595.latchPin, LOW);
     
  for (uint8_t i = 0; i < BYTE_BIT_LENGTH; i++) {
    set_value_pin(pin_hc595.dataPin, (bar_length > 0));
    set_value_pin(pin_hc595.clockPin, HIGH);
		set_value_pin(pin_hc595.clockPin, LOW);
    if (bar_length > 0) bar_length--;
  }
  // set the latchPin to high potential, after sending data
  set_value_pin(pin_hc595.latchPin, HIGH);
}

void shift_out(pin_74hc595 pins, uint8_t val) {
  for (uint8_t i = 0; i < BYTE_BIT_LENGTH; i++) {
    set_value_pin(pins.dataPin, val & 1);
    val >>= 1;
    set_value_pin(pins.clockPin, HIGH);
		set_value_pin(pins.clockPin, LOW);
  }
}
}