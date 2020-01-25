#include "UltrasonicSensor.h"
#include "MaxMatrix.h"
#include "Servo.h"
#include "RGBLed.h"
#define Buzzer 11
#define OpeningDistance 15

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

#define opened 0
#define closed 90
#define ServoPin 10
#define DIN 7
#define CS 6
#define CLK 5    //קביעת הפינים צריכה להתבצע כך בגלל אופן פעולת הספרייה 
#define maxInUse 1 //מספר הלוחות המחוברים

MaxMatrix matrix(DIN, CS, CLK, maxInUse);
UltrasonicSensor ExternalSensor(7, 8); //trig is 7, echo is 8
UltrasonicSensor InternalSensor(4, 6); //like the previous^
Servo servo;
RGBLed Led(3, 5, 0);

void setup()
{
  ExternalSensor.StoppingDistance, InternalSensor.StoppingDistance = OpeningDistance;
  pinMode(Buzzer, OUTPUT);
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
      digitalWrite(Buzzer, HIGH);
    }
    digitalWrite(Buzzer, LOW);

    OpenDoor();
    delay(5000);
    while (SomeoneIsComing()) {} //כלום
    CloseDoor();
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

void RefreshMatrix()
{
  int sprite = round(map(GetLowerDistance(), OpeningDistance, OpeningDistance + 10, 0, 4));
  char mysprit[10] = {Sprites[sprite]};
  matrix.writeSprite(0, 0, mysprit);
}

void CloseDoor()
{
  servo.write(closed);
  Led.TurnOn(colors::Red);
}

void OpenDoor()
{
  servo.write(opened);
  Led.TurnOn(colors::Green);
}
