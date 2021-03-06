#ifndef __Suiko_h__
#define __Suiko_h__

#include "Arduino.h"

#define SERIAL_BPS 9600
#define PIN_EC_INPUT A0
#define PIN_EC_POWER A4
#define PIN_PH_INPUT A5
#define PIN_CYCLE_WATER 2
#define PIN_INPUT_WATER 3
#define PIN_AIR_PUMP_PWM 5
#define PIN_BLUETOOTH_RXD 10
#define PIN_BLUETOOTH_TXD 11
#define PIN_ONE_WIRE 12

#define BLOCK_MEASURE_EC_DURING_MILLIS 5000
#define BLOCK_OBSERVE_EC_DURING_MILLIS 60000
#define AIR_PUMP_PWM_POWER 110

void setup();
void loop();

#endif /* __Suiko_h__ */
