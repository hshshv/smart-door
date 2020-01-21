#ifndef Door_h
#define Door_h

#include "Arduino.h"
#include "locker.h"
#include "RGBLed.h"

class Door
{
  private:
    RGBLed Led;
  public:
    Door(int HingePin, int LockerPin, int RedPin, int GreenPin);
    locker Locker;
    locker Hinge;
    void Open();
    void Close();
    void Lock();
    void UnLock();
    bool IsLock();
    bool IsOpen();
};
#endif
