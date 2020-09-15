#ifndef __Commander_h__
#define __Commander_h__

#include "Arduino.h"

enum CommandType {
  COMMAND_BUFFER,
  COMMAND_NOT_FOUND,
  COMMAND_CYCLE_WATER,
  COMMAND_INPUT_WATER,
  COMMAND_MEASURE_EC,
  COMMAND_OBSERVE_MODE_OFF,
  COMMAND_OBSERVE_MODE_ABOVE,
  COMMAND_OBSERVE_MODE_BELOW,
};

union CommandPayload {
  bool is;
  float ec;
};

struct Command {
  CommandType type;
  CommandPayload payload;
};

class Commander {
private:
  CommandType bufferType = COMMAND_NOT_FOUND;
  String bufferPayload = String(4);
  void clearBuffer();  
  
public:
  Command receive(char  ch);
};

#endif /* __Commander_h__ */
