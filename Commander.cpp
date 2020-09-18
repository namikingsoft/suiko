#include "Commander.h"

Command Commander::receive(char const ch) {
  CommandType type = COMMAND_NOT_FOUND;
  CommandPayload payload = { false };
  if (ch == CHAR_CYCLE_WATER_ON) {
    type = COMMAND_CYCLE_WATER;
    payload.is = true;
  } else if (ch == CHAR_CYCLE_WATER_OFF) {
    type = COMMAND_CYCLE_WATER;
  } else if (ch == CHAR_INPUT_WATER_ON) {
    type = COMMAND_INPUT_WATER;
    payload.is = true;
  } else if (ch == CHAR_INPUT_WATER_OFF) {
    type = COMMAND_INPUT_WATER;
  } else if (ch == CHAR_MEASURE_EC) {
    type = COMMAND_MEASURE_EC;
  } else if (ch == CHAR_OBSERVE_MODE_OFF) {
    type = COMMAND_OBSERVE_MODE_OFF;
  } else if (ch == CHAR_OBSERVE_MODE_ABOVE) {
    type = COMMAND_BUFFER;
    this->bufferType = COMMAND_OBSERVE_MODE_ABOVE;
  } else if (ch == CHAR_OBSERVE_MODE_BELOW) {
    type = COMMAND_BUFFER;
    this->bufferType = COMMAND_OBSERVE_MODE_BELOW;
  } else if (this->bufferType != COMMAND_NOT_FOUND && isDigit(ch) || ch == '.') {
    type = COMMAND_BUFFER;
    this->bufferPayload += ch;
    if (this->bufferPayload.length() >= 4) {
      payload.ec = this->bufferPayload.toFloat();
      if (payload.ec != 0)
        type = this->bufferType;
      else
        type = COMMAND_NOT_FOUND;
    }
  }

  if (type != COMMAND_BUFFER) {
    this->clearBuffer();
  }

  return { type, payload };
}

void Commander::clearBuffer() {
  this->bufferType = COMMAND_NOT_FOUND;
  this->bufferPayload = "";
}
