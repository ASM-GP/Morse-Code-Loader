#ifndef __C_LED
#define __C_LED

#include <stdint.h>
#include <stdio.h>
#include "C_low_Library.h"

class LCD_C { // for 2 x 16 LCD model
  public:
  LCD_C(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  void initialize(void) __attribute__((cold));
  void print(const char text) __attribute__((hot));
  void print(const char* text) __attribute__((hot));
  void setCursor(uint8_t col, uint8_t row); 
  void clear(void);
  uint8_t currentSelected;
  uint8_t currentLine;
  private:
  uint8_t rs, enable, d4, d5, d6, d7;
  void send(uint8_t command, bool type) __attribute__((hot));
  void sendCommand(uint8_t command) __attribute__((hot));
  void sendData(uint8_t data);
  void write4bits(uint8_t value);
  void pulseEnable(void);
};

extern LCD_C LCD;
#endif