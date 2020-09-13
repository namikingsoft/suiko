#include "BlockDuringMillis.h"
#include "Arduino.h"

BlockDuringMillis::BlockDuringMillis(int duringMillis) {
  this->duringMillis = duringMillis;
  this->prevMillis = - duringMillis;
}

bool BlockDuringMillis::isBlock() {
  int msec = millis();
  if (msec - this->prevMillis < this->duringMillis) {
    if (msec - this->prevMillis < 0) // For millis overflow
      this->prevMillis = msec;
    return true;
  }
  this->prevMillis = msec;
  return false;
}