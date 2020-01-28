#include "Door.h"

Door::Door(int HingePin, int LockerPin, int RedPin, int GreenPin, int InSensorTrig, int InSensorEcho, int ExSensorTrig, int ExSensorEcho, int Din, int Cs, int Clk, int BuzzerPin)
  : Locker(LockerPin), Hinge(HingePin), Led(RedPin, GreenPin, 0), matrix(Din, Cs, Clk, 1)
{
  Buzzer = BuzzerPin;
  pinMode(Buzzer, OUTPUT);
  matrix.init();
  matrix.setIntensity(15);
  UnLock();
  Close();
}

void Door::Open()
{
  if (Locker.IsOpen())
  {
    Hinge.Open();
  }
}

void Door::Close()
{
  Locker.Open();
  Hinge.Close();
}

void Door::Lock()
{
  if (!Hinge.IsOpen())
  {
    Locker.Close();
    Led.TurnOn(colors::Red);
  }
}

void Door::UnLock()
{
  Locker.Open();
  Led.TurnOn(colors::Green);
}

bool Door::IsLock()
{
  return (!Locker.IsOpen());
}

bool Door::IsOpen()
{
  return (Hinge.IsOpen());
}

bool Door::SomeoneIsComing()
{
  return (!BotheSidesAreBlocked() && (InternalSensor.Blocked() || ExternalSensor.Blocked()));
}

bool Door::BotheSidesAreBlocked()
{
  return (InternalSensor.Blocked() && ExternalSensor.Blocked());
}

float Door::GetLowerDistance()
{
  float inter = InternalSensor.Get();
  float exter = ExternalSensor.Get();
  if (inter > exter)
  {
    return (exter);
  }
  return (inter);
}

void Door::RefreshMatrix()////////**********///////
{
  int sprite = round(map(GetLowerDistance(), InternalSensor.StoppingDistance, InternalSensor.StoppingDistance + 10, 0, 4));
  byte mysprit[10] = {Sprites[sprite]};
  matrix.writeSprite(0, 0, mysprit);
}

void Door::Refresh()
{
  if (BotheSidesAreBlocked())
  {
    digitalWrite(Buzzer, HIGH);
  }
  else
  {
    digitalWrite(Buzzer, LOW);
  }

  if (SomeoneIsComing())
  {
    Open();
    CloseAt = millis() + 5000;
  }

  if (millis() > CloseAt && !(SomeoneIsComing() || BotheSidesAreBlocked()))
  {
    Close();
  }

  RefreshMatrix();
}
