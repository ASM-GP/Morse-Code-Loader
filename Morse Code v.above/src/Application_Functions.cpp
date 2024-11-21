#include "Application_Function.h"

MCL0 mcl0(SINGLEDOT_PIN, CONNECTDOT_PIN, ENTERDOT_PIN, BUZZ_PIN);

//Clock_Count clock_count(LATCH_PIN, CLOCK_PIN, DATA_PIN);
bit_shifter_use::LED_bar led_bar(LATCH_PIN, CLOCK_PIN, DATA_PIN);

LCD_C LCD(7, 8, 9, 10, 11, 12);

Application_Set application_set;

volatile bool flag = false;

uint16_t __HM::__mil = 0;

uint16_t __HM::m_time = 0;

// Timer1 overflow interrupt service routine
ISR(TIMER1_COMPA_vect) {
  flag = true;     // Increment the elapsed time (assuming a 1ms timer period)
}

void Application_Set::LoadEEPROM_Option(void) {
  if (initialized_start != 0xFF) {
    initialized_start = 0xFF;
    LCD.setCursor(0, 0);
    LCD.print("Load admin");
    LCD.setCursor(0, 1);
    LCD.print("settings? Y/N");
    while (true) {
      if (!readPin(SINGLEDOT_PIN)) {
         break;
      }
      if (!readPin(CONNECTDOT_PIN)) {
        return;
      }
    }
  }

  RefreshScreen();
}

void Application_Set::StartProgram(void) {
  LCD.initialize();
  setTimer1();
  for (int i = 0; i < 5; i++) { // needs to be updated
    for (int j = 0; j < 32; j++) {
      morseCode[i][j] = '\0';
    }
  }
  
  LCD.clear();
  LCD.print("Welcome");
  currentSelected = 0;
  currentLine = 0;
  //clock_count.turnOff();
  led_bar.setBar(0); // off
  _delay_ms(1000);
  up = true;
  LoadEEPROM_Option();
}

void Application_Set::MainMenu(void) {
  LCD.clear();
  currentSelected = 0;
  currentLine = 0;
  //lock_count.turnOff();
  led_bar.setBar(0); // off
  up = true;
  RefreshScreen();
}

void Application_Set::RefreshScreen(void) { // needs to be finished>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if (currentLine >= PRESSED) {return;}
  static const char letter[COLUMN_SIZE] = {'A', 'B', 'C', 'D', 'E'};

  LCD.clear();

  LCD.setCursor(15, (up) ? 0 : 1);
  LCD.print("<");
  LCD.setCursor(15, (!up) ? 0 : 1);
  LCD.print(" ");
  if (up) {
    if (currentLine == 0) {
      LCD.setCursor(0, 0);
      LCD.print("Select");
    }
    LCD.setCursor(14, 0);
    LCD.print(letter[currentLine]);
    LCD.setCursor(14, 1);
    LCD.print(letter[currentLine + 1]);
  }
  else {
    if (currentLine - 1 == 0) {
      LCD.setCursor(0, 0);                    // Sets the cursor to column 0, line 0 (1), (x, y)
      LCD.print("Select");
    }
    LCD.setCursor(14, 0);
    LCD.print(letter[currentLine - 1]);
    LCD.setCursor(14, 1);
    LCD.print(letter[currentLine]);
  }
}

void Application_Set::MainProgram() {
  flag_mil_incr(); // remember to adjust the rest of the code>>>>>>>>>>>>>>>>>>>>>>>
  if (currentSelected == 0) {
    if (IS_BLINK()) {
      if (currentLine < COLUMN_SIZE - 2 || currentLine < COLUMN_SIZE - 1 && !up) {
        LCD.setCursor(0, 1);
        LCD.print("v");
      }
      if (currentLine > 0 && !(currentLine == 1 && !up)) {
        LCD.setCursor(1, 0);
        LCD.print("^");
      }
    }
    else {
      if (currentLine < COLUMN_SIZE - 2 || currentLine < COLUMN_SIZE - 1 && !up) {
        LCD.setCursor(0, 1);
        LCD.print(" ");
      }
      if (is_blink_H() && currentLine > 0 && !(currentLine == 1 && !up)) {
        LCD.setCursor(1, 0);
        LCD.print(" ");
      }
    }

    uint8_t temp = currentLine;
    if (mcl0.SelectionInputChange(currentLine)) {
      up = (temp > currentLine || currentLine == 0);
      RefreshScreen();
    }

    if (currentLine >= PRESSED) {
      currentSelected = (currentLine % PRESSED) + 1;
      mcl0.LoadCurrentLocation(morseCode[currentSelected - 1]);
    }
  }
  else {
    uint8_t cS = currentSelected - 1;

    //clock_count.sevenSegWrite(mcl0.dataSize);
    led_bar.setBar(mcl0.dataSize);

    ////////// Blinking effect ///////////
    LCD.setCursor((mcl0.currentLocation + 16) % 16, mcl0.currentLocation / 16);
    LCD.print(IS_BLINK() ? " " : "_");
    //////////////////////////////////////
    uint8_t current = mcl0.currentLocation;
    mcl0.SelectInputDot(morseCode[cS]);
    if (current != mcl0.currentLocation) {
      LCD.setCursor((mcl0.currentLocation + 15) % 16, (mcl0.currentLocation - 1) / 16);
      LCD.print(&morseCode[cS][mcl0.currentLocation - 1]);
      
      if (mcl0.currentLocation >= ROW_COLUMN_SIZE) { // 32
        MainMenu();
      } else if (mcl0.pressedBuzz) {
        mcl0.BuzzSoundDecode(morseCode[cS]);
      }
    }
  }
}