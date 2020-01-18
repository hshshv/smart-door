#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

#include "Arduino.h"
class UltrasonicSensor
{
  private:
    int TrigPin;
    int EchoPin;
  public:
    UltrasonicSensor(int Trig, int Echo);
    UltrasonicSensor();
    void SetPins(int Trig, int Echo);
    int StoppingDistance = 10;
    float Get();
    float GetAvg();
    float GetAvg(int times);
    bool Blocked();
};
#endif
