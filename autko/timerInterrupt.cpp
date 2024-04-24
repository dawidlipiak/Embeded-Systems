#include "timerInterrupt.hpp"

TimerInterrupt::TimerInterrupt(long period) : interval(period) {}

bool TimerInterrupt::periodIterruptCheck() {
  unsigned long currentMillis = millis();

  if (currentMillis - this->previousMillis >= this->interval) {
    this->previousMillis = currentMillis;
    if(this->state == State::OFF){
      this->state = State::ON;
    }
    else {
      this->state = State::OFF;
    }
    return true;
  }
  this->state = State::OFF;
  return false;
}

bool TimerInterrupt::periodIterruptCheck(long period) {
  this->interval = period;
  unsigned long currentMillis = millis();

  if (currentMillis - this->previousMillis >= this->interval) {
    this->previousMillis = currentMillis;
    if(this->state == State::OFF){
      this->state = State::ON;
    }
    else {
      this->state = State::OFF;
    }
    return true;
  }
  return false;
}

long TimerInterrupt::getInterruptInterval() {
  return this->interval;
}

void TimerInterrupt::setState(State state) {
  this->state = state;
}
TimerInterrupt::State TimerInterrupt::getState() {
  return this->state;
}
