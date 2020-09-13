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

bool enableObserveEC = false;
bool enableInputWater = false;

void setup() {
  pinMode(PIN_EC_INPUT, INPUT);
  pinMode(PIN_EC_POWER, OUTPUT); 
  pinMode(PIN_EC_GROUND, OUTPUT); 
  pinMode(PIN_INPUT_WATER, OUTPUT);
  pinMode(PIN_REVERT_WATER, OUTPUT);
  pinMode(PIN_INPUT_FERTILIZER, OUTPUT);
  
  digitalWrite(PIN_EC_POWER, LOW);
  digitalWrite(PIN_EC_GROUND, LOW);
  digitalWrite(PIN_INPUT_WATER, LOW); 
  digitalWrite(PIN_REVERT_WATER, LOW);
  digitalWrite(PIN_INPUT_FERTILIZER, LOW);
  
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
    
  if (command == 'I') {
    mySerial.println("I -> Input water: ON");
    digitalWrite(PIN_INPUT_WATER, HIGH);
    digitalWrite(PIN_INPUT_FERTILIZER, LOW);
    enableInputWater = true;
  } else if (command == 'i') {
    mySerial.println("i -> Input water: OFF");
    digitalWrite(PIN_INPUT_WATER, LOW);
    enableInputWater = false;
  } else if (command == 'R') {
    mySerial.println("R -> Revert water: ON");
    digitalWrite(PIN_REVERT_WATER, HIGH);
  } else if (command == 'r') {
    mySerial.println("r -> Revert water: OFF");
    digitalWrite(PIN_REVERT_WATER, LOW);
  } else if (command == 'O') {
    mySerial.println("O -> Observe EC: ON");
    enableObserveEC = true;
  } else if (command == 'o') {
    mySerial.println("o -> Observe EC: OFF");
    enableObserveEC = false;
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

  if (result.ec25 < 0.7 && digitalRead(PIN_INPUT_FERTILIZER) == LOW) {
    mySerial.println("---> Input fertilizer: ON");
    digitalWrite(PIN_INPUT_FERTILIZER, HIGH);
  } else if (result.ec25 >= 0.7 && digitalRead(PIN_INPUT_FERTILIZER) == HIGH) {
    mySerial.println("---> Input fertilizer: OFF");
    digitalWrite(PIN_INPUT_FERTILIZER, LOW);
    if (enableInputWater)
      digitalWrite(PIN_INPUT_WATER, HIGH);
  } 
}

void loop() {
  receiveCommandFromBTSerial();
  if (enableObserveEC) observeECForInputFertilizer();
}
