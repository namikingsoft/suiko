#ifndef __ECMeter_h__
#define __ECMeter_h__

#include <DallasTemperature.h>

struct ECResult {
  float temperature;
  float ec25;
};

class ECMeter {
private:
  DallasTemperature* sensors;
  int pinInput;
  int pinPower;
  
public:
  ECMeter(DallasTemperature* sensors, int pinInput, int pinPower);
  ECResult ECMeter::measure();
};

#endif /* __ECMeter_h__ */
