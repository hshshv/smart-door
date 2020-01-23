#include "UltrasonicSensor.h"
#include "Door.h"
//אלו הספריות שבהם ייעשה שימוש בהמשך הקוד
#define Buzzer 11
#define OpeningDistance 15
//הגדר: כל פעם שכתוב 'זמזם' הארדואינו יידע שזה אמור להיות 11, וכל פעם שכתוב 'מרחק פתיחה' זה אמור להיות 15

Door door(10, 0, 3, 5);
//הכרזה על דלת חדשה בשם 'door'
//מנוע הסרוו שפותח את הדלת יחובר לפין 10, אין מנוע שנועל את הדלת לכן כתוב 0, הלד האדום יחובר לפין 3, והירוק לפין חמש
//Door(int HingePin, int LockerPin, int RedPin, int GreenPin);//
UltrasonicSensor ExternalSensor(7, 8); //trig is 7, echo is 8//, הכרזה על החיישן האולטראסוני החיצוני שיחובר לפינים שבע ושמונה
UltrasonicSensor InternalSensor(4, 6); //like the previous^//הכרזה על החיישן האולטראסוני הפנימי

void setup()
{
  ExternalSensor.StoppingDistance, InternalSensor.StoppingDistance = OpeningDistance;//מגדיר את מרחק העצירה  של החיישנים ל'מרחק פתיחה' שהוגדר מקודם, כדי שנדע מתי החיישן נחשב 'חסום' ומתי לא
  pinMode(Buzzer, OUTPUT);//הגדרה של פין של הזמזם כפין יציאה
}

void loop()
{
  if (SomeoneIsComing())//אם מישהו מגיע (קריאה לפונקצייה שנקראת כך)
  { 
    while (BothSensorsAreBlocked())//כל עוד יש אנשים בשתי הצדדים
    {
      digitalWrite(Buzzer, HIGH);//הפעל את הזמזם
    }
    digitalWrite(Buzzer, LOW);//כבה את הזמזם

    door.Open();//פתח את הדלת
    delay(5000);//חכה חמש שניות
    while (SomeoneIsComing()) {} //חכה עד שלא יהיה מישהו ליד הדלת
    door.Close();//סגור את הדלת
  }

}

bool SomeoneIsComing()//הפונקצייה הזו תחזיר 'אמת' אם מישהו מתקרב לדלת, ושקר אם לא
{
  return (InternalSensor.Blocked() || ExternalSensor.Blocked());
}

bool BothSensorsAreBlocked()//הפונקצייה הזו תחזיר 'אמת' אם יש אנשים בשני הצדדים, ושקר אם לא - אם יש בצד אחד או באף צד
{
  return (InternalSensor.Blocked() && ExternalSensor.Blocked());
}
