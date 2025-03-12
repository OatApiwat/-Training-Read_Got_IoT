#ifndef MIC_MQTT_H
#define MIC_MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ModbusRtu.h>
#include <ArduinoJson.h>
#include "HardwareSerial.h"
#include "Arduino.h"
#include "Config.h"

class Mic_mqtt {
private:
    const char* ssid = "MIC_Iot";
    const char* password = "Micdev@2024";
    const char* mqtt_server = "192.168.0.166";
    uint16_t mqtt_port = 1883;
    void setup_wifi();
    void reconnect();
    static constexpr int max_data = 100;
    uint16_t got_data[max_data];

    Modbus slave = Modbus(1, Serial1, 0);
    WiFiClient espClient;
    PubSubClient client = PubSubClient(espClient);

    TaskHandle_t Task1 = NULL;  // Handle สำหรับ Task 1 (Modbus)
    TaskHandle_t Task2 = NULL;  // Handle สำหรับ Task 2 (MQTT)

public:
    // Mic_mqtt(const char* ssid, const char* password, const char* mqtt_server, const char* mqtt_port)
    //     : ssid(ssid), password(password), mqtt_server(mqtt_server), mqtt_port(mqtt_port) {}

    void init();
    void run();
};

#endif // MIC_MQTT_H
