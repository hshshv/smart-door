#include "locker.h"

locker::locker(int pin)
{
  servo.attach(pin);
  Close();
}

void locker::SetClose(int NewClose)
{
  if(NewClose <= 0 && NewClose >= 180)
  {
    CloseD = NewClose;
  }
}

void locker::SetOpen(int NewOpen)//איכשהו פולימורפיזם
{
  if(NewOpen <= 0 && NewOpen >= 180)
  {
    OpenD = NewOpen;
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
