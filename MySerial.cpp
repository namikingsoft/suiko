#include "MySerial.h"
#include "Arduino.h"

MySerial::MySerial(SoftwareSerial* serial) {
  this->serial = serial;
}

void MySerial::begin(int const bps) {
  Serial.begin(bps);
  this->serial->begin(bps);
}

void MySerial::print(char* const str) {
  Serial.print(str);
  this->serial->print(str);
}

void MySerial::print(float const num) {
  Serial.print(num);
  this->serial->print(num);
}

void MySerial::println(char* const str) {
  Serial.println(str);
  this->serial->println(str);
}

void MySerial::write(char const ch) {
  Serial.write(ch);
  this->serial->write(ch);
}

char MySerial::read() {
  return Serial.available() > 0 ? Serial.read() : this->serial->read();
}

int MySerial::available() {
  return Serial.available() + this->serial->available();
}
