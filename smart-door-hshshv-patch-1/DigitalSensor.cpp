#include "DigitalSensor.h"

DigitalSensor::DigitalSensor(byte inPin)
{
  InPin = inPin;
  pinMode(InPin, INPUT);
}

bool DigitalSensor::Activated()
{
  return (digitalRead(InPin));
}
