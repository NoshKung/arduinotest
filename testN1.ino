#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "Wire.h"
#include "SPI.h"  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <Servo.h>

#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME           "Device"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define I2CADDR 0x21
#define sensor_pin A0 
Servo myservo;
RTC_DS3231 RTC;
LiquidCrystal_I2C lcd(0x27,16,2);
int read_ADC;
int ntu;


const byte ROWS = 4;  // กำหนดจำนวนของ Rows
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad_I2C keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );


int Relay1 = D4;
int Num1,Num2,Number;
int H;
int M;
int S;
char key,action;
boolean Mi = false;
boolean B = false;
boolean C = false;
boolean D = false;
boolean result = false;

void setup(){
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  
  pinMode(sensor_pin, INPUT);
  myservo.attach(D3);
  Wire.begin();
  keypad.begin( makeKeymap(keys) );
  RTC.begin();
  // ตั้งเวลาเตือนให้เอา // ข้างหน้า RTC.adjust ออก
  RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  RTC.setAlarm1Simple(H, M);
  RTC.turnOnAlarm(1);

  if (RTC.checkAlarmEnabled(1)) {
    Serial.println("Alarm Enabled");

  }
  DateTime now = RTC.now();
  pinMode(Relay1, OUTPUT);
  lcd.init();
  lcd.backlight();
}
  
void loop(){
  DateTime now = RTC.now(); 
  digitalWrite (Relay1 , HIGH);
  key = keypad.getKey();
  H = now.hour();
  M = now.minute();
  S = now.second();
  if (key == 'B'){
      B = true;
      C = false;
      D = false;
  }
  if (key == 'C'){
      C = true;
      B = false;
      D = false;
  }
  if (key == 'D'){
      B = false;
      C = false;
      D = true;
  }
    if (B == true){
      Time();
  }
  if (C == true){
      SetTime();
  }
  if (D == true){
      Water();
  }
 /* if (Num1 == H && Num2== M){
    myservo.write(180);
    delay(1000);
    myservo.write(0);
    delay(1000);
  }*/
  }
  if (key != NO_KEY){
  Serial.println(Number);
  }
    Blynk.run();

}
void Time(){
  DateTime now = RTC.now();  
  key = keypad.getKey();
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Time: ");
  lcd.setCursor(5, 0); 
  lcd.print(now.hour(), DEC);
  lcd.setCursor(7, 0);
  lcd.print(':');
  lcd.setCursor(9, 0);
  lcd.print(now.minute(), DEC);
  lcd.setCursor(11, 0);
  lcd.print(':');
  lcd.setCursor(13, 0);
  lcd.print(now.second(), DEC);
  
  lcd.setCursor(1, 1); 
  lcd.print("Set: ");
  lcd.setCursor(5, 1);
  lcd.print(Num1);
  lcd.setCursor(7, 1);
  lcd.print(':');
  lcd.setCursor(9, 1);
  lcd.print(Num2);
  delay(500);
  }


void SetTime(){
  Keypads();
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("SetTime: ");
  lcd.setCursor(9, 0);
  lcd.print(Num1);
  lcd.setCursor(11, 0);
  lcd.print(':');
  lcd.setCursor(13, 0);
  lcd.print(Num2);

  if (key == 'A'){
      Mi = true;
  }
  if (key=='*'){
      Mi = false;
  }
  if(Mi == true){
    lcd.setCursor(1, 1); 
    lcd.print("Minute: ");
    lcd.print(Number);
  }
  if(Mi == false){
    lcd.setCursor(3, 1);
    lcd.print("hour: ");
    lcd.setCursor(9, 1);
    lcd.print(Number);
  }
  delay(500);

}

void Water(){
    read_ADC = analogRead(sensor_pin);
    if(read_ADC>208)read_ADC=208;

    ntu = map(read_ADC, 0, 208, 300, 0); 
 
    lcd.setCursor(0,0);
    lcd.print("Turbidity: ");
    lcd.print(ntu);
    Serial.println(ntu);
    lcd.print("  ");

    lcd.setCursor(0,1);
    if(ntu<10)            lcd.print("Water Very Clean");
    if(ntu>=10 && ntu<30) lcd.print("Water Norm Clean");
    if(ntu>=30)           lcd.print("Water Very Dirty");

delay(100);
}
void Keypads()
{ 
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
    if (key == 'A' || key == 'B' || key == 'C'|| key == 'D'){
    Num1 = Number;  
    Number =0;
    delay(100);
    }
    
}
