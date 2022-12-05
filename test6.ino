#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "SPI.h"  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <Servo.h>
Servo myservo;
RTC_DS3231 RTC;
LiquidCrystal_I2C lcd(0x27,16,2);
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int Relay1 = 11;
int Num1,Num2,Number;
int H;
int M;
int S;
char key,action;
boolean result = false;

void setup(){
  Serial.begin(9600);
  pinMode(A0,INPUT);
  myservo.attach(10);
  Wire.begin();
  RTC.begin();
  // ตั้งเวลาเตือนให้เอา // ข้างหน้า RTC.adjust ออก
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, HIGH);
  lcd.init();
  lcd.backlight();
}
  
void loop(){
  DateTime now = RTC.now();  
  digitalWrite(Relay1, HIGH);
  key = keypad.getKey();
  lcd.setCursor(0, 0); 
  lcd.print("Time: ");
  
  lcd.setCursor(5, 0); 
  lcd.print(now.hour(), DEC);
  lcd.setCursor(7, 0);
  lcd.print(':');
  lcd.setCursor(9, 0);
  lcd.print(now.minute(), DEC);
  H = now.hour();
  M = now.minute();
  S = now.second();
  
  lcd.setCursor(1, 1); 
  lcd.print("Set: ");

  if (Num1 == H && Num2== M){
    myservo.write(150);
    delay(1000);
    myservo.write(180);
    delay(1000);
  }
  else if (Num1 == H && Num2== M && S < 10){
    myservo.write(0);
  }
  Keypads();
  SetTime();
  Water();
  if (key != NO_KEY){
  Serial.println(key);
  }
}

void Keypads(){ 
    lcd.clear();
    if (key=='*') {
    Number=Num1=Num2=0; result=false;
    lcd.clear();
    }
    if (key == '1'){
    if (Number==0)
    Number=1;
    else
    Number = (Number*10) + 1;
    }
    if (key == '4'){
    if (Number==0)
    Number=4;
    else
    Number = (Number*10) + 4;
    }
    if (key == '7'){
    if (Number==0)
    Number=7;
    else
    Number = (Number*10) + 7;
    }
    if (key == '0'){
    if (Number==0)
    Number=0;
    else
    Number = (Number*10) + 0;
    }
    if (key == '2'){
    if (Number==0)
    Number=2;
    else
    Number = (Number*10) + 2;
    }
    if (key == '5'){
    if (Number==0)
    Number=5;
    else
    Number = (Number*10) + 5;
    }
    if (key == '8'){
    if (Number==0)
    Number=8;
    else
    Number = (Number*10) + 8;
    }
    if (key == '#'){
    Num2=Number;
    result = true;
    }
    if (key == '3'){
    if (Number==0)
    Number=3;
    else
    Number = (Number*10) + 3;
    }
    if (key == '6'){
    if (Number==0)
    Number=6;
    else
    Number = (Number*10) + 6;
    }
    if (key == '9'){
    if (Number==0)
    Number=9;
    else
    Number = (Number*10) + 9;
    }
    if (key == 'A'){
    Num1 = Number;  
    Number =0;
    delay(100);
  }
}

void SetTime(){
  lcd.setCursor(5, 1);
  lcd.print(Num1);
  lcd.setCursor(7, 1);
  lcd.print(':');
  lcd.setCursor(9, 1);
  lcd.print(Num2);
}
void Water(){
  int sensor = analogRead(A0);
  float volte = sensor*(5.0/1024.0);
  lcd.setCursor(11, 1);
  lcd.print("V:");
  lcd.setCursor(13, 1);
  lcd.print(volte);
  delay(500);
}
