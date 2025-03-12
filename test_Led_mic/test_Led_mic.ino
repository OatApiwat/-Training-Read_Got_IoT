#include "Led.h"
#define LED_PIN 1
Led led(LED_PIN);
void setup(){
  led.init();
}

void loop(){
  led.on();
  led.off();
}