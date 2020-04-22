#ifndef AnalogSensor_h
#define AnalogSensor_h

#include "Sensor.h"

class AnalogSensor: public Sensor
{
  public:
    AnalogSensor(byte InPin, int activatedLevel);
    AnalogSensor(byte InPin);
    void SetLevel(int NewLevel);
    int Get();
    bool Activated();
  private:
    int ActivatedLevel;
};

#endif
