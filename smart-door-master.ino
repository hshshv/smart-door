#include "UltrasonicSensor.h"
#include "Door.h"
#include "MaxMatrix.h"
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

#define DIN 7
#define CS 6
#define CLK 5    //קביעת הפינים צריכה להתבצע כך בגלל אופן פעולת הספרייה 
#define maxInUse 1 //מספר הלוחות המחוברים
MaxMatrix matrix(DIN, CS, CLK, maxInUse);

Door door(10, 0, 3, 5);
//Door(int HingePin, int LockerPin, int RedPin, int GreenPin);//
UltrasonicSensor ExternalSensor(7, 8); //trig is 7, echo is 8
UltrasonicSensor InternalSensor(4, 6); //like the previous^

void setup()
{
  ExternalSensor.StoppingDistance, InternalSensor.StoppingDistance = OpeningDistance;
  pinMode(Buzzer, OUTPUT);
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

void RefreshMatrix()////////**********///////
{
  int sprite = round(map(GetLowerDistance(), OpeningDistance, OpeningDistance + 10, 0, 4));
  char mysprit[10] = {Sprites[sprite]};
  matrix.writeSprite(0, 0, mysprit);
}
