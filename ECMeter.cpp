#include "ECMeter.h"

#define V_IN_RC 5
#define R_PIN 25 // Resistor of Arduino pin
#define R_EC (1000 + R_PIN) // Do not set Less than 300

#define TEMPERATURE_COEF 0.019
#define EC_CELL_CONSTANT 2.80

ECMeter::ECMeter(DallasTemperature* const sensors, int const pinInput, int const pinPower) {
  this->sensors = sensors;
  this->pinInput = pinInput;
  this->pinPower = pinPower;
}

ECResult ECMeter::measure() {  
  this->sensors->requestTemperatures();
  float temperature = this->sensors->getTempCByIndex(0);

  digitalWrite(this->pinPower, HIGH);
  analogRead(this->pinInput); // Throw away because first reading will be low
  float raw = analogRead(this->pinInput);
  digitalWrite(this->pinPower, LOW);

  float vDrop = (V_IN_RC * raw) / 1024.0;
  float rc = (vDrop * R_EC) / (V_IN_RC - vDrop);
  rc = rc - R_PIN; //acounting for Digital Pin Resitance
  float ec = 1000 / (rc * EC_CELL_CONSTANT);

  float ec25  =  ec / (1 + TEMPERATURE_COEF * (temperature - 25.0));
  return { temperature, ec25 };
}
