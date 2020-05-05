//----------ספריות בשימוש

#include "Sensors.h"
#include "Servo.h"
#include "RGBLed.h"

//#include "Arduino.h"// לא נחוץ בהכרח
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define TX 3
#define RX 2//תחליף, אם זה לא עובד  

#include <SPI.h>
#include "MFRC522.h"

#include <Wire.h>
#include "LQ.h"

//----------הגדרות

#define openedOutward 0
#define closed 90
#define openedInward 180

#define SDA_PIN 10
#define RST_PIN 9
#define ServoPin 8
#define Busy_pin 6

#define OpeningSund 3
#define BuzzerSound 2
#define StartSound 1

//----------יצירת אובייקטים

SoftwareSerial mySoftwareSerial(RX, TX);
DFRobotDFPlayerMini Player;

LQ lcd(0x27, 16, 2);

MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Create MFRC522 instance.

AnalogSensor ExternalSensor(A0);
AnalogSensor InternalSensor(A1);

DigitalSensor Bizi(Busy_pin);
Servo servo;

RGBLed Led(4, 5, 0);

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
  
  if(!Player.begin(mySoftwareSerial))
  {
    Serial.println("Problems with the player");
  }
  

  Serial.println("player started");
  
  lcd.begin();
  lcd.backlight();
  Printo("mashoo chip", 0, true);

  Serial.println("LCD started");

  Player.volume(30); 
  Player.play(1);

  Serial.println("player is playing");

}

//----------לוופ

void loop()
{/*
  if (SomeoneIsComing())
  {
    Printo("sensor alert!", 0, true);
    if(BothSidesAreBlocked())
    {
      Printo("be careful!", 1, false);//יש מישהו בצדד השני  
      Player.play(BuzzerSound);
      while(BothSidesAreBlocked()){}//כל עוד יש אנשים בשני הצדדים] אל תעשה כלום  
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
  */
}

//----------פונקציות

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
}
