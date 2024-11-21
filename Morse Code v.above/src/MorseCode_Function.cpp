#include "MorseCode_Function.h"
#include "Application_Function.h"

#define MAX_INPUT            32

//uint8_t NE555_INPUT::input_pin = NEE555_OC_INPUT_PIN; // not used
//volatile uint16_t NE555_INPUT::buzzSpeed = 0; // not used
//volatile bool NE555_INPUT::buzz_thread_on = false; // not used
//volatile bool NE555_INPUT::next_length = false; // not use

void buzz_sound(volatile uint8_t& port, const uint8_t pin, const uint16_t duration) {
  port = writeHigh(port, pin);
  delay_ms(duration);
  port = writeLow(port, pin);
}

MCL0::MCL0(uint8_t singlePin, uint8_t twoPin, uint8_t enterPin, uint8_t buzzPin) {           // setups the class, recommended used once
  setPin(singlePin, INPUT_PULLUP); // sets up the 1 value button when pressed
  setPin(twoPin,    INPUT_PULLUP); // sets up the 2 value button when pressed
  setPin(enterPin,  INPUT_PULLUP); // sets up enter button to save input
  setPin(buzzPin,   OUTPUT);       // sets up buzzer
  pin.singlePin = singlePin;
  pin.twoPin = twoPin;
  pin.enterPin = enterPin;
  pin.buzzPin = buzzPin;
  ////////// DO NOT REMOVE //////////
  setPin(13, OUTPUT);
  PORTB = writeLow(PORTB, PORTB5);
  ///////////////////////////////////
  ResetData();
  pressedBuzz = false;
  holdingShortcut = false;
}

void MCL0::setBuzzSpeed(void) {
  // needs to be worked on
}

void MCL0::BuzzSound(uint8_t pin, const int16_t duration) const {
  struct PortMapping {
    volatile uint8_t &port;
    uint8_t offset;
  };
  
  static const PortMapping mappings[] = {
    {PORTE, 4},       // pin 2
    {PORTE, 5},       // pin 3
    {PORTG, PORTG5},  // pin 4
    {PORTE, PORTE3},  // pin 5
    {PORTH, 3},       // pin 6
    {PORTH, 4},       // pin 7
    {PORTH, 5},       // pin 8
    {PORTH, 6},       // pin 9
    {PORTB, 4},       // pin 10
    {PORTB, 5},       // pin 11
    {PORTB, 6},       // pin 12
    {PORTL, PORTL5},  // pin 44
    {PORTL, PORTL4},  // pin 45
    {PORTL, PORTL3}   // pin 46
  };

  if (pin >= 2 && pin <= 12) {
    buzz_sound(mappings[pin - 2].port, mappings[pin - 2].offset, duration);
  } else if (pin >= 44 && pin <= 46) {
    buzz_sound(mappings[pin - 32].port, mappings[pin - 32].offset, duration);
  }
}

void MCL0::PressedBuzzSound(const uint8_t pin, const uint8_t value) const { // could be changed
  if (value == 1) {
    BuzzSound(pin, DURATION);
  } else if (value == 2) {
    BuzzSound(pin, DURATION << 1);
  }
}

void MCL0::BuzzSoundDecode(const char* arr2) {
  for (uint8_t i = 0; i < LIBRARY_SIZE; i++) { // Iterate through each value in the library
     for (uint8_t j = 0; j < SUBSIZE; j++) { // Iterate through each sub-value
      if (*arr2 == msLibrary[j]) { // If a match is found, call GetBuzzValue with the corresponding mLibrary value
        GetBuzzValue(mLibrary[j]);
        break;
      }
    }
  }
  pressedBuzz = false; // Set pressedBuzz to false after processing
}

void MCL0::GetBuzzValue(const uint8_t* indexArr) const {
  for (uint8_t i = 0; i < SUBSIZE; i++) {
    if (*indexArr == 1) { // If the value is 1, play short buzz sound
      PressedBuzzSound(pin.buzzPin, 1);
    } else if (*indexArr == 2) { // If the value is 2, play long buzz sound
      PressedBuzzSound(pin.buzzPin, 2); 
    }
    //delay_ms(NE555_INPUT::buzzSpeed); // not needed
  }
  _delay_ms(150);
}

void MCL0::LoadCurrentLocation(const char* indexArr) {
  currentLocation = 0;
  LCD.clear();
  LCD.setCursor(0, 0);
  while (indexArr[currentLocation] != '\0') {
    LCD.print(indexArr[currentLocation]);
    currentLocation++;
  }
  _delay_ms(250);
}

