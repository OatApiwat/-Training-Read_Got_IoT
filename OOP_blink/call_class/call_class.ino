#define LED_PIN 1

Led mic(LED_PIN);

void setup(){
  mic.init();

}
void loop(){
  mic.on();
  mic.off();
}