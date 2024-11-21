#include "C_LCD.h"

LCD_C::LCD_C(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
  : rs(rs), enable(enable), d4(d4), d5(d5), d6(d6), d7(d7) {}

void LCD_C::initialize(void) {
  setPin(rs, OUTPUT);
  setPin(enable, OUTPUT);
  setPin(d4, OUTPUT);
  setPin(d5, OUTPUT);
  setPin(d6, OUTPUT);
  setPin(d7, OUTPUT);

  _delay_ms(50); // wait for LCD to power up
  write4bits(0x03);
  _delay_ms(5);
  write4bits(0x03);
  _delay_us(150);
  write4bits(0x03);
  write4bits(0x02);

  sendCommand(0x28);
  sendCommand(0x0C);
  sendCommand(0x06);
  sendCommand(0x01);
  
  _delay_ms(5);

  clear();
}

void LCD_C::print(const char text) {
  sendData(text);
}

void LCD_C::print(const char* text) {
  while (*text) {
    sendData(*text++);
  }
}

void LCD_C::setCursor(uint8_t col, uint8_t row) {
  static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if (row > 1) row = 1;
  sendCommand(0x80 | (col + row_offsets[row])); // 1000 0000 | 1-16 + row_offsets
}

void LCD_C::clear(void) {
  sendCommand(0x01); // Clear display command
  _delay_ms(2);
  sendCommand(0x01); // Send clear command again
  _delay_ms(2);
}

void LCD_C::send(uint8_t sendCommand, bool data) { // uses ASCII
  set_value_pin(rs, data);                            // high or low
  write4bits(sendCommand >> 4);
  write4bits(sendCommand);
}

void LCD_C::sendCommand(uint8_t sendCommand) {
  send(sendCommand, false);
}

void LCD_C::sendData(uint8_t data) {
  send(data, true);
}

void LCD_C::write4bits(uint8_t value) {
  set_value_pin(d4, (value >> 0) & 0x01);
  set_value_pin(d5, (value >> 1) & 0x01);
  set_value_pin(d6, (value >> 2) & 0x01);
  set_value_pin(d7, (value >> 3) & 0x01);
  pulseEnable();
}

void LCD_C::pulseEnable() {
  set_value_pin(enable, LOW);
  _delay_us(1); // Wait for 1000 microseconds (1 millisecond)
  set_value_pin(enable, HIGH);
  _delay_us(1);
  set_value_pin(enable, LOW);
  _delay_us(100);
}