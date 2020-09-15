#include "Commander.h"

Command Commander::receive(char ch) {
  CommandType type = COMMAND_NOT_FOUND;
  CommandPayload payload = { false };
  if (ch == 'C') {
    type = COMMAND_CYCLE_WATER;
    payload.is = true;
  } else if (ch == 'c') {
    type = COMMAND_CYCLE_WATER;
  } else if (ch == 'I') {
    type = COMMAND_INPUT_WATER;
    payload.is = true;
  } else if (ch == 'i') {
    type = COMMAND_INPUT_WATER;
  } else if (ch == 'm') {
    type = COMMAND_MEASURE_EC;
  } else if (ch == 'o') {
    type = COMMAND_OBSERVE_MODE_OFF;
  } else if (ch == '>') {
    type = COMMAND_BUFFER;
    this->bufferType = COMMAND_OBSERVE_MODE_ABOVE;
  } else if (ch == '<') {
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
