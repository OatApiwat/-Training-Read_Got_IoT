#include "Led.h"


void Led::init() {
  pinMode(_pin, OUTPUT);
  Serial.begin(115200);
}

void Led::on() {
  digitalWrite(_pin, HIGH);
  Serial.println("ON");
  delay(1000);
}

void Led::off() {
  digitalWrite(_pin, LOW);
  Serial.println("OFF");
  delay(2000);
}
