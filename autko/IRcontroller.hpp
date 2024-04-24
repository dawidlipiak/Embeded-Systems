#ifndef IRcontroller_hpp
#define IRcontroller_hpp

#include <Arduino.h>
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>

// Definicje dla przycisków pilota IR
#define IR_Button_FORWARD 24
#define IR_Button_BACKWARD 82 
#define IR_Button_LEFT 8
#define IR_Button_RIGHT 90
#define IR_Button_ENTER 28
#define IR_Button_0 25
#define IR_Button_1 69
#define IR_Button_2 70
#define IR_Button_3 71
#define IR_Button_4 68
#define IR_Button_5 64
#define IR_Button_6 67
#define IR_Button_7  7
#define IR_Button_8 21
#define IR_Button_9 9

class IRcontroller {
public:
  IRcontroller();
  void validatePIN();
  char getCharOfIRbutton(uint16_t IRcommand);

private:
  // class IRbutton {
  //   public:
  //     uint16_t IRcode
  //     char symbol = '';
  //     IRbutton(uint16_t command) : IRcode(command) {};
  //     IRbutton(uint16_t command, char _symbol) : IRcode(command) : symbol(_symbol) {};
  // }

  // IRbutton irButton_ENTER(28);
  // IRbutton irButton_FORWARD(24);
  // IRbutton irButton_BACKWARD(82);
  // IRbutton irButton_LEFT(8);
  // IRbutton irButton_RIGHT(90);
  // IRbutton irButton_0(25,'0');  
  // IRbutton irButton_1(69,'1');
  // IRbutton irButton_2(70,'2');  
  // IRbutton irButton_3(71,'3');  
  // IRbutton irButton_4(68,'4');  
  // IRbutton irButton_5(64,'5');  
  // IRbutton irButton_6(67,'6');  
  // IRbutton irButton_7( 7,'7');  
  // IRbutton irButton_8(21,'8');  
  // IRbutton irButton_9(9,'9');  
  
#define PIN_LENGTH 4
  char* PIN_CODE = "1234";
  char receivedPIN[PIN_LENGTH + 1];  // +1 dla znaku końca ciągu znaków
  int pinIndex = 0;
  bool pinEntered = false;
  bool pinCorrect = false;

};

#endif