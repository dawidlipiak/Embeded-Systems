#include "Beeper.h"

static Beeper* _beeper = nullptr;

Beeper::Beeper(int pin) : ledPin(pin) {}

void Beeper::init() {
  pinMode(this->ledPin, OUTPUT);
	_beeper = this;
}

static void Beeper::doBeep() {
	digitalWrite(_beeper->ledPin, digitalRead(_beeper->ledPin) ^ 1);
}

void Beeper::turnOffBeeper(){

}

void Beeper::turnOnBeeper(int speed) {
  if (speed == 0) {
    return;
  }
  uint16_t beepPeriod = this->timerInterrupt.getInterruptInterval() / (float)(speed/100);
  if(this->timerInterrupt.periodIterruptCheck(beepPeriod)) {
    this->doBeep();
  }
}
