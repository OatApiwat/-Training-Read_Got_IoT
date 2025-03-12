#define LED_PIN 1
#include "Led.h"

Led mic(LED_PIN);

void setup() {
  mic.init();
}
void loop() {
  mic.on();
  mic.off();
}