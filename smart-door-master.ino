#include "UltrasonicSensor.h"
#include "Servo.h"
#include "RGBLed.h"
#include "colors.h"

#define ServoPin 10
#define Buzzer 11

#define Close 90
#define Open 180
#define OpeningDistance 15

RGBLed CLed(3, 5, 11);//Red is 3, Green is 5, (Blue is 11)
Servo Engine;
UltrasonicSensor ExternalSensor(7, 8); //trig is 7, echo is 8
UltrasonicSensor InternalSensor(4, 6); //like the previous^

void setup()
{
  Engine.write(Close);
  Engine.attach(ServoPin);
  CLed.TurnOn(colors::Green);
  ExternalSensor.StoppingDistance, InternalSensor.StoppingDistance = OpeningDistance;
  pinMode(Buzzer, OUTPUT);
}

void loop()
{
  if (SomeoneIsComing())
  {
    CLed.TurnOn(colors::Red);
    
    while (ThereIsSomeoneOnOnlyOneSide())
    {
      digitalWrite(Buzzer, HIGH);
    }
    digitalWrite(Buzzer, LOW);

    Engine.write(Open);
    delay(5000);
    while (SomeoneIsComing()) {} //כלום
    Engine.write(Close);
    CLed.TurnOn(colors::Green);
  }

}

bool SomeoneIsComing()
{
  return (InternalSensor.Blocked() || ExternalSensor.Blocked());
}

bool ThereIsSomeoneOnOnlyOneSide()
{
  return ((InternalSensor.Blocked() || ExternalSensor.Blocked()) && !(InternalSensor.Blocked() && ExternalSensor.Blocked()));
}
