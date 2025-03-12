#include "Mic_mqtt.h"

void Mic_mqtt::setup_wifi() {
    delay(10);
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

    Serial.println("\nWiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void Mic_mqtt::reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32_oat";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            // client.subscribe(topic_sub_1);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void Mic_mqtt::init() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, 18, 17);
    slave.start();
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    delay(1000);
    xTaskCreatePinnedToCore(modbus_task, "Task1", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(func2_Task, "Task2", 10000, NULL, 2, NULL, 0);
}

void Mic_mqtt::run() {
  if (!client.connected()) {
    reconnect();
  }
}

void modbus_task(void* pvParam) {
    while (1) {
        slave.poll(got_data, max_data);

        for (int i = 0; i < (sizeof(def_tb) / sizeof(def_tb[0])); i++) {
            int index = def_tb[i][1].toInt();
            if (index > 0 && index <= max_data) {
                def_tb[i][3] = got_data[index - 1];
                Serial.println(def_tb[i][3]);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void func2_Task(void* pvParam) {
    char topic_pub[50];
    snprintf(topic_pub, sizeof(topic_pub), "%s%s", topic_pub_1, mc_no);

    while (1) {
        StaticJsonDocument<200> T1;
        T1["Data_1"] = def_tb[5][3];
        T1["Data_2"] = def_tb[6][3];
        T1["Data_3"] = def_tb[7][3];
        T1["Data_4"] = def_tb[8][3];
        T1["Data_5"] = def_tb[9][3];

        String jsonString;
        serializeJson(T1, jsonString);
        client.publish(topic_pub, jsonString.c_str());

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
