#include "Suiko.h"
#include "ECMeter.h"
#include "MySerial.h"
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
  char command = mySerial.read();
    
  if (command == 'C') {
    mySerial.println("C -> Cycle water: ON");
    digitalWrite(PIN_CYCLE_WATER, HIGH);
    digitalWrite(PIN_INPUT_WATER, LOW);
    enableCycleWater = true;
  } else if (command == 'c') {
    mySerial.println("c -> Cycle water: OFF");
    digitalWrite(PIN_CYCLE_WATER, LOW);
    enableCycleWater = false;
  } else if (command == 'I') {
    mySerial.println("I -> Input water: ON");
    digitalWrite(PIN_INPUT_WATER, HIGH);
    digitalWrite(PIN_CYCLE_WATER, LOW);
  } else if (command == 'i') {
    mySerial.println("i -> Input water: OFF");
    digitalWrite(PIN_INPUT_WATER, LOW);
  } else if (command == 'a') {
    mySerial.println("a -> Observe mode: OFF");
    observeMode = OFF;
  } else if (command == 's') {
    mySerial.println("s -> Observe mode: INPUT WATER");
    observeMode = INPUT_WATER;
  } else if (command == 'd') {
    mySerial.println("d -> Observe mode: INPUT FERTILIZER");
    observeMode = INPUT_FERTILIZER;
  } else if (command == 'm') {
    // Wait few seconds prevent breaking sensors
    if (blockMeasureEC.isBlock()) return;

    ECResult result = ecMeter.measure();
    mySerial.print("m -> ");
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
