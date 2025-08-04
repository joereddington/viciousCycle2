#include <Wire.h>
#include "bike_logic.h"
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>
const int ldrPin = A0;//The IR sensor is connected here. 
int lastValue = 0;
const float gear_ratio=3.34;//I worked this out by doing 100 crank revolutions and checking the number of main revolutions recorded

const int output_tick_size = 200;//changing this causes strange probelms 
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 if needed

void updateLCD(float cadence, float kph, float distance_km) {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CAD:");
  lcd.print(cadence, 0);
  lcd.print(" KPH:");
  lcd.print(kph, 1);
  lcd.setCursor(0, 1);
  lcd.print("Dist:");
  lcd.print(distance_km, 3);
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

unsigned long lastAlarmTime = 0;  // Track when last alarm played

void playAlarmSound(int pin) {
  tone(pin, 1000, 250);  // Play a single 250 ms tone at 1kHz
  lastAlarmTime = millis();  // Record the time the tone was played
}

void uncharted(int cadence) {
  unsigned long currentTime = millis();
  
  if (cadence < 60 && (currentTime - lastAlarmTime >= 1000)) {
    playAlarmSound(8);  // Only plays if 1 second has passed since last tone
  }
}

void road_rash(int cadence){
if (cadence > 65){ //then acellerate 
      static unsigned long lastPress = 0;
      Keyboard.press('s');
      Serial.print("gas+ ");
      if (cadence > 65 && millis() - lastPress > 2000) {
          Keyboard.release('s');
          //delay(50);  // tap duration
          Keyboard.press('s');
          lastPress = millis();
      
      }
    } else{
      Serial.print("gas-");
      Keyboard.release('s');
    }
if (cadence < 20){
      Keyboard.press('a');  
      Serial.print("break+");
    } else{
      Keyboard.release('a');
      Serial.print("break-");
    }    

}

// Friendly startup sound
void playHelloSound(int pin) {
  tone(pin, 2000, 150);  // first beep
  delay(200);
  tone(pin, 2400, 150);  // second, higher beep
  delay(200);
}

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  playHelloSound(8);
}

void loop() {
  //todo - lots of these shouldn't be static 
  static unsigned long startTime = millis();
  static unsigned long changes = 0;
  static unsigned long total_readings = 0;
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
    uncharted(cadence);


    lastCheckTime = currentTime;
    changesSinceLastTick = 0;

    updateSerial(changes, readings_per_change, rps, rpm, cadence, kph, distance_km, averageKPH);
    //pcs();
    updateLCD(cadence, kph, distance_km);
  }

  delay(1);
}

