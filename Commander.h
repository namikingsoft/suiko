#ifndef __Commander_h__
#define __Commander_h__

#include "Arduino.h"

enum CommandType {
  COMMAND_BUFFER,
  COMMAND_NOT_FOUND,
  COMMAND_CYCLE_WATER,
  COMMAND_INPUT_WATER,
  COMMAND_AIR_PUMP,
  COMMAND_MEASURE_EC,
  COMMAND_OBSERVE_MODE_OFF,
  COMMAND_OBSERVE_MODE_ABOVE,
  COMMAND_OBSERVE_MODE_BELOW,
};

#define CHAR_CYCLE_WATER_ON 'C'
#define CHAR_CYCLE_WATER_OFF 'c'
#define CHAR_INPUT_WATER_ON 'I'
#define CHAR_INPUT_WATER_OFF 'i'
#define CHAR_AIR_PUMP_ON 'A'
#define CHAR_AIR_PUMP_OFF 'a'
#define CHAR_MEASURE_EC 'm'
#define CHAR_OBSERVE_MODE_OFF 'o'
#define CHAR_OBSERVE_MODE_ABOVE '>'
#define CHAR_OBSERVE_MODE_BELOW '<'

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
  Command receive(char const ch);
};

#endif /* __Commander_h__ */
