#include "UltrasonicSensor.h"
#include "Door.h"

#define Buzzer 11
#define OpeningDistance 15

Door door(10, 0, 3, 5);
//Door(int HingePin, int LockerPin, int RedPin, int GreenPin);//
UltrasonicSensor ExternalSensor(7, 8); //trig is 7, echo is 8
UltrasonicSensor InternalSensor(4, 6); //like the previous^

void setup()
{
  ExternalSensor.StoppingDistance, InternalSensor.StoppingDistance = OpeningDistance;
  pinMode(Buzzer, OUTPUT);
}

void loop()
{
  if (SomeoneIsComing())
  { 
    while (!ThereIsSomeoneOnOnlyOneSide())
    {
      digitalWrite(Buzzer, HIGH);
    }
    digitalWrite(Buzzer, LOW);

    door.Open();
    delay(5000);
    while (SomeoneIsComing()) {} //כלום
    door.Close();
  }

}

bool SomeoneIsComing()
{
  return (InternalSensor.Blocked() || ExternalSensor.Blocked());
}

bool ThereIsSomeoneOnOnlyOneSide()
{
  return (SomeoneIsComing() && !(InternalSensor.Blocked() && ExternalSensor.Blocked()));
}
