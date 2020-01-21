#include "locker.h"

locker::locker(int pin)
{
  servo.attach(pin);
  Close();
}

void locker::SetOpen(int NewOpen)//איכשהו פולימורפיזם
{
  if(NewOpen <= 0 && NewOpen >= 180)
  {
    OpenD = NewOpen;
    if(OpenD < 90)
    {
      CloseD = OpenD + 90;
    }
    else
    {
      CloseD = OpenD - 90;
    }
  }
}

void locker::Close()
{
  AmIOpened = false;
  servo.write(CloseD);
}

void locker::Open()
{
  AmIOpened = true;
  servo.write(OpenD);
}

bool locker::IsOpen()
{
  return(AmIOpened);
}
