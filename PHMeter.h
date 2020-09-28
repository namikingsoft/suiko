#ifndef __PHMeter_h__
#define __PHMeter_h__

#define V2PH_SLOPE -5.81261548798925
#define V2PH_INTERCEPT 21.9317612968251

class PHMeter {
private:
  int pinAnalogIn;
  
public:
  PHMeter(int const pinAnalogIn);
  float PHMeter::measure();
};

#endif /* __ECMeter_h__ */
