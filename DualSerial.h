#ifndef __DualSerial_h__
#define __DualSerial_h__

#include <SoftwareSerial.h>

class DualSerial {
private:
  SoftwareSerial* serial;
  
public:
  DualSerial(SoftwareSerial* serial);
  void begin(int const bps);
  void print(char* const str);
  void print(float const num);
  void println(char* const str);
  void write(char const ch);
  char read();
  int available();
};

#endif /* __DualSerial_h__ */
