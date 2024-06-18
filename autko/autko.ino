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
#include <math.h>

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
  w.attach(12,8,6,2,4,5);
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


  // Czujnik szczelinowy
  PCICR  = 0x02;  // włącz pin change interrupt dla 1 grupy (A0..A5)
  PCMSK1 = 0x03;  // włącz przerwanie dla A0, A1'
  cnt0=0;
  cnt1=0;
  servo.write(90);
  delay(500);
  lookAndTellDistance(0);
  delay(500);
  w.setSpeed(130);
  w.forward();
  // IrController.validatePIN();
}

void loop() {
  w.manageBeeper();

  // IR PILOT HANDLER 
  if (IrReceiver.decode()) {
    uint16_t IR_command = IrReceiver.decodedIRData.command; 
    Serial.println(IR_command);
    switch(IR_command) {
        case IR_Button_FORWARD :
            w.setSpeedLeft(130);
            w.setSpeedRight(130);
            w.forward();
            servo.write(90);
            servoPosition = 90;
          break;
        case IR_Button_BACKWARD :
          w.setSpeed(130);
          w.back();
          break;
        case IR_Button_LEFT :
          turnByAngle(-90);
          servo.write(180);

          break;
        case IR_Button_RIGHT :
          turnByAngle(90);
          servo.write(0);
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

  // Avoid collision
  if(lookAndTellDistance(90) < 23){
    avoidCollision();
  }
  else {
    w.forward();
  }
}

// Zwiększ liczbę zobaczonych szczelin przez czujnik szczelinowy
ISR(PCINT1_vect){
 if( (PINC & (1 << PC0)) ) 
 cnt0++;

 if( (PINC & (1 << PC1)) )
 cnt1++;
}

void turnByAngle(int16_t angle) {
    // Resetujemy liczniki obrotów kół
    cnt0 = 0;
    cnt1 = 0;

    // Ustawiamy prędkości obrotowe kół dla skrętu w miejscu

    // Obliczamy odległość potrzebną do osiągnięcia żądanego kąta skrętu w miejscu
    double wheelDiameter = 66.0; // Średnica koła w mm
    double wheelCircumference = wheelDiameter * 3.14159; // Obwód koła
    double distanceToTravel = (abs(angle) / 360.0) * wheelCircumference; // Odległość do przejechania dla określonego kąta
    double turnFactor = 2.5;
    w.stop();
    w.setSpeed(130);
    // w.setSpeedLeft();
    // w.setSpeedRight();
    
    if(angle < 0 ){
      w.forwardRight();
      w.backLeft();
    }
    else {
      w.forwardLeft();
      w.backRight();
    } 
    // Oczekujemy na wykonanie odpowiedniej ilości obrotów kół, aby osiągnąć żądany kąt
    while ((cnt0 + cnt1) / 2 * (wheelDiameter / 21) < distanceToTravel*turnFactor) {
        // Serial.print("Cnt0: ");
        // Serial.print(cnt0);
        // Serial.print("Cnt1: ");
        // Serial.println(cnt1);
        // Oczekujemy na osiągnięcie kąta
    }
    
    w.stop();
}

// void turnLeft() {
//   w.stop();
//   w.setSpeed(140);
//   delay(150);
//   w.backLeft();
//   w.forwardRight();
//   delay(600);
//   w.stop();
//   delay(150);
// }

// void turnRight() {
//   w.stop();
//   w.setSpeed(140);
//   delay(150);
//   w.backRight();
//   w.forwardLeft();
//   delay(600);
//   w.stop();
//   delay(150);
// }

uint16_t lookAndTellDistance(byte angle) {
  
  unsigned long tot;      // czas powrotu (time-of-travel)
  uint16_t distance;

  Serial.print("Patrzę w kącie ");
  Serial.print(angle);
  servo.write(angle);
  servoPosition = angle;
  delay(50);

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

bool checkObjectCollision(byte angle, uint8_t distance){
  if(lookAndTellDistance(angle) < distance) {
    delay(500);
    Serial.println("Kolizja w ta strone = true");
    return true;
  }
  else {
    delay(500);
    return false;
  }
}

void avoidCollision() {
  w.stop();
  delay(200);
  int8_t angleAdjustment = 0;
  int8_t directionOfAvoidence = 1; // 1 if we go right, -1 if we go left

  if(checkObjectCollision(180, 40)) {
    directionOfAvoidence = 1;
  }
  if((checkObjectCollision(0, 40))) {
    directionOfAvoidence = -1;
  }
  servo.write(90);
  delay(300);
  turnByAngle(directionOfAvoidence * 90);
  delay(300);
  w.setSpeed(130);

 // Below might work or not

  // // Look to the side of the box
  // if(directionOfAvoidence < 0) {
  //   servo.write(0);
  //   servoPosition = 0;
  // }
  // else {
  //   servo.write(180);
  //   servoPosition = 180;
  // }

  // delay(400);
  // uint8_t servoPositionTemp = servoPosition;
  // servo.write(servoPositionTemp);

  // // Go until you pass the box or meet another one
  // while(lookAndTellDistance(servoPositionTemp) < 25){
  //   w.forward();
  //   // if(lookAndTellDistance(servoPositionTemp) < 8) {
  //   //   turnByAngle(directionOfAvoidence * 5);
  //   //   angleAdjustment +=  directionOfAvoidence * 5;
  //   // }
  //   delay(300);
  //   w.stop();
  //   delay(200);
    
  //   // Look ahead and check any object existans
  //   if(checkObjectCollision(90, 17)) {
  //     delay(200);
  //     avoidCollision();
  //   }
  //   delay(300);
  //   servo.write(servoPositionTemp);
  //   delay(300);
  //   w.forward();
  // }

  // delay(1000);
  // w.stop();
  // servo.write(90);
  // delay(200);
  // turnByAngle(-1 * (directionOfAvoidence * 90  + angleAdjustment));
  // delay(400);
  
  w.setSpeed(130);
  w.forward();
}

