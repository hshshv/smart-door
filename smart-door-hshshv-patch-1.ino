#include "Sensors.h"
#include "MaxMatrix.h"
#include "Servo.h"
#include "RGBLed.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


#include <SPI.h>
#include "MFRC522.h"

#include <Wire.h>
#include "LQ.h"

#define openedOutward 0
#define closed 90
#define openedInward 180

#define SDA_PIN 10
#define RST_PIN 9
#define BuzzerPin 2
#define ServoPin 8
//#define DIN 7
//#define CS 6
//#define CLK 5
#define maxInUse 1 //מספר הלוחות המחוברים
#define TX 2
#define RX 3
#define Busy_pin 5
SoftwareSerial SOS(RX, TX);
DFRobotDFPlayerMini Player;

LQ lcd(0x27, 16, 2);
MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Create MFRC522 instance.
//MaxMatrix matrix(DIN, CS, CLK, maxInUse);
AnalogSensor ExternalSensor(A0);
AnalogSensor InternalSensor(A1);
//Led Buzzer(BuzzerPin);
DigitalSensor Bizi(Busy_pin);
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
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  Printo("mashoo chip", 0, true);
  SOS.begin(9600);
  Player.volume(30); // 0 - 30
  if(Player.begin(SOS))//הצליח להתחיל את הנגן
  {
    Printo("MP3 added", 0, true);
  }
  else
  {
    Printo("FAIL to add MP3", 0, true);
  }
}

void loop()
{
  if (SomeoneIsComing())
  {
    Printo("sensor alert!", 0, false);
    while (BothSidesAreBlocked())
    {
      if(!Bizi.Activated())
      {
        Player.play(0);
      }
    }
    OpenDoor();
    delay(5000);
    while (SomeoneIsComing()) {} //כלום
    CloseDoor();
  }

  if (ReadChips())
  {
    Printo("chip detected", 0, false);
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

bool BothSidesAreBlocked()
{
  return (InternalSensor.Activated() || ExternalSensor.Activated());
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
  Printo("closing door", 1, false);
  servo.write(closed);
  Led.TurnOn(colors::Red);
}

void OpenDoor()
{
  Printo("opening door", 1, false);
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
    return (false);
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return (false);
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

void Printo(String text, int line, bool Clear)
{
  if (Clear)
  {
    lcd.clear();
  }
  lcd.setCursor(0, line);
  lcd.print(text);
  /*/*//*/*/
}
