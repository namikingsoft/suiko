#ifndef __MySerial_h__
#define __MySerial_h__

#include <SoftwareSerial.h>

class MySerial {
private:
  SoftwareSerial* serial;
  
public:
  MySerial(SoftwareSerial* serial);
  void begin(int bps);
  void print(char* str);
  void print(float num);
  void println(char* str);
  void write(char ch);
  char read();
  int available();
};

#endif /* __MySerial_h__ */
