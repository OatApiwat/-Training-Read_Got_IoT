#include <WiFi.h>
#include <PubSubClient.h>
#include "ArduinoJson.h"
const char* ssid = "MIC_Iot";
const char* password = "Micdev@2024";
const char* mqtt_server = "192.168.0.166";  // My IP Broker in mqtt

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long prv_time = 0;
unsigned long prv_time1 = 0;
const char* msg = "a";
const char* topic_pub_1 = "Test/sent/oat1";
const char* topic_pub_2 = "Test/sent/oat2";
const char* topic_sub_1 = "Test/Rx/#";
//String data1 = "AAA";

void setup_wifi() {

  delay(10);  // delay unit ms
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //client.subscribe(topic_sub_1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  //publish normal
  if (millis() - prv_time > 2000) {
    prv_time = millis();
    StaticJsonDocument<200> T1; //https://arduinojson.org/v6/assistant/#/step3
    T1["sensor"] = "temperature"; 
    T1["value"] = 23.5;
    T1["unit"] = "C";
    JsonArray array = T1.createNestedArray("cars");
    array.add("toyota");
    array.add("yamaha");
    String jsonString;
    serializeJson(T1,jsonString);
    client.publish(topic_pub_1, jsonString.c_str());
  }
}
