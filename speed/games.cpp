#include "games.h"

unsigned long lastAlarmTime = 0;  // Track when last alarm played


void playAlarmSound(int pin) {//TODO EASY this can be more complex and interesting
  tone(pin, 1000, 250);  // Play a single 250 ms tone at 1kHz
  lastAlarmTime = millis();  // Record the time the tone was played
}
/*
void playBabyShark(int pin){
 int start_feq=2000;
 float note_to_note=1.09;
 int a=1000;
 int b= (int) a*note_to_note;
 int c= (int) b*note_to_note;
 int d= (int) c*note_to_note;
 Serial.println(d,3);
 tone(pin,a,400);
 delay(400);
 tone(pin,b,400);
 delay(400);
 tone(pin,d,400);
 delay(400);sf
 tone(pin,d,200);
 delay(200);
 tone(pin,d,200);
 delay(200);
 tone(pin,d,200);
 delay(200);
 tone(pin,d,200);
 delay(200);
 tone(pin,d,200);
 delay(200);
 tone(pin,d,200);
 delay(200);
}
*/
// Friendly startup sound
void playHelloSound(int pin) {//TODO (EASY) make more complex and interesting 
  tone(pin, 2000, 150);  // first beep
  delay(200);
  tone(pin, 2400, 150);  // second, higher beep
  delay(200);
  //playBabyShark(pin);
  //an octave is twice the fequency when it starts as ends, and has eight notes, so it goes up in the fourth root of 2. 
}

void uncharted(int cadence) {//TODO rename 
  unsigned long currentTime = 0;//millis();
  if (cadence < 60 && (currentTime - lastAlarmTime >= 1000)) {//TODO make the 1000 and 60 adjustable 
    playAlarmSound(8);  // Only plays if 1 second has passed since last tone
  }
}

void road_rash(int cadence){//TODO - lots to do here - rewrite for clarity 
if (cadence > 65){ //then acellerate 
      static unsigned long lastPress = 0;
      Keyboard.press('s');
      Serial.println("gas+ ");
      if (cadence > 65 && millis() - lastPress > 2000) {
          Keyboard.release('s');
          //delay(50);  // tap duration
          Keyboard.press('s');
          lastPress = millis();//TODO this millis should be factored to a variable 
      
      }
    } else{
      Serial.println("gas-");
      Keyboard.release('s');
    }
if (cadence < 20){
      Keyboard.press('a');  
      Serial.println("break+");
    } else{
      Keyboard.release('a');
      Serial.println("break-");
    }    

}
