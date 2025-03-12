#include "Led.h"


void Led_mic::init() {
  pinMode(_pin, OUTPUT);
  Serial.begin(115200);
}

void Led_mic::on() {
  digitalWrite(_pin, HIGH);
  Serial.println("ON");
  delay(1000);
}

void Led_mic::off() {
  digitalWrite(_pin, LOW);
  Serial.println("OFF");
  delay(2000);
}
