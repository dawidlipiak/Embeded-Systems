/* 
 * prosta implementacja klasy obsługującej 
 * silniki pojazdu za pośrednictwem modułu L298
 *
 * Sterowanie odbywa się przez:
 * 1)  powiązanie odpowiednich pinów I/O Arduino metodą attach() 
 * 2)  ustalenie prędkości setSpeed*()
 * 3)  wywołanie funkcji ruchu
 *
 * TODO:
 *  - zabezpieczenie przed ruchem bez attach()
 *  - ustawienie domyślnej prędkości != 0
 */


#include <Arduino.h>
#include "Beeper.h"
#include "timerInterrupt.hpp"

#ifndef Wheels_h
#define Wheels_h



class Wheels {
    public: 
        Wheels(Beeper& _beeper);

        enum Direction {
            FORWARD,
            BACKWARD,
            NONE
        };

        /*
         *  pinForward - wejście "naprzód" L298
         *  pinBack    - wejście "wstecz" L298
         *  pinSpeed   - wejście "enable/PWM" L298
         */
        void attachRight(int pinForward, int pinBack, int pinSpeed);
        void attachLeft(int pinForward, int pinBack, int pinSpeed);
        void attach(int pinRightForward, int pinRightBack, int pinRightSpeed,
                    int pinLeftForward, int pinLeftBack, int pinLeftSpeed);
        /*
         *  funkcje ruchu
         */
        void forward();
        void forwardLeft();
        void forwardRight();
        void back();
        void backLeft();
        void backRight();
        void stop();
        void stopLeft();
        void stopRight();
        void goForward(uint8_t);
        void goBackward(uint8_t);
        void changeLeftDirection(Direction dir);
        void changeRightDirection(Direction dir);
        void changeDirection(Direction dir);
        
        void manageBeeper();
        /*
         *  ustawienie prędkości obrotowej (przez PWM)
         *   - minimalna efektywna wartość 60
         *      może zależeć od stanu naładowania baterii
         */
        void setSpeed(uint8_t);
        void setSpeedRight(uint8_t);
        void setSpeedLeft(uint8_t);

        uint8_t getLeftSpeed();
        uint8_t getRightSpeed();

        Direction getLeftDirection();
        Direction getRightDirection();

    private: 
        int pinsRight[3];
        int pinsLeft[3];
        uint8_t speedLeft;
        uint8_t speedRight;
        Direction leftWheelDirection = Direction::NONE;
        Direction rightWheelDirection = Direction::NONE;
        Beeper& beeper;
};



#endif
