#include <Wire.h>
#include "bike_logic.h"
#include "games.h"
#include <LiquidCrystal_I2C.h>
const int ldrPin = A0;//The IR sensor is connected here. 
int lastValue = 0; //TODO I'm reasonably sure this should be in the loop

const int output_tick_size = 300;//This is/should be 1 over the gear ratio because that's the amount of time needed to get a perfect amount of data. //TODO rename this - it should be something like "Minium calculation time"
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 if needed


void updateLCD(int cadence, int kph, float distance_km) {//TODO work out how normal monitors show this information 
  lcd.setCursor(0, 0);
  lcd.print("Cad:");
  lcd.print(padn(cadence,3));
  lcd.print(" KPH:");
  lcd.print(padn(kph,3));
  lcd.setCursor(0, 1);
  lcd.print("KMs:");
  lcd.print(distance_km, 2);
  lcd.print("km");
}

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Vicious Cycle V3.1");
  
  Serial.println("Vicious Cycle V3.1");
  playHelloSound(8);
  lcd.clear();
  //run_startup_tests();
}

void run_startup_tests(){ //this should probably be named unique to this file 
Serial.println("Starting Tests");
test(1,1, "Basic");
test(1.0,1, "Float");
test(round(get_distance(-1)),0,"Distance -1");
test(round(get_distance(0)),0,"Distance 0");
test(round(get_distance(1000000)),35,"One million changes");
test(round(get_distance(10000000)),351,"ten million changes");
test(round(get_distance(100000000)),3509,"100 million changes");
test(calculateKPH(0),0,"KPH 0");
test(calculateKPH(-1),0,"KPH -1");
test(round(calculateKPH(100)),42,"KPH 100");
test(round(calculateKPH(10)),4,"KPH 10");
test(round(calculateKPH(10000)),4218,"KPH 10000");
}


bool test(float a, float b, String test){
  //It would be cool if we could switch from Serial print to a terminal print when running locally
  if (a==b){
    Serial.println("Pass: "+test);
    return true;
  } else {
    Serial.print("Fail: "+test +"(");
    Serial.print(a,8);
    Serial.print(")(");
    Serial.print(b,8);
    Serial.println(")");
    return false;
  }
}

void loop() {
  int currentValue = detectColor(analogRead(ldrPin));
  
  static unsigned long total_readings = 0;//TODO this can be part of a debugging system 
  static unsigned int readings_since_change=0;
  total_readings++;
  if (currentValue != lastValue) {
    readings_since_change=0;
    lastValue = currentValue;
    segmentPassed();
  }
  readings_since_change++;
  if (readings_since_change>200){
    lcd.clear();
  }
  delay(1);//TODO see what changing this does to the readings_per_change

}

/* This function triggers every time the sensor notices a new event. It needs no arguments*/ 
void segmentPassed(){
  //TODO the 'changest since last' and 'tick time' should be part arrays.  A rolling system of 'these happend when

  static unsigned long lastCheckTime = 0;
  static unsigned long changes = 0;
  static unsigned int changesSinceLastTick = 0;
  unsigned long currentTime = millis();
  changes++;
  changesSinceLastTick++;

  if (currentTime - lastCheckTime >= output_tick_size) {
    updateDisplay(changesSinceLastTick,changes);
    uncharted(changesSinceLastTick);
    changesSinceLastTick = 0;
    lastCheckTime = currentTime;
  }
}

void updateDisplay(int cadence, long changes){
    int kph=calculateKPH(cadence);
    float distance_km=get_distance(changes);
    updateLCD(cadence, kph, distance_km);
}