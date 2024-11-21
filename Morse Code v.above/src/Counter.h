#ifndef EXTENSION_H
#define EXTENSION_H

#include <binary.h>
#include "C_Low_Library.h"

#define DEFUALT_LATCHPIN   3                        // connect to the ST_CP of 74HC595 (pin 3,latch pin)
#define DEFAULT_CLOCKPIN   4                        // connect to the SH_CP of 74HC595 (pin 4, clock pin)
#define DEFAULT_DATAPIN    2                        // connect to the DS of 74HC595 (pin 2)

typedef uint8_t byte;
namespace bit_shifter_use {
  typedef struct pin_74hc595 {
  uint8_t latchPin;
  uint8_t clockPin;
  uint8_t dataPin;
} pin_74hc595;

struct Clock_Count { 
  // define the LED digit patterns, from 0 - 9
  // 1 = LED on, 0 = LED off, in this order:
  //                74HC595 pin     Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7 
  //                Mapping to      a,b,c,d,e,f,g of Seven-Segment LED
  byte seven_seg_digits[10] = {
    B11111100,  // = 0
    B01100000,  // = 1
    B11011010,  // = 2
    B11110010,  // = 3
    B01100110,  // = 4
    B10110110,  // = 5
    B10111110,  // = 6
    B11100000,  // = 7
    B11111110,  // = 8
    B11100110   // = 9
  };
    
  Clock_Count(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin);

  void sevenSegWrite(byte digit);
  void turnOff(void) __attribute__((cold));

  private:
  pin_74hc595 pin_hc595;
};

struct LED_bar {
  LED_bar(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin);

  void setBar(uint8_t bar_length);
  private:
  pin_74hc595 pin_hc595;
};

void shift_out(pin_74hc595 pins, uint8_t val); // moderate
}

#endif