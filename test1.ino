#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "SPI.h"
#include "RTClib.h"

RTC_DS3231 RTC;
Servo myservo;

float volte = sensor * (5.0 / 1024.0);
int servo = 10;
int og = 9;
int JT = 11;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);
ะ int cursorColumn = 0;

void setup() {
  Serial.begin(9600);
  pinMode(servo, OUTPUT);
  pinMode(og, OUTPUT);
  pinMode(A0, INPUT);
  
  lcd.begin();
  lcd.backlight();

  Wire.begin();
  RTC.begin();
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  //ตั้งเวลา
  RTC.setAlarm1Simple(8, 30);
  RTC.turnOnAlarm(1);
  if (RTC.checkAlarmEnabled(1)) {
    Serial.println("Enabled");
  }
}
void loop() {
  fish();
  lcd();
  changewater();
  oxygen();
}
void fish() {
  DateTime now = RTC.now();
  if (RTC.checkIfAlarm(1)) {
    Serial.println("food on");
    digitalWrite(servo, HIGH);
    delay(2000);
    digitalWrite(servo, LOW);
  }
}
void water_check() {
  int sensor = analogRead(A0);
  volte = sensor * (5.0 / 1024.0);
  Serial.println(volte);
  delay(500);
}
void oxygen() {
  digitalWrite(og, HIGH);
}
void changewater() {
  if (volte <= 883) {
    digitalWrite(JT, HIGH);
  } else if (volte >= 882) {
    digitalWrite(JT, LOW);
  }
}
void lcd() {
  char key = keypad.getKey();

  if (key) {
    lcd.setCursor(cursorColumn, 9);
    lcd.print(key);

    cursorColumn++;
    if (cursorColumn == 16) {
      lcd.clear();
      cursorColumn = 0;
    }
  lcd.setCursor(0, 0);
  lcd.print("hum:     ");
  lcd.setCursor(4, 0);
  lcd.print(volte);
  delay(500);
}
