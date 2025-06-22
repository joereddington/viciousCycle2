#include <Wire.h>
#include "bike_logic.h"
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>
const int ldrPin = A0;//The IR sensor is connected here. 
const int segments_per_revolution = 8;
int lastValue = 0;
const float gear_ratio=4.3;

const int output_tick_size = 300;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 if needed

void updateLCD(float cadence, float kph, float distance_km) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CAD:");
  lcd.print(cadence, 0);
  lcd.print(" KPH:");
  lcd.print(kph, 1);
  lcd.setCursor(0, 1);
  lcd.print("Dist:");
  lcd.print(distance_km, 2);
  lcd.print("km");
}


void updateSerial(unsigned long changes,
                  float readings_per_change,
                  float rps,
                  float rpm,
                  float cadence,
                  float kph,
                  float distance_km,
                  float averageKPH) {
  Serial.print("{");
  Serial.print("\"changes\":"); Serial.print(changes); Serial.print(",");
  Serial.print("\"readings_per_change\":"); Serial.print(readings_per_change); Serial.print(",");
  Serial.print("\"rps\":"); Serial.print(rps, 1); Serial.print(",");
  Serial.print("\"rpm\":"); Serial.print(rpm, 1); Serial.print(",");
  Serial.print("\"cadence\":"); Serial.print(cadence, 1); Serial.print(",");
  Serial.print("\"kph\":"); Serial.print(kph, 2); Serial.print(",");
  Serial.print("\"distance_km\":"); Serial.print(distance_km, 3); Serial.print(",");
  Serial.print("\"averageKPH\":"); Serial.print(averageKPH, 2);
  Serial.println("}");
}

void road_rash(int cadence){
if (cadence > 65){ //then acellerate 
      Keyboard.press('s');  
      Serial.print("gas");
      
             // Hold it for 50 milliseconds
    } else{
      Serial.print("stopping gas");
      Keyboard.release('s');
    }
if (cadence < 20){
      Keyboard.press('a');  
             // Hold it for 50 milliseconds
      Serial.print("break");
    } else{
      Keyboard.release('a');
      Serial.print("releasing break");
    }    

}

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
}

void loop() {
  //todo - lots of these shouldn't be static 
  static unsigned long startTime = millis();
  static unsigned int changes = 0;
  static unsigned int total_readings = 0;
  static unsigned int changesSinceLastTick = 0;
  static unsigned long lastCheckTime = 0;
  static float rps = 0.0;

  int currentValue = detectColor(analogRead(ldrPin));
  unsigned long currentTime = millis();
  total_readings++;

  if (currentValue != lastValue) {
    lastValue = currentValue;
    changes++;
    changesSinceLastTick++;
  }

  if (currentTime - lastCheckTime >= output_tick_size) {
    //todo factor this into a separate, testable, set of functions 

    rps = (float)changesSinceLastTick / segments_per_revolution;
    float rpm = rps * 60.0 * (1000 / output_tick_size);
    float cadence = rpm / gear_ratio; 
    float kph = calculateKPH(cadence);
    float distance_km = get_distance(changes); 

    float totalTimeHours = (currentTime - startTime) / 3600000.0;
    float averageKPH = (totalTimeHours > 0) ? distance_km / totalTimeHours : 0;

    float readings_per_change = (changes > 0) ? (float)total_readings / changes : 0;
    road_rash(cadence);


    lastCheckTime = currentTime;
    changesSinceLastTick = 0;

    updateSerial(changes, readings_per_change, rps, rpm, cadence, kph, distance_km, averageKPH);
    //pcs();
    updateLCD(cadence, kph, distance_km);
  }

  delay(1);
}

