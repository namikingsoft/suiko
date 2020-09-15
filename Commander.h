#ifndef __Commander_h__
#define __Commander_h__

enum CommandType {
  COMMAND_NOOP,
  COMMAND_CYCLE_WATER,
  COMMAND_INPUT_WATER,
  COMMAND_OBSERVE_MODE_OFF,
  COMMAND_OBSERVE_MODE_ABOVE,
  COMMAND_OBSERVE_MODE_BELOW,
  COMMAND_MEASURE_EC
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
  char firstLetter = '\0';
  char payloadBuf[3];
  
public:
  Command receive(char  ch);
};

#endif /* __Commander_h__ */
