#include "Sensors.h"
#include "MaxMatrix.h"
#include "Servo.h"
#include "RGBLed.h"


#include <SPI.h>
#include "MFRC522.h"

#define openedOutward 0
#define closed 90
#define openedInward 180

#define SS_PIN 10
#define RST_PIN 9
#define BuzzerPin 2
#define ServoPin 8
#define DIN 7
#define CS 6
#define CLK 5    //קביעת הפינים צריכה להתבצע כך בגלל אופן פעולת הספרייה 
#define maxInUse 1 //מספר הלוחות המחוברים

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
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
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop()
{/*
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
  }*/
  
  if(ReadChips())
  {
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

bool ReadChips()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "A9 68 62 A3") //change here the UID of the card/cards that you want to give access
  {
    return (true);
  }

  else
  {
    return (false);
  }
}