void MCL0::SelectInputDot(char* in) {
  const bool holdingTwoPin = (readPin(pin.twoPin) == LOW);
  const bool holdingSinglePin = (readPin(pin.singlePin) == LOW);
  const bool holdingEnterPin = (readPin(pin.enterPin) == LOW);

  if (_pressed_1 == 0 && !holdingShortcut) {
    if (holdingTwoPin) {
      _pressed_1 = pin.twoPin;
    } else if (holdingSinglePin) {
      _pressed_1 = pin.singlePin;
    } else if (holdingEnterPin || dataSize >= DATA_INPUT_SIZE) {
      in[currentLocation] = (dataSize != 0) ? Decode() : '\0';
      if (in[currentLocation] != '\0') {
        currentLocation++;
      }

      ResetData();
    }
  } else if (!holdingTwoPin && _pressed_1 == pin.twoPin) {
    _dataInput[dataSize++] = 2;
    _pressed_1 = 0;
  } else if (!holdingSinglePin && _pressed_1 == pin.singlePin) {
    _dataInput[dataSize++] = 1;
    _pressed_1 = 0;
  } else if (holdingTwoPin && _pressed_1 == pin.singlePin) {
    pressedBuzz = true;
    holdingShortcut = true;
    _pressed_1 = 0;
  } else if (holdingSinglePin && _pressed_1 == pin.twoPin) {
    _pressed_1 = 0;
    ResetData();
    application_set.MainMenu();
  } else if (holdingEnterPin && _pressed_1 == pin.singlePin) {
    in[currentLocation] = ' ';
    currentLocation++;
    _pressed_1 = 0;
    ResetData();
    _delay_ms(200);
  } else if (holdingEnterPin && _pressed_1 == pin.twoPin) {
    ResetData();
    if (currentLocation > 0) {
      in[currentLocation - 1] = '\0';
      currentLocation--;
      _pressed_1 = 0;
      mcl0.LoadCurrentLocation(application_set.morseCode[application_set.currentSelected - 1]);
      _delay_ms(200);
    }
  }
  else if (!holdingSinglePin && !holdingTwoPin && !holdingEnterPin) {
    holdingShortcut = false;
  }
}

bool MCL0::SelectionInputChange(uint8_t& currentLine, uint8_t column_size) { // improve soon>>>>>>
  const bool holdingTwoPin = (readPin(pin.twoPin) == LOW);
  const bool holdingSinglePin = (readPin(pin.singlePin) == LOW);
  const bool holdingEnterPin = (readPin(pin.enterPin) == LOW);
  static bool holding = false;

  if (!holding) {
    if (holdingTwoPin) {
      holding = true;
      if (currentLine + 1 < column_size) {                                         // moves current column down
        currentLine++;
        return true;
      }
      return false;
    } else if (holdingSinglePin) {
      holding = true;
      if (0 < currentLine) {
        currentLine--;
        return true;
      }
      return false;
    } else if (holdingEnterPin) {
      currentLine += PRESSED;                                                      // starts the line for the choosen column
      holding = false;                                                             // preset for next call
      return true;                                                                 // no delay??? (new)
    } 
  } else if (!holdingTwoPin && !holdingSinglePin && !holdingEnterPin) {
    holding = false;
  }
  return false;
}

void MCL0::ResetData(void) {   // Resets data for the next decode
  for (int i = 0; i < DATA_INPUT_SIZE; i++) {  // Reset each element to 0
    _dataInput[i] = 0;
  }
  dataSize = 0; // Reset the data size
}

char MCL0::CheckColumns(const uint8_t extendedValue) const {
  for (uint8_t subValue = 0; subValue < SUBSIZE; subValue++) { // Iterate through each sub-value
    // Check if the current element matches the input data
    if (mLibrary[extendedValue][subValue] != _dataInput[subValue]) {
      return '\0'; // Return '\0' if there's no match
    }
  }
  return msLibrary[extendedValue]; // Return the corresponding value from msLibrary if all elements match
}

char MCL0::Decode(void) const {
  for (uint8_t i = 0; i < LIBRARY_SIZE; i++) {
    char temp = CheckColumns(i); // Check columns for the current value
    if (temp != '\0') { // If a match is found, return the corresponding character
      return temp;
    }
  }
  return '\0'; // Return null character if no match is found
}