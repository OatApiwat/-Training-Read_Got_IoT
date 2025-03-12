#define LED_PIN 1
void setup(){
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}
void loop(){
  digitalWrite(LED_PIN, HIGH);
  Serial.println("ON");
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  Serial.println("OFF");
  delay(2000);
}