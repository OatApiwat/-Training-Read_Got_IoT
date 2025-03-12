#ifndef LED_H
#define LED_H
#include "Arduino.h"

class Led {
private:
  byte _pin;
public:
  Led(byte pin) {
    this->_pin = pin;
  }
  void init();
  void on();
  void off();
};
#endif