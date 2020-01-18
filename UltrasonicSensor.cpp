#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor (int Trig, int Echo)
{
  SetPins(Trig, Echo);
}

UltrasonicSensor::UltrasonicSensor()
{
  UltrasonicSensor(4,5);
}

void UltrasonicSensor::SetPins(int Trig, int Echo)
{
  TrigPin = Trig;
  EchoPin = Echo;
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

bool UltrasonicSensor::Blocked()
{
  if (StoppingDistance > 150 || StoppingDistance < 3)
  {
    StoppingDistance = 10;
  }
  return (Get() < StoppingDistance);
}

float UltrasonicSensor::Get()
{
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  return (pulseIn(EchoPin, HIGH) / 58.77);
}

float UltrasonicSensor::GetAvg()
{
  GetAvg(5);
}

float UltrasonicSensor::GetAvg(int times)
{
  float sum = 0;
  for (int i = 0; i < times; ++i)
  {
    sum = sum + Get();
    delay(1);
  }
  return (sum / times);
}
