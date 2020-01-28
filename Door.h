#ifndef Door_h
#define Door_h

#include "Arduino.h"
#include "locker.h"
#include "RGBLed.h"
#include "UltrasonicSensor.h"
#include "MaxMatrix.h"

class Door
{
  private:
    RGBLed Led;
    UltrasonicSensor ExternalSensor;
    UltrasonicSensor InternalSensor;
    byte Buzzer;
    byte maxInUse = 1;
    MaxMatrix matrix;
    static byte Sprites[6][10] = {{
        8, 8,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000
      }, {
        8, 8,
        B00000000,
        B00000000,
        B00000000,
        B00010000,
        B00000000,
        B00000000,
        B00000000,
        B00000000
      }, {
        8, 8,
        B00000000,
        B00000000,
        B00000000,
        B00011000,
        B00011000,
        B00000000,
        B00000000,
        B00000000
      }, {
        8, 8,
        B00000000,
        B00000000,
        B00111100,
        B00111100,
        B00111100,
        B00111100,
        B00000000,
        B00000000
      }, {
        8, 8,
        B00000000,
        B01111110,
        B01111110,
        B01111110,
        B01111110,
        B01111110,
        B01111110,
        B00000000
      }, {
        8, 8,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111
      }
    };
    bool SomeoneIsComing();
    bool BotheSidesAreBlocked();
    float GetLowerDistance();
    void RefreshMatrix();
    unsigned long CloseAt;

  public:
    Door(int HingePin, int LockerPin, int RedPin, int GreenPin, int InSensorTrig, int InSensorEcho, int ExSensorTrig, int ExSensorEcho, int Din, int Cs, int Clk, int BuzzerPin);
    locker Locker;
    locker Hinge;
    void Open();
    void Close();
    void Lock();
    void UnLock();
    bool IsLock();
    bool IsOpen();
    void Refresh();

};
#endif
