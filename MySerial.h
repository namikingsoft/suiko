#ifndef __MySerial_h__
#define __MySerial_h__

#include <SoftwareSerial.h>

class MySerial {
private:
  SoftwareSerial* serial;
  
public:
  MySerial(SoftwareSerial* serial);
  void begin(int const bps);
  void print(char* const str);
  void print(float const num);
  void println(char* const str);
  void write(char const ch);
  char read();
  int available();
};

#endif /* __MySerial_h__ */
