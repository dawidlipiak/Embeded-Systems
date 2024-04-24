#include "Arduino.h"
#include "timerInterrupt.hpp"

#ifndef Beeper_h
#define Beeper_h

class Beeper{
public:
	Beeper(int pin);
  void init();
	static void doBeep();
	void turnOffBeeper();
	void turnOnBeeper(int speed);

private:
	int ledPin;
  TimerInterrupt timerInterrupt = TimerInterrupt(500);
};

#endif

