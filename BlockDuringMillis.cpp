#include "BlockDuringMillis.h"
#include "Arduino.h"

BlockDuringMillis::BlockDuringMillis(unsigned long const duringMillis) {
  this->duringMillis = duringMillis;
  this->prevMillis = 0;
}

bool BlockDuringMillis::isBlock() {
  const unsigned long msec = millis();
  if (msec - this->prevMillis < this->duringMillis) {
    if (msec - this->prevMillis < 0) // For millis overflow
      this->prevMillis = msec;
    return true;
  }
  this->prevMillis = msec;
  return false;
}

void BlockDuringMillis::setDuringMillis(unsigned long const duringMillis) {
  this->duringMillis = duringMillis;
}

void BlockDuringMillis::resetPrevMillis() {
   this->prevMillis = millis();
}
