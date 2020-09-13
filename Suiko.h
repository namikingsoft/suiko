#ifndef __Suiko_h__
#define __Suiko_h__

#include "Arduino.h"

#define PIN_EC_INPUT A0
#define PIN_EC_POWER A4
#define PIN_EC_GROUND A1
#define PIN_CYCLE_WATER 2
#define PIN_INPUT_FERTILIZER 3
#define PIN_BLUETOOTH_RXD 10
#define PIN_BLUETOOTH_TXD 11
#define PIN_ONE_WIRE 12

#define THRESHOLD_EC_FOR_INPUT_FERTILIZER 0.8

void setup();
void loop();

#endif /* __Suiko_h__ */
