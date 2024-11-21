#include "pin_control.h"

const uint8_t bits[] = {
  [2] = 4, [3] = 5,
  [4] = PORTG5,
  [5] = PORTE3,
  [6] = 3, [7] = 4, [8] = 5, [9] = 6,
  [10] = 4, [11] = 5, [12] = 6,
  [44] = PORTL5,
  [45] = PORTL4,
  [46] = PORTL3
};

void setPin(const uint8_t pin, const uint8_t type) {
  volatile uint8_t* ddr;
  volatile uint8_t* port = NULL;
  uint8_t bit = 0;

  switch (pin) {
    case 2: case 3:
      ddr = &DDRE;
      bit = pin + 2;
      port = &PORTE;
      break;
    case 4:
      ddr = &DDRG;
      bit = DDG5;
      port = &PORTG;
      break;
    case 5:
      ddr = &DDRE;
      bit = DDE3;
      port = &PORTE;
      break;
    case 6: case 7: case 8: case 9:
      ddr = &DDRH;
      bit = pin - 3;
      port = &PORTH;
      break;
    case 10: case 11: case 12:
      ddr = &DDRB;
      bit = pin - 6;
      port = &PORTB;
      break;
    case 44:
      ddr = &DDRL;
      bit = DDL5;
      port = &PORTL;
      break;
    case 45:
      ddr = &DDRL;
      bit = DDL4;
      port = &PORTL;
      break;
    case 46:
      ddr = &DDRL;
      bit = DDL3;
      port = &PORTL;
      break;
    default:
      return; // Invalid pin, do nothing
  }

  if (type == OUTPUT) {
    *ddr = setOutput(*ddr, bit);
  } else if (type == INPUT || type == INPUT_PULLUP) {
    *ddr = setInput(*ddr, bit);
    if (type == INPUT_PULLUP && port != NULL) {
      *port = writeHigh(*port, bit);
    }
  }
}

bool readPin(const uint8_t pin) { // needs code in it, new>>>>>>>>> noexcept
  static volatile uint8_t* const registers[] = {
    [2] = &PINE, [3] = &PINE,
    [4] = &PING,
    [5] = &PINE,
    [6] = &PINH, [7] = &PINH, [8] = &PINH, [9] = &PINH,
    [10] = &PINB, [11] = &PINB, [12] = &PINB,
    [44] = &PINL,
    [45] = &PINL,
    [46] = &PINL
  };

  if (pin >= 2 && pin <= 12 || pin >= 44 && pin <= 46) {
    return checkPin(*registers[pin], bits[pin]);
  }
    
  return 0; // Default case, invalid pin
}

void set_value_pin(const uint8_t pin, bool val) {
  static volatile uint8_t* const ports[] = {
    [2] = &PORTE, [3] = &PORTE,
    [4] = &PORTG,
    [5] = &PORTE,
    [6] = &PORTH, [7] = &PORTH, [8] = &PORTH, [9] = &PORTH,
    [10] = &PORTB, [11] = &PORTB, [12] = &PORTB,
    [44] = &PORTL,
    [45] = &PORTL,
    [46] = &PORTL
  };

  if ((pin >= 2 && pin <= 12) || (pin >= 44 && pin <= 46)) {
    if (val) {
      *ports[pin] = writeHigh(*ports[pin], bits[pin]);
    } else {
      *ports[pin] = writeLow(*ports[pin], bits[pin]);
    }
  }
}

void analoginitial(void) {
  // Set the reference voltage to AVcc (5V)
  ADMUX = (1 << REFS0);

  // Enable the ADC and set the prescaler to 128
  // This gives an ADC clock of 16MHz/128 = 125kHz
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void analogWrite(const uint8_t pin) {
  
}

uint16_t analogRead(const uint8_t pin) {
    // Select the corresponding channel 0~15
    // ANDing with '0x07' to make sure we are only selecting the lower 3 bits
    if (pin < 0 || pin > 15) {return;}
    ADMUX = (ADMUX & 0xF8) | (pin & 0x07);

    // Start the conversion
    ADCSRA |= (1 << ADSC);

    // Wait for the conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Read the ADC value
    return ADC;
}