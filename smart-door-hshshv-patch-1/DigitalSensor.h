#ifndef DigitalSensor_h
#define DigitalSensor_h

#include "Sensor.h"

class DigitalSensor: public Sensor
{
  public:
    DigitalSensor(byte InPin);
    bool Activated();
};

#endif
