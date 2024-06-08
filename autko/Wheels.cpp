#include <Arduino.h>
#include "Wheels.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

Wheels::Wheels(Beeper& _beeper) : beeper(_beeper) {}


void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
    this->speedRight = s;
    this->manageBeeper();
}

void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
    this->speedLeft = s;
    this->manageBeeper();
}

void Wheels::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
    this->manageBeeper();
}

uint8_t Wheels::getLeftSpeed() {
  return this->speedLeft;
}
uint8_t Wheels::getRightSpeed() {
  return this->speedRight;
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
    this->changeLeftDirection(Direction::FORWARD);
}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
    this->changeRightDirection(Direction::FORWARD);
}

void Wheels::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
    this->changeLeftDirection(Direction::BACKWARD);
}

void Wheels::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
    this->changeRightDirection(Direction::BACKWARD);
}

void Wheels::forward()
{
    changeDirection(Direction::FORWARD);
    this->forwardLeft();
    this->forwardRight();
}

void Wheels::back()
{
    changeDirection(Direction::BACKWARD);
    this->backLeft();
    this->backRight();
}

void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
    this->changeLeftDirection(Direction::NONE);
}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
    this->changeRightDirection(Direction::NONE);
}

void Wheels::stop()
{
    this->changeDirection(Direction::NONE);   
    this->stopLeft();
    this->stopRight();
}

void Wheels::goForward(uint8_t cm)
{
    // this->forward();
    // this->setSpeed(100);
    // delay(cm*29);
    // this->stop();
    this->forward();
    this->setSpeed(100);
    // this->setSpeed(100);
}

void Wheels::goBackward(uint8_t cm)
{
    this->back();
    this->setSpeed(100);
    delay(cm*29);
    this->stop();
}

Wheels::Direction Wheels::getLeftDirection(){
  return leftWheelDirection;
}

Wheels::Direction Wheels::getRightDirection() {
  return rightWheelDirection;
}

void Wheels::changeLeftDirection(Direction dir) {
    leftWheelDirection = dir;
    this->manageBeeper();
}

void Wheels::changeRightDirection(Direction dir) {
    rightWheelDirection = dir;
    this->manageBeeper();
}

void Wheels::changeDirection(Direction dir) {
    leftWheelDirection = dir;
    rightWheelDirection = dir;
    this->manageBeeper();
}

void Wheels::manageBeeper() {
    if(leftWheelDirection == Direction::BACKWARD && rightWheelDirection == Direction::BACKWARD) {
      beeper.turnOnBeeper((speedLeft+speedRight)/2);
    }
    else {
      beeper.turnOffBeeper();
    }
}

// void Wheels::setTargetSpeed(uint8_t speed) {
//     targetSpeedLeft = speed - 6; // Dostosowanie prędkości lewego koła
//     targetSpeedRight = speed + 6; // Dostosowanie prędkości prawego koła
// }

// void Wheels::updateSpeed() {
//     double currentSpeedLeft = 0; // kod do odczytu prędkości z czujnika szczelinowego lewego koła
//     double currentSpeedRight = 0; // kod do odczytu prędkości z czujnika szczelinowego prawego koła

//     double outputLeft = pidLeft.compute(currentSpeedLeft);
//     double outputRight = pidRight.compute(currentSpeedRight);

//     setSpeedLeft(constrain(outputLeft, 0, 255));
//     setSpeedRight(constrain(outputRight, 0, 255));
// }
