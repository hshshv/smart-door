#include "Sensors.h"
#include "MaxMatrix.h"
#include "Servo.h"
#include "RGBLed.h"
#define BuzzerPin 11
/*
char Sprites[5][10] = {{
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
  }
};
*/
#define openedOutward 0
#define closed 90
#define openedInward 180
#define ServoPin 10
#define DIN 7
#define CS 6
#define CLK 5    //קביעת הפינים צריכה להתבצע כך בגלל אופן פעולת הספרייה 
#define maxInUse 1 //מספר הלוחות המחוברים

MaxMatrix matrix(DIN, CS, CLK, maxInUse);
AnalogSensor ExternalSensor(A0);
AnalogSensor InternalSensor(A1); 
Led Buzzer(BuzzerPin);

Servo servo;
RGBLed Led(3, 5, 0);

void setup()
{
  InternalSensor.InPin = 1;
  ExternalSensor.SetLevel(400);
  InternalSensor.SetLevel(400);
  servo.attach(ServoPin);
  matrix.init();
  matrix.setIntensity(15);
}

void loop()
{
  if (SomeoneIsComing())
  {
    while (!ThereIsSomeoneOnOnlyOneSide())
    {
      Buzzer.TurnOn();
    }
    Buzzer.TurnOff();

    OpenDoor();
    delay(5000);
    while (SomeoneIsComing()) {} //כלום
    CloseDoor();
  }

}

bool SomeoneIsComing()
{
  return (InternalSensor.Activated() || ExternalSensor.Activated());
}

bool ThereIsSomeoneOnOnlyOneSide()
{
  return (SomeoneIsComing() && !(InternalSensor.Activated() && ExternalSensor.Activated()));
}
/*
float GetLowerDistance()
{
  float inter = InternalSensor.Get();
  float exter = ExternalSensor.Get();
  if (inter > exter)
  {
    return (exter);
  }
  return (inter);
}
/*
void RefreshMatrix()
{
  int sprite = round(map(GetLowerDistance(), OpeningDistance, OpeningDistance + 10, 0, 4));
  char mysprit[10] = {Sprites[sprite]};
  matrix.writeSprite(0, 0, mysprit);
}
*/
void CloseDoor()
{
  servo.write(closed);
  Led.TurnOn(colors::Red);
}

void OpenDoor()
{
  if (ExternalSensor.Activated())
  {
    servo.write(openedInward);
  }
  else
  {
    servo.write(openedOutward);
  }
    
  Led.TurnOn(colors::Green);
}
