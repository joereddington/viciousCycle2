#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

//#include <Keyboard.h>
#include "bike_logic.h"

void playAlarmSound(int pin);
void playHelloSound(int pin);

void uncharted(int cadence);

#ifdef LEONARDO
void road_rash(int cadence);
#endif

#endif