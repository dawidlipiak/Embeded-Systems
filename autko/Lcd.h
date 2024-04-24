#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Wheels.h"

#ifndef Lcd_h
#define Lcd_h

class Lcd : LiquidCrystal_I2C {
public:
  Lcd(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);

  enum ArrowState {
    TOP,
    MID,
    BOTTOM,
    TOP_BOTTOM
  };

  void initialize();

  void clearLeft();
  void clearRight();

  void setMsgLeft(int speed, Wheels::Direction dir);
  void setMsgRight(int speed, Wheels::Direction dir);
  void setMsgCenter(int distance);

  void drawUpArrow(int column, ArrowState& arrowState);
  void drawDownArrow(int column, ArrowState& arrowState);

  void leftArrowUpAnimation();
  void rightArrowUpAnimation();
  void arrowUpAnimation();

  void leftArrowDownAnimation();
  void rightArrowDownAnimation();
  void arrowDownAnimation();

private:
  ArrowState arrowLeftState = ArrowState::BOTTOM;
  ArrowState arrowRightState = ArrowState::BOTTOM;
  TimerInterrupt timer_left = TimerInterrupt(420); 
  TimerInterrupt timer_right = TimerInterrupt(420); 
  int distance;

  uint8_t arrowUp[8] = {
      0b00000, // . . . . . 
      0b00100, // . . # . . 
      0b01110, // . # # # . 
      0b11111, // # # # # # 
      0b01110, // . # # # . 
      0b01110, // . # # # . 
      0b01110, // . # # # . 
      0b01110  // . # # # . 
  };

  uint8_t arrowUpPartUpper[8] = {
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00100, // . . # . .
      0b01110, // . # # # .
      0b11111  // # # # # #
  };

  uint8_t arrowUpPartBottom[8] = {
      0b01110, // . # # # .
      0b01110, // . # # # .
      0b01110, // . # # # .
      0b01110, // . # # # .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000  // . . . . .
  };

      uint8_t arrowDown[8] = {
      0b01110, // . # # # . 
      0b01110, // . # # # . 
      0b01110, // . # # # . 
      0b01110, // . # # # . 
      0b11111, // # # # # # 
      0b01110, // . # # # . 
      0b00100, // . . # . . 
      0b00000  // . . . . . 
  };

  uint8_t arrowDownPartBottom[8] = {
      0b11111, // # # # # # 
      0b01110, // . # # # . 
      0b00100, // . . # . . 
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000  // . . . . .
  };

  uint8_t arrowDownPartUpper[8] = {
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b00000, // . . . . .
      0b01110, // . # # # .
      0b01110, // . # # # .
      0b01110, // . # # # .
      0b01110  // . # # # .
  };
};

//void setup() {
//
//  Serial.begin(9600);
//  Serial.setTimeout(200);
//
//  lcd.init();
//  lcd.backlight();
//
//  lcd.createChar(0, arrowRight);
//}

//void loop() {
//
//  uint8_t barPos = 0;
//  
//  argIn = Serial.parseInt(SKIP_ALL);
//  if(argIn <= 0) return;
//
//  lcd.clear();
//  lcd.setCursor(barPos, 1);
//  lcd.write(0);
//
//  for(int val=0;val<=argIn; val++) {
//    lcd.setCursor(0,0);
//    lcd.print(val);
//    barPos = map(val, 0, argIn, 0, 16);
//    lcd.setCursor(barPos, 1);
//    lcd.print('=');
//    lcd.write(0);
//    delay(20);
//  }
//}

#endif
