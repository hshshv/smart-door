#include "Door.h"

Door::Door(int HingePin, int LockerPin, int RedPin, int GreenPin)
  :Locker(LockerPin), Hinge(HingePin), Led(RedPin, GreenPin, 0)
{
  UnLock();
  Close();
}

void Door::Open()
{
  Hinge.Open();
}

void Door::Close()
{
  Hinge.Close();
}

void Door::Lock()
{
  Locker.Close();
  Led.TurnOn(colors::Red);
}

void Door::UnLock()
{
  Locker.Open();
  Led.TurnOn(colors::Green);
}

bool Door::IsLock()
{
  return(!Locker.IsOpen());
}

bool Door::IsOpen()
{
  return(Hinge.IsOpen());
}
