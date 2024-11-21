#ifndef __Application_Function_File__
#define __Application_Function_File__

#include "C_LCD.h"
#include "MorseCode_Function.h"

#define DATA_PIN                 2
#define LATCH_PIN                3
#define CLOCK_PIN                4
#define BUZZ_PIN                 5
#define SINGLEDOT_PIN           44
#define CONNECTDOT_PIN          45
#define ENTERDOT_PIN            46

#define BLINK                  500       // 0.250 * 2 milliseconds

struct Application_Set final {
  void StartProgram(void) __attribute__((cold));
  void MainMenu(void);

  #define IS_BLINK() ({ \
    uint16_t temp = __HM::m_time + 1000; \
    bool result; \
    if (temp < BLINK) { \
        result = true; \
    } else { \
      while (temp >= blink_speed) { \
        temp -= blink_speed; \
      } \
      result = (temp <= BLINK); \
    } \
    result; \
  })

  #define is_blink_H() ({\
    uint16_t temp = __HM::m_time + 1250; \
    bool result = false; \
    if (temp < BLINK) { \
        result = true; \
    } else { \
      while (temp >= blink_speed) { \
        temp -= blink_speed; \
      } \
      result = (temp <= BLINK); \
    } \
    result; \
  })
  
  void RefreshScreen(void);
  void MainProgram(void) __attribute__((optimize("O3"), hot)); // could be removed, takes 1 k bytes more and 30 microseconds faster
  void LoadEEPROM_Option(void) __attribute((cold));
  bool up;
  uint8_t row; // x
  uint8_t column; // y

  char morseCode[5][32]; // 5 stored 32 is max characters for each currently

  uint8_t currentSelected;
  uint8_t currentLine;
  uint8_t lineDelay;
};

extern Application_Set application_set;

/*************************************************************************************************/
class __HM final {
  private:
  static uint16_t __mil;
  friend void setupTimer(void) __attribute__((cold)); 
  //friend uint16_t milli(void) __attribute__((hot, always_inline));
  friend inline void flag_mil_incr(void) __attribute__((optimize("O3"), hot));
  friend void flag_mil_incr(void) __attribute__((optimize("O3"), hot));
  public:
  static uint16_t m_time;
};
/*************************************************************************************************/

extern volatile bool flag;

constexpr uint16_t blink_ratio(void) {
  return static_cast<uint16_t>(BLINK * 2);
}

constexpr uint16_t blink_speed = blink_ratio();

extern void setupTimer(void) __attribute__((cold));  // only used when starting the microcontroller
//[[deprecated("Use newFunction() instead")]]
//extern inline uint16_t milli(void) __attribute__((hot, always_inline));
/***************************************************
*   \todo flag_mil_incr needs to be change / updated
***************************************************/
extern __attribute__((optimize("O3"), hot)) 
inline void flag_mil_incr(void) {
  if (flag) {
    __HM::__mil++;
    if (__HM::__mil == blink_speed) {
      __HM::__mil = 0;
    }
    __HM::m_time = __HM::__mil;
    flag = false;
  }
}
#endif