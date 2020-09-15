#include "Commander.h"

Command Commander::receive(char ch) {
  CommandType type = COMMAND_NOOP;
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
  } else if (ch == 'a') {
    type = COMMAND_OBSERVE_MODE_OFF;
  } else if (ch == 's') {
    type = COMMAND_OBSERVE_MODE_ABOVE;
  } else if (ch == 'd') {
    type = COMMAND_OBSERVE_MODE_BELOW;
  } else if (ch == 'm') {
    type = COMMAND_MEASURE_EC;
  }
  return { type, payload };
}
