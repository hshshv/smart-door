#include "Sensors.h"
#include "MaxMatrix.h"
#include "Servo.h"
#include "RGBLed.h"

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define TX 3
#define RX 2
SoftwareSerial mySoftwareSerial(RX, TX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#include <SPI.h>
#include "MFRC522.h"

#include <Wire.h>
#include "LQ.h"

#define openedOutward 0
#define closed 90
#define openedInward 180

#define SDA_PIN 100
#define RST_PIN 90
#define BuzzerPin 2
#define ServoPin 8
#define Busy_pin 6
DFRobotDFPlayerMini Player;

LQ lcd(0x27, 16, 2);
MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Create MFRC522 instance.
AnalogSensor ExternalSensor(A0);
AnalogSensor InternalSensor(A1);
DigitalSensor Bizi(Busy_pin);
Servo servo;
RGBLed Led(4, 5, 0);

void setup()
{
  InternalSensor.InPin = 1;
  ExternalSensor.SetLevel(400);
  InternalSensor.SetLevel(400);
  servo.attach(ServoPin);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  lcd.begin();
  lcd.backlight();
  Printo("mashoo chip", 0, true);


  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }


  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);

}
void loop()
{
  if (SomeoneIsComing())
  {
    Printo("sensor alert!", 0, false);
    while (BothSidesAreBlocked())
    {
      if (!Bizi.Activated())
      {
        Player.play(1);
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
