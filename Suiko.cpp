#include "Suiko.h"
#include "ECMeter.h"
#include "MySerial.h"
#include "Commander.h"
#include "BlockDuringMillis.h"

#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial btSerial(PIN_BLUETOOTH_RXD, PIN_BLUETOOTH_TXD);
MySerial mySerial(&btSerial);

OneWire oneWire(PIN_ONE_WIRE);
DallasTemperature sensors(&oneWire);
ECMeter ecMeter(&sensors, PIN_EC_INPUT, PIN_EC_POWER);

BlockDuringMillis blockMeasureEC(5000);
BlockDuringMillis blockObserveEC(30000);

Commander commander;
ObserveMode observeMode = OFF; 
bool enableCycleWater = false;

void setup() {
  pinMode(PIN_EC_INPUT, INPUT);
  pinMode(PIN_EC_POWER, OUTPUT); 
  pinMode(PIN_EC_GROUND, OUTPUT); 
  pinMode(PIN_CYCLE_WATER, OUTPUT);
  pinMode(PIN_INPUT_WATER, OUTPUT);
  
  digitalWrite(PIN_EC_POWER, LOW);
  digitalWrite(PIN_EC_GROUND, LOW);
  digitalWrite(PIN_CYCLE_WATER, LOW); 
  digitalWrite(PIN_INPUT_WATER, LOW);
  
  mySerial.begin(9600);
  mySerial.println("Conneted");
}

void printECResult(ECResult* result) {
  mySerial.print("EC: ");
  mySerial.print(result->ec25);
  mySerial.print(" Celsius: ");
  mySerial.print(result->temperature);
  mySerial.println(" *C");
}

void receiveCommandFromBTSerial() {
  if (mySerial.available() <= 0) return;
  char ch = mySerial.read();
  mySerial.write(ch); // echo mode
  
  Command cmd = commander.receive(ch);
  switch (cmd.type) {
  case COMMAND_CYCLE_WATER:
    mySerial.print(" -> Cycle water ");
    mySerial.println(cmd.payload.is ? "ON" : "OFF");
    digitalWrite(PIN_CYCLE_WATER, cmd.payload.is ? HIGH : LOW);
    if (cmd.payload.is) digitalWrite(PIN_INPUT_WATER, LOW);
    enableCycleWater = cmd.payload.is;
    break;
  case COMMAND_INPUT_WATER:
    mySerial.print(" -> Input water ");
    mySerial.println(cmd.payload.is ? "ON" : "OFF");
    digitalWrite(PIN_INPUT_WATER, cmd.payload.is ? HIGH : LOW);
    if (cmd.payload.is) digitalWrite(PIN_CYCLE_WATER, LOW);
    break;
  case COMMAND_OBSERVE_MODE_OFF:
    mySerial.println(" -> Observe mode: OFF");
    observeMode = OFF;
    break;
  case COMMAND_OBSERVE_MODE_ABOVE:
    mySerial.println(" -> Observe mode: INPUT WATER");
    observeMode = INPUT_WATER;
    break;
  case COMMAND_OBSERVE_MODE_BELOW:
    mySerial.println(" -> Observe mode: INPUT FERTILIZER");
    observeMode = INPUT_FERTILIZER;
    break;
  case COMMAND_MEASURE_EC:
    // Wait few seconds prevent breaking sensors
    if (blockMeasureEC.isBlock()) return;

    ECResult result = ecMeter.measure();
    mySerial.print(" -> ");
    printECResult(&result);
  }
}

void observeECForInputFertilizer() {
  // Wait few seconds prevent breaking sensors
  if (blockObserveEC.isBlock()) return;
  
  ECResult result = ecMeter.measure();
  mySerial.print("---> ");
  printECResult(&result);

  if (
    (observeMode == INPUT_WATER && result.ec25 > THRESHOLD_EC_FOR_INPUT_WATER) ||
    (observeMode == INPUT_FERTILIZER && result.ec25 < THRESHOLD_EC_FOR_INPUT_FERTILIZER)
  ) {
    if (digitalRead(PIN_INPUT_WATER) == HIGH)
      return;
    mySerial.println("---> Input water: ON");
    digitalWrite(PIN_INPUT_WATER, HIGH);
    digitalWrite(PIN_CYCLE_WATER, LOW);
  } else {
    if (digitalRead(PIN_INPUT_WATER) == LOW)
      return;
    mySerial.println("---> Input water: OFF");
    digitalWrite(PIN_INPUT_WATER, LOW);
    if (enableCycleWater)
      digitalWrite(PIN_CYCLE_WATER, HIGH);
  } 
}

void loop() {
  receiveCommandFromBTSerial();
  if (observeMode != OFF) observeECForInputFertilizer();
}
