#ifndef locker_h
#define locker_h

#include "Arduino.h"
#include "Servo.h"

class locker
{
  private:
    Servo servo;
    int CloseD = 0;
    int OpenD = 90;
    bool AmIOpened = false;
  public:
    locker(int pin);
    void SetClose(int NewClose);
    void SetOpen(int NewOpen);
    void Close();
    void Open();
    bool IsOpen();
};
#endif
