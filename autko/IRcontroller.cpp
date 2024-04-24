#include "IRcontroller.hpp"

IRcontroller::IRcontroller(){}

void IRcontroller::validatePIN() {
  Serial.println("Provide PIN to unlock the car");
  while( !this->pinCorrect ) {
    // Odbiór kodu PIN
    if (IrReceiver.decode()) {
      uint16_t IRcommand = IrReceiver.decodedIRData.command;

      if (IRcommand != IR_Button_ENTER) {
        char charOfIRbutton = this->getCharOfIRbutton(IRcommand);
        receivedPIN[this->pinIndex++] = charOfIRbutton;
        Serial.print(charOfIRbutton);

        if (pinIndex == PIN_LENGTH) {
          receivedPIN[PIN_LENGTH] = '\0';  // Dodaj znak końca ciągu znaków
          this->pinIndex = 0;
          memset(receivedPIN, 0, sizeof(receivedPIN));  // Wyczyść tablicę receivedPIN
        }
      }
      else {
        Serial.println();
        if (strcmp(receivedPIN, this->PIN_CODE) == 0) {
          Serial.println("Correct PIN entered.");
          this->pinCorrect = true;
        } else {
          Serial.println("Incorrect PIN.");
        }
        this->pinIndex = 0;
        memset(receivedPIN, 0, sizeof(receivedPIN));  // Wyczyść tablicę receivedPIN
      }

      // Resetowanie dekodera sygnałów IR
      IrReceiver.resume();
    }
  }
}

char IRcontroller::getCharOfIRbutton(uint16_t IRcommand) {
  switch (IRcommand){
    case IR_Button_0:
      return '0';
    case IR_Button_1:
      return '1';
    case IR_Button_2:
      return '2';
    case IR_Button_3:
      return '3';
    case IR_Button_4:
      return '4';
    case IR_Button_5:
      return '5';
    case IR_Button_6:
      return '6';
    case IR_Button_7:
      return '7';
    case IR_Button_8:
      return '8';
    case IR_Button_9:
      return '9';
  }
  return NULL;
}