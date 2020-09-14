#include "MySerial.h"
#include "Arduino.h"

MySerial::MySerial(SoftwareSerial* serial) {
  this->serial = serial;
}

void MySerial::begin(int bps) {
  Serial.begin(bps);
  this->serial->begin(bps);
}

void MySerial::print(char* str) {
  Serial.print(str);
  this->serial->print(str);
}

void MySerial::print(float num) {
  Serial.print(num);
  this->serial->print(num);
}

void MySerial::println(char* str) {
  Serial.println(str);
  this->serial->println(str);
}

char MySerial::read() {
  return Serial.available() > 0 ? Serial.read() : this->serial->read();
}

int MySerial::available() {
  return Serial.available() + this->serial->available();
}
