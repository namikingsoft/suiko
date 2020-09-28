#include "PHMeter.h"
#include "Arduino.h"

#define V2PH_SLOPE -5.81261548798925
#define V2PH_INTERCEPT 21.9317612968251
#define BUFFUR_LENGTH 10

PHMeter::PHMeter(int const pinAnalogIn) {
  this->pinAnalogIn = pinAnalogIn;
}
 
float PHMeter::measure() {
  int sensorValue = 0;
  int buf[BUFFUR_LENGTH], temp;

  for (int i=0; i<BUFFUR_LENGTH; i++) {
    buf[i] = analogRead(this->pinAnalogIn);
    delay(10);
  }
  
  // bubble sort
  for (int i=0; i<BUFFUR_LENGTH-1; i++) {
    for (int j=i+1; j<BUFFUR_LENGTH; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  unsigned long int avgValue = 0;
  for (int i=2; i<BUFFUR_LENGTH-2; i++) avgValue += buf[i];
  
  float vol = (float)avgValue * 5.0 / 1024 / 6;
  return V2PH_SLOPE * vol + V2PH_INTERCEPT;
}
