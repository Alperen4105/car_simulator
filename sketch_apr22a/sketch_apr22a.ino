#include <LiquidCrystal.h>
#define LDR_THRESHOLD 250

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// Input
const int motorButton = 22;
const int seatbeltButton = 23;
const int temp = A0;
const int Light = A1;
const int Fuel = A2;
const int switch_door = 24; 

// Output
const int redLED = 25;
const int buzzer = 26;
const int dcMotor = 27;
const int dcFan = 28;
const int blueLED = 29;
const int yellowLED= 31;
const int RGBred = 32;
const int RGBblue = 33; 

unsigned long prevMillis = 0; 
bool ledState = LOW;
bool motorRunning = false;
bool fanRunning = false;
bool kapiAcik = false;
bool fuelState = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(motorButton, INPUT_PULLUP);
  pinMode(seatbeltButton, INPUT_PULLUP);
  pinMode(temp,INPUT);
  pinMode(Light,INPUT);
  pinMode(Fuel,INPUT);
  pinMode(switch_door, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(dcMotor, OUTPUT);
  pinMode(dcFan, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED,OUTPUT);
  pinMode(RGBred, OUTPUT);
  pinMode(RGBblue, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Sistem Hazir");
  delay(1500);
  lcd.clear();
}
void loop(){

  FUEL4();
  DOOR5();



  if(motorRunning) { 
    TempAC2();
    HeadLight3();
  } else{
    digitalWrite(yellowLED, HIGH);
    digitalWrite(buzzer, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);
    digitalWrite(dcFan, LOW);
  }
}
void EngSb1() {
  bool motorBasili = !digitalRead(motorButton);    
  bool kemerTakili = !digitalRead(seatbeltButton);

  if (motorBasili && kemerTakili && !motorRunning) {
    digitalWrite(dcMotor, HIGH);
    digitalWrite(buzzer, LOW);
    digitalWrite(redLED, LOW);
    lcd.clear();
    lcd.print("Motor Calisiyor");
    motorRunning = true;
  }
  else if (motorRunning && !kemerTakili) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLED, HIGH);
    lcd.clear();
    lcd.print("KEMERI TAK!");
  }
  else if (motorRunning && kemerTakili) {
    digitalWrite(buzzer, LOW);
    digitalWrite(redLED, LOW);
  }

  static bool oncekiButon = HIGH;
  bool simdikiButon = digitalRead(motorButton);
  
  if (oncekiButon == LOW && simdikiButon == HIGH && motorRunning) {
    digitalWrite(dcMotor, LOW);
    lcd.clear();
    lcd.print("Motor Kapali");
    motorRunning = false;
  }
  oncekiButon = simdikiButon;

  delay(500);
  lcd.clear();
}

void TempAC2(){

  int ANALOGvalue = analogRead(temp);
  float volt = ANALOGvalue * (5.0/1023.0);
  float TEMP = volt * 100;

  Serial.print("Sıcaklık: ");
  Serial.print(TEMP);
  Serial.println(" °C");

  if (TEMP > 25.0){
    digitalWrite(dcFan,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(TEMP);
    lcd.setCursor(0,1);
    delay(500);
    lcd.print("Air-Condition ON");
  } else {
    digitalWrite(dcFan, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(TEMP);
    delay(500);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Air-Condition OFF");
    delay(500);
  }

delay(500);
}

void HeadLight3() {
  int ldrValue = analogRead(Light);
  Serial.print("LDR Değeri: ");
  Serial.println(ldrValue);

  if (ldrValue <= LDR_THRESHOLD) { 
    digitalWrite(blueLED, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Karanlik:");
    lcd.print(ldrValue);
    lcd.setCursor(0, 1);
    lcd.print("Farlar: ACIK");
  } else { 
    digitalWrite(blueLED, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aydinlik");
    lcd.print(ldrValue);
    lcd.setCursor(0, 1);
    lcd.print("Farlar: KAPALI");
  }
  
  delay(500); 
}

void FUEL4() {
  int potValue = analogRead(Fuel);
  int fuelPercent = map(potValue, 0, 1023, 0, 100);
  
  lcd.clear();
  lcd.print("Pot: ");
  lcd.print(potValue);
  lcd.print(" -> %");
  lcd.println(fuelPercent);


  if(fuelPercent <= 0) {
    motorRunning = false;
    digitalWrite(dcMotor, LOW);
    lcd.clear();
    lcd.print("YAKIT BITTI!");
    digitalWrite(yellowLED, HIGH);
    delay(500);
    fuelState = false;
    return;
  }
  else { 
    fuelState = true;
    if(fuelPercent < 5) {
      criticalFuel(fuelPercent);
    }
    else if(fuelPercent < 10) {
      lowFuel(fuelPercent);
    }
    else {
      normalFuel(fuelPercent);
    }
  }

  delay(500);
}


void criticalFuel(int percent) {
  static unsigned long prevMillis = 0;
  static bool ledState = LOW;
  
  if(millis() - prevMillis >= 500) {
    prevMillis = millis();
    ledState = !ledState;
    digitalWrite(yellowLED, ledState);
  }

  lcd.clear();
  lcd.print("KRITIK UYARI!");
  lcd.setCursor(0,1);
  lcd.print("Yakit: %");
  lcd.print(percent);
  delay(500);
}

void lowFuel(int percent) {
  digitalWrite(yellowLED, HIGH);
  lcd.clear();
  lcd.print("UYARI: Yakit Az");
  lcd.setCursor(0,1);
  lcd.print("Seviye: %");
  lcd.print(percent);
  delay(500);
}

void normalFuel(int percent) {
  digitalWrite(yellowLED, LOW);
  lcd.clear();
  lcd.print("Yakit Seviyesi:");
  lcd.setCursor(0,1);
  lcd.print("%");
  lcd.print(percent);
  lcd.print(" - NORMAL");
  delay(500);
}

void DOOR5() {
  kapiAcik = digitalRead(switch_door); 
  
  if(kapiAcik) {
    analogWrite(RGBred, 255);
    analogWrite(RGBblue, 80); 
    lcd.clear();
    lcd.print("UYARI: KAPI ACIK");
    lcd.setCursor(0,1);
    lcd.print("MOTOR CALISMAZ!");
    
    if(motorRunning) {
      digitalWrite(dcMotor, LOW);
      motorRunning = false;
    }
  }
   else {
    digitalWrite(RGBred, LOW);
    digitalWrite(RGBblue, LOW);
    lcd.clear();
    lcd.print("KAPI KAPALI");
    if(fuelState){
      EngSb1();
    }
  }
  
  delay(200);
}





