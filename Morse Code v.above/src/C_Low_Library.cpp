#include "C_Low_Library.h"

uint8_t initialized_start __attribute__((section(".noinit")));

void setTimer1(void) {
  cli();
  //Set Timer1 to interrupt every milisecond at 249
  TCCR1A = 0; // Clear Timer1 control register A
  TCCR1B = 0; // Clear Timer1 control register B
  TCNT1 = 0; // Initialize counter

  OCR1A = 499; // 16MHz / (64 * (249 + 1)) = 1kHz (1ms timer period, 249)
  TCCR1B |= (1 << WGM12); // Turn on CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10); // set presacular to 8
  TIMSK1 |= (1 << OCIE1A); // enable timer1 overflow interupt
  sei(); // enable global interupts
}

void delay_ms(uint16_t duration) noexcept {
  while (duration--) {
    _delay_ms(1);
  }
}