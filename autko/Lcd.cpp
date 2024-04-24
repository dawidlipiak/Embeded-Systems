#include <Arduino.h>

#include "Lcd.h"

Lcd::Lcd(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows) {}

void Lcd::initialize() {
  this->init();
  this->backlight();
  this->setCursor(0, 0);
  this->print("Left");
  this->setCursor(11, 0);
  this->print("Right");
  this->createChar(0, this->arrowUp);
  this->createChar(1, this->arrowUpPartUpper);
  this->createChar(2, this->arrowUpPartBottom);
  this->createChar(3, this->arrowDown);
  this->createChar(4, this->arrowDownPartUpper);
  this->createChar(5, this->arrowDownPartBottom);
}

void Lcd::clearLeft() {
  for(int i = 0; i < 7; i++){
    setCursor(i,1);
    this->print(" ");  
  }
  for(int i = 4; i < 7; i++){
    setCursor(i,0);
    this->print(" ");
    setCursor(i,1);
    this->print(" ");   
  }
}

void Lcd::clearRight() {
  for(int i = 7; i < 16; i++){
    setCursor(i,1);
    this->print(" ");  
  }
  for(int i = 7; i < 11; i++){
    setCursor(i,0);
    this->print(" ");
    setCursor(i,1);
    this->print(" ");   
  }  
}

void Lcd::setMsgLeft(int speed, Wheels::Direction dir){
  if(speed != 0) {
    for(int i = 0; i < 4; i++){
      setCursor(i,1);
      this->print(" ");  
    }
    setCursor(0,1);
    if(dir == Wheels::Direction::BACKWARD){
      this->print("-");
    }
    setCursor(1,1);
    this->print(speed);
  }
}

void Lcd::setMsgRight(int speed, Wheels::Direction dir){
  if(speed != 0) {
    for(int i = 11; i < 16; i++){
      setCursor(i,1);
      this->print(" ");  
    }
    setCursor(11,1);
    if(dir == Wheels::Direction::BACKWARD){
      this->print("-");
    }
    setCursor(12,1);
    this->print(speed);
  }
}

void Lcd::drawUpArrow(int column, ArrowState& arrowState){
  setCursor(column,0);
  this->print(" ");
  setCursor(column,1);
  this->print(" ");

  if(arrowState == ArrowState::BOTTOM){
    arrowState = ArrowState::MID;
    setCursor(column,0);
    this->write(1);
    setCursor(column,1);
    this->write(2);
  }
  else if (arrowState == ArrowState::MID) {
    arrowState = ArrowState::TOP;
    setCursor(column,0);
    this->write(0);
  }
  else if (arrowState == ArrowState::TOP) {
    arrowState = ArrowState::TOP_BOTTOM;
    setCursor(column,0);
    this->write(2);
    setCursor(column,1);
    this->write(1);
  }
  else {
    arrowState = ArrowState::BOTTOM;
    setCursor(column,1);
    this->write(0);
  }
}

void Lcd::drawDownArrow(int column, ArrowState& arrowState){
  setCursor(column,0);
  this->print(" ");
  setCursor(column,1);
  this->print(" ");

  if(arrowState == ArrowState::TOP){
    arrowState = ArrowState::MID;
    setCursor(column,0);
    this->write(4);
    setCursor(column,1);
    this->write(5);
  }
  else if (arrowState == ArrowState::MID) {
    arrowState = ArrowState::BOTTOM;
    setCursor(column,1);
    this->write(3);
  }
  else if (arrowState == ArrowState::BOTTOM) {
    arrowState = ArrowState::TOP_BOTTOM;
    setCursor(column,1);
    this->write(4);
    setCursor(column,0);
    this->write(5);
  }
  else {
    arrowState = ArrowState::TOP;
    setCursor(column,0);
    this->write(3);
  }
}

void Lcd::leftArrowUpAnimation(){
  unsigned long currentMillis = millis();

  if (this->timer_left.periodIterruptCheck()) {
    drawUpArrow(4, arrowLeftState);
  }
}

void Lcd::rightArrowUpAnimation(){
  unsigned long currentMillis = millis();

  if (this->timer_right.periodIterruptCheck()) {
    drawUpArrow(10, arrowRightState);
  }
}

void Lcd::leftArrowDownAnimation(){
  unsigned long currentMillis = millis();

  if (this->timer_left.periodIterruptCheck()) {
    drawDownArrow(4, arrowLeftState);
  }
}

void Lcd::rightArrowDownAnimation(){
  unsigned long currentMillis = millis();

  if (this->timer_right.periodIterruptCheck()) {
    drawDownArrow(10, arrowRightState);
  }
}

void Lcd::arrowUpAnimation() {
  leftArrowUpAnimation();
  rightArrowUpAnimation();
}
void Lcd::arrowDownAnimation() {
  leftArrowDownAnimation();
  rightArrowDownAnimation();
}