#define LED_PIN 1

class Led {
private:
  byte _pin;
public:
  Led(byte pin) {
    this->_pin = pin;
  }


  void init() {
    pinMode(_pin, OUTPUT);
    Serial.begin(115200);
  }

  void on() {
    digitalWrite(_pin, HIGH);
    Serial.println("ON");
    delay(1000);
  }

  void off() {
    digitalWrite(_pin, LOW);
    Serial.println("OFF");
    delay(2000);
  }
};

Led mic(LED_PIN);

void setup() {
  mic.init();
}
void loop() {
  mic.on();
  mic.off();
}