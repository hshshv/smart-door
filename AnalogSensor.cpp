#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(byte inPin, int activatedLevel)
{
  InPin = inPin;
  ActivatedLevel = activatedLevel;
}

AnalogSensor::AnalogSensor(byte inPin):  AnalogSensor(inPin, 512)
{
}

void AnalogSensor::SetLevel(int NewLevel)
{
  ActivatedLevel = NewLevel;
}
    
int AnalogSensor::Get()
{
  return (analogRead(InPin));
}

bool AnalogSensor::Activated()
{
  return (Get() >= ActivatedLevel);
}
