#ifndef __PHMeter_h__
#define __PHMeter_h__

class PHMeter {
private:
  int pinAnalogIn;
  
public:
  PHMeter(int const pinAnalogIn);
  float PHMeter::measure();
};

#endif /* __ECMeter_h__ */
