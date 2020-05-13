//----------ספריות בשימוש

#include "Sensors.h"
#include "Servo.h"
#include "RGBLed.h"

//#include "Arduino.h"// לא נחוץ בהכרח
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include <SPI.h>
#include "MFRC522.h"

#include <Wire.h>
#include "LQ.h"

//----------הגדרות

#define openedOutward 0
#define closed 90
#define openedInward 170

#define TX 3
#define RX 2//תחליף, אם זה לא עובד  
//זה תמיד צריך להיות טקס שלוש ורקס ארבע
#define SDA_PIN 10
#define RST_PIN 9
#define SCK_PIN 13 ///
#define MISO_PIN 12 /// 
#define MOSI_PIN 11 /// לא באמת צריכים הגדרה,

#define ServoPin 8
#define Busy_pin 6

#define OpeningSound 1
#define BuzzerSound 2
#define StartSound 3

//----------יצירת אובייקטים

SoftwareSerial mySoftwareSerial(RX, TX);
DFRobotDFPlayerMini Player;

LQ lcd(0x27, 16, 2);

MFRC522 mfrc522(SDA_PIN, RST_PIN);

AnalogSensor ExternalSensor(A0);
AnalogSensor InternalSensor(A1);

DigitalSensor Bizi(Busy_pin);
Servo servo;

#define BlueLedPin 4
#define GreenLedPin 5
//RGBLed Led(0, 5, 4);

//----------אתחול

void setup()
{
  Serial.begin(9600);

  Serial.println("SetUp");

  ExternalSensor.SetLevel(400);
  InternalSensor.SetLevel(400);

  servo.attach(ServoPin);

  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  Serial.println("SPI started");

  mySoftwareSerial.begin(9600);

  Serial.println("SoftwareSerial started");

  if (!Player.begin(mySoftwareSerial))
  {
    Serial.println("Problems with the player!!!");
  }


  Serial.println("player started");

  Player.volume(30);
  Player.play(StartSound);

  Serial.print("Player is playing");

  lcd.begin();
  lcd.backlight();
  Printo("ze tov3", 0, true);
  Serial.println("LCD started");
  /*
   */
}

void Test()
{
  while (!ReadChips()) {}
  Serial.println("Chip added");
}
//----------לוופ

void loop()
{
  if (SomeoneIsComing())
  {
    Printo("sensor alert!", 0, true);
    if (BothSidesAreBlocked())
    {
      Printo("be careful!", 1, false);//יש מישהו בצדד השני
      Player.play(BuzzerSound);
      while (BothSidesAreBlocked()) {} //כל עוד יש אנשים בשני הצדדים] אל תעשה כלום
    }
    OpenDoor();
    delay(5000); //חכה חמש שניות
    while (SomeoneIsComing()) {} //אל תסגור את הדלת כל עוד יש מישהו לידה
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

  //Serial.println(ReadChips());
}

//----------פונקציות

void TurnOnLed(bool IfItsBlueThenItsTrue)
{
  if (IfItsBlueThenItsTrue)
  {
    digitalWrite(GreenLedPin, LOW);
    delay(50);
    digitalWrite(BlueLedPin, HIGH);
  }
  else
  {
    digitalWrite(BlueLedPin, LOW);
    delay(50);
    digitalWrite(GreenLedPin, HIGH);
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
*/
void CloseDoor()
{
  Printo("closing door", 1, false);
  servo.write(closed);
  TurnOnLed(true);
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
  Player.play(OpeningSound);
  TurnOnLed(false);
}

bool ReadChips()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    //Serial.println("no card is here");
    return (false);
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    Serial.println("eror");
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
  Serial.println(content.substring(1));
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
}
