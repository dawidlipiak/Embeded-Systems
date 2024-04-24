#include <Arduino.h>

#ifndef TimerInterrupt_hpp
#define TimerInterrupt_hpp

class TimerInterrupt {
private:
  enum State{ON, OFF};
  unsigned long previousMillis = 0;
  long interval;
  State state = State::OFF;

public:
  TimerInterrupt(long period);
  bool periodIterruptCheck();
  bool periodIterruptCheck(long period);
  long getInterruptInterval();
  void setState(State state);
  TimerInterrupt::State getState();
};

#endif