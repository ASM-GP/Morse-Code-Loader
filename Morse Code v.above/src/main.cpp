/*********************************************
* \author Alejandro Ramirez (ASM-GP)
* \date 2024.6.25
* \dir Updated 2024.9.20
* \dir Reupdated 2024.10.12
**********************************************/
//#include <HardwareSerial.h>
#include "Application_Function.h"
//#include <Arduino.h>
void setup(void) {
  //Serial.begin(9600);
  application_set.StartProgram();
}

void Scheduler(void) {
  //unsigned long startTime = micros(); // Record the start time
  //for (uint8_t i = 0; i < 255; i++) {
  //
  //  }
  application_set.MainProgram();
  //unsigned long endTime = micros(); // Record the end time
  //unsigned long duration = endTime - startTime; // Calculate the duration

  //Serial.print("Execution time: ");
  //Serial.print(duration);
  //Serial.println(" microseconds");
}

int main(void) {
    setup();
    while (1) {
      Scheduler();
    }
    return 0;
}