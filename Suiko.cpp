#include "Suiko.h"
#include "ECMeter.h"
#include "PHMeter.h"
#include "DualSerial.h"
#include "Commander.h"
#include "BlockDuringMillis.h"

#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define MSG_RETRY_AFTER_SECONDS" -> Retry after seconds"

SoftwareSerial swSerial(PIN_BLUETOOTH_RXD, PIN_BLUETOOTH_TXD);
DualSerial mySerial(&swSerial);

OneWire oneWire(PIN_ONE_WIRE);
DallasTemperature sensors(&oneWire);
ECMeter ecMeter(&sensors, PIN_EC_INPUT, PIN_EC_POWER);
PHMeter phMeter(PIN_PH_INPUT);

BlockDuringMillis blockMeasureEC(BLOCK_MEASURE_EC_DURING_MILLIS);
BlockDuringMillis blockObserveEC(BLOCK_OBSERVE_EC_DURING_MILLIS);

Commander commander;
Command observeCommand = { COMMAND_OBSERVE_MODE_OFF, { false } }; 
bool enableCycleWater = false;

void setup() {
  pinMode(PIN_EC_INPUT, INPUT);
  pinMode(PIN_PH_INPUT, INPUT);
  pinMode(PIN_EC_POWER, OUTPUT); 
  pinMode(PIN_CYCLE_WATER, OUTPUT);
  pinMode(PIN_INPUT_WATER, OUTPUT);
  pinMode(PIN_AIR_PUMP_PWM, OUTPUT);

  digitalWrite(PIN_EC_POWER, LOW);
  digitalWrite(PIN_CYCLE_WATER, LOW);
  digitalWrite(PIN_INPUT_WATER, LOW);
  analogWrite(PIN_AIR_PUMP_PWM, 0);
  
  mySerial.begin(SERIAL_BPS);
  mySerial.println("Connected");
}

void printMeasureResult(ECResult* const result, float const ph) {
  mySerial.print(result->ec25);
  mySerial.print("EC ");
  mySerial.print(ph);
  mySerial.print("pH ");
  mySerial.print(result->temperature);
  mySerial.println("*C");
}

void receiveCommandFromBTSerial() {
  if (mySerial.available() <= 0) return;
  char const ch = mySerial.read();

  Command const cmd = commander.receive(ch);
  if (cmd.type != COMMAND_NOT_FOUND) {
    mySerial.write(ch); // echo mode
  }
  
  if (cmd.type == COMMAND_CYCLE_WATER) {
    mySerial.print(" -> Cycle water ");
    mySerial.println(cmd.payload.is ? "ON" : "OFF");
    digitalWrite(PIN_CYCLE_WATER, cmd.payload.is ? HIGH : LOW);
    if (cmd.payload.is) digitalWrite(PIN_INPUT_WATER, LOW);
    enableCycleWater = cmd.payload.is;
  } else if (cmd.type == COMMAND_INPUT_WATER) {
    mySerial.print(" -> Input water ");
    mySerial.println(cmd.payload.is ? "ON" : "OFF");
    digitalWrite(PIN_INPUT_WATER, cmd.payload.is ? HIGH : LOW);
    if (cmd.payload.is) digitalWrite(PIN_CYCLE_WATER, LOW);
  } else if (cmd.type == COMMAND_AIR_PUMP) {
    mySerial.print(" -> Air pump ");
    mySerial.println(cmd.payload.is ? "ON" : "OFF");
    analogWrite(PIN_AIR_PUMP_PWM, 200); // force push air for prevent to stop motor
    delay(500); // wait for motor rolling
    analogWrite(PIN_AIR_PUMP_PWM, cmd.payload.is ? AIR_PUMP_PWM_POWER : 0);
  } else if (cmd.type == COMMAND_MEASURE_EC) {
    // Wait few seconds prevent breaking sensors
    if (blockMeasureEC.isBlock()) {
      mySerial.println(MSG_RETRY_AFTER_SECONDS);
      return;
    } else if (observeCommand.type != COMMAND_OBSERVE_MODE_OFF) {
      mySerial.println(" -> Turn off observe mode");
      return;
    }
    ECResult const result = ecMeter.measure();
    float const ph = phMeter.measure();
    mySerial.print(" -> ");
    printMeasureResult(&result, ph);
  } else if (cmd.type == COMMAND_OBSERVE_MODE_OFF) {
    mySerial.println(" -> Observe mode: OFF");
    observeCommand = cmd;
    blockMeasureEC.resetPrevMillis();
  } else if (cmd.type == COMMAND_OBSERVE_MODE_ABOVE || cmd.type == COMMAND_OBSERVE_MODE_BELOW) {
    // Wait few seconds prevent breaking sensors
    if (blockMeasureEC.isBlock()) {
      mySerial.println(MSG_RETRY_AFTER_SECONDS);
      return;
    }
    mySerial.print(" -> Observe mode: IF EC");
    mySerial.print(cmd.type == COMMAND_OBSERVE_MODE_ABOVE ? ">" : "<");
    mySerial.print(cmd.payload.ec);
    mySerial.println(" THEN INPUT WATER");
    observeCommand = cmd;
  }
}

void observeForInputWater() {
  // Wait few seconds prevent breaking sensors
  if (blockObserveEC.isBlock()) return;

  // set to default
  blockObserveEC.setDuringMillis(BLOCK_OBSERVE_EC_DURING_MILLIS);
  
  ECResult const result = ecMeter.measure();
  float const ph = phMeter.measure();
  mySerial.print("---> ");
  printMeasureResult(&result, ph);

  if (
    (observeCommand.type == COMMAND_OBSERVE_MODE_ABOVE && result.ec25 > observeCommand.payload.ec) ||
    (observeCommand.type == COMMAND_OBSERVE_MODE_BELOW && result.ec25 < observeCommand.payload.ec)
  ) {
    // speed up observation
    blockObserveEC.setDuringMillis(BLOCK_MEASURE_EC_DURING_MILLIS);
    
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
  if (
    observeCommand.type == COMMAND_OBSERVE_MODE_ABOVE ||
    observeCommand.type == COMMAND_OBSERVE_MODE_BELOW
  ) observeForInputWater();
}
