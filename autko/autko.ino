#define WS_SENSOR0 A0 // Wheel speed sensors
#define WS_SENSOR1 A1

#define TRIG A2 // piny dla sonaru (HC-SR04)
#define ECHO A3

#define LEDPIN 13
#define IR_RECEIVE_PIN 7  // Pin podłączony do odbiornika IR
#define SERVO_PIN 3 // Pin servomechanizmu

#include <Servo.h>
#include <PinChangeInterrupt.h>
#include <IRremote.hpp>

#include "Wheels.h"
#include "Lcd.h"
#include "Beeper.h"
#include "IRcontroller.hpp"
#include "timerInterrupt.hpp"



Beeper beeper(LEDPIN);
Wheels w(beeper);
Lcd lcd(0x27, 16, 2);
IRcontroller IrController;
Servo servo;
TimerInterrupt timer_lcdSpeed(400);
uint16_t servoPosition = 0;

volatile int cnt0, cnt1;
volatile int prevCnt0, prevCnt1;

volatile char cmd;

void setup() {
  // put your setup code here, to run once:
  w.attach(8,12,6,4,2,5);
  servo.attach(SERVO_PIN);
  IrReceiver.begin(IR_RECEIVE_PIN, false);
  beeper.init();

  lcd.initialize();
  lcd.clearLeft();
  lcd.clearRight();

  Serial.begin(9600);
  Serial.println("Forward: WAD");
  Serial.println("Back: ZXC");
  Serial.println("Stop: S");

  pinMode(WS_SENSOR0, INPUT);
  pinMode(WS_SENSOR1, INPUT);
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls

  cnt0=0;
  cnt1=0;

  PCICR  = 0x02;  // włącz pin change interrupt dla 1 grupy (A0..A5)
  PCMSK1 = 0x03;  // włącz przerwanie dla A0, A1'
  servo.write(90);
  // IrController.validatePIN();
}

void loop() {
  w.manageBeeper();

  if (IrReceiver.decode()) {
    uint16_t IR_command = IrReceiver.decodedIRData.command; 
    Serial.println(IR_command);
    switch(IR_command) {
        case IR_Button_FORWARD :
            w.setSpeed(150);
            w.forward();
            servo.write(90);
            servoPosition = 90;
          break;
        case IR_Button_BACKWARD :
          w.setSpeed(150);
          w.back();
          break;
        case IR_Button_LEFT :
          servo.write(180);
          servoPosition = 180;
          w.setSpeed(150);
          w.backLeft();
          w.forwardRight();
          delay(850);
          w.stop();
          servo.write(90);
          servoPosition = 90;
          break;
        case IR_Button_RIGHT :
          servo.write(0);
          servoPosition = 0;
          w.setSpeed(150);
          w.backRight();
          w.forwardLeft();
          delay(850);
          w.stop();
          servo.write(90);
          servoPosition = 90;
          break;
        case IR_Button_ENTER :
          w.stop();
          break; 
      }
      IrReceiver.resume(); // Enable receiving of the next value
  }

  while(Serial.available())
  {
    cmd = Serial.read();
    Serial.println(cmd);
    switch(cmd)
    {
      case 'w': w.forward(); break;
      case 'x': w.back(); break;
      case 'a': w.forwardLeft(); break;
      case 'd': w.forwardRight(); break;
      case 'z': w.backLeft(); break;
      case 'c': w.backRight(); break;
      case 's': w.stop(); break;
      case '1': w.setSpeedLeft(75); break;
      case '2': w.setSpeedLeft(200); break;
      case '9': w.setSpeedRight(75); break;
      case '0': w.setSpeedRight(200); break;
      case '5': w.setSpeed(100); break;
    }
  }

  if (timer_lcdSpeed.periodIterruptCheck()) {
    // Serial.println((20.73*1000*(cnt0 - prevCnt0))/(sensorInterval*20));
    lcd.setMsgLeft((20.73*1000*(cnt0 - prevCnt0))/(timer_lcdSpeed.getInterruptInterval()*21), w.getLeftDirection());
    lcd.setMsgRight((20.73*1000*(cnt1 - prevCnt1))/(timer_lcdSpeed.getInterruptInterval()*21), w.getRightDirection());
    prevCnt0 = cnt0;
    prevCnt1 = cnt1;
  }

  if(w.getLeftDirection() == Wheels::Direction::FORWARD){
    lcd.leftArrowUpAnimation();
  }
  if(w.getRightDirection() == Wheels::Direction::FORWARD){
    lcd.rightArrowUpAnimation();
  }
  if(w.getLeftDirection() == Wheels::Direction::BACKWARD){
    lcd.leftArrowDownAnimation();
  }
  if(w.getRightDirection() == Wheels::Direction::BACKWARD){
    lcd.rightArrowDownAnimation();
  }
  if(w.getRightDirection() == Wheels::Direction::BACKWARD){
    lcd.rightArrowDownAnimation();
  }
  if(w.getLeftDirection() == Wheels::Direction::NONE){
    lcd.clearLeft();
  }
  if(w.getRightDirection() == Wheels::Direction::NONE){
    lcd.clearRight();
  }

  if(lookAndTellDistance(servoPosition) < 10){
    if(w.getLeftDirection() != Wheels::Direction::BACKWARD){
      servo.write(180);
      servoPosition = 180;
      turnRight();
      while(lookAndTellDistance(servoPosition) < 15){
        w.forward();
        delay(100);
      }
      turnLeft();
      w.forward();
      servo.write(90);
    }
  }
}

ISR(PCINT1_vect){
 if( (PINC & (1 << PC0)) ) 
 cnt0++;

 if( (PINC & (1 << PC1)) )
 cnt1++;
}

void turnLeft() {
  w.setSpeed(150);
  w.backLeft();
  w.forwardRight();
  delay(850);
  w.stop();
}

void turnRight() {
  w.setSpeed(150);
  w.backRight();
  w.forwardLeft();
  delay(850);
  w.stop();
}
uint16_t lookAndTellDistance(byte angle) {
  
  unsigned long tot;      // czas powrotu (time-of-travel)
  uint16_t distance;

  Serial.print("Patrzę w kącie ");
  Serial.print(angle);
  
/* uruchamia sonar (puls 10 ms na `TRIGGER')
 * oczekuje na powrotny sygnał i aktualizuje
 */
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

/* prędkość dźwięku = 340m/s => 1 cm w 29 mikrosekund
 * droga tam i z powrotem, zatem:
 */
  distance = tot/58;

  Serial.print(": widzę coś w odległości ");
  Serial.println(distance);
  return distance;
}


