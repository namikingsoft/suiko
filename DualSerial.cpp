#include "DualSerial.h"
#include "Arduino.h"

DualSerial::DualSerial(SoftwareSerial* serial) {
  this->serial = serial;
}

void DualSerial::begin(int const bps) {
  Serial.begin(bps);
  this->serial->begin(bps);
}

void DualSerial::print(char* const str) {
  Serial.print(str);
  this->serial->print(str);
}

void DualSerial::print(float const num) {
  Serial.print(num);
  this->serial->print(num);
}

void DualSerial::println(char* const str) {
  Serial.println(str);
  this->serial->println(str);
}

void DualSerial::write(char const ch) {
  Serial.write(ch);
  this->serial->write(ch);
}

char DualSerial::read() {
  return Serial.available() > 0 ? Serial.read() : this->serial->read();
}

int DualSerial::available() {
  return Serial.available() + this->serial->available();
}
