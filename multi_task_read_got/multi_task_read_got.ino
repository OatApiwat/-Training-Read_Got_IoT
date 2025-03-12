#include <WiFi.h>
#include <PubSubClient.h>
#include "ModbusRtu.h"
#include "HardwareSerial.h"
#include "ArduinoJson.h"

Modbus slave(1, Serial1, 0);

const int max_data = 100;
uint16_t got_data[max_data];

const char* ssid = "MIC_Iot";
const char* password = "Micdev@2024";
const char* mqtt_server = "192.168.0.166";  // My IP Broker in mqtt

WiFiClient espClient;
PubSubClient client(espClient);

const char* topic_pub_1 = "Test/sent/demo";
const char* topic_pub_2 = "Test/sent/demo";
const char* mc_no = "OAT";



// สร้างตัวแปรเก็บค่า Tick ที่แปลงจากเวลาที่เป็น millisecond
// const TickType_t xDelay2000ms = pdMS_TO_TICKS(2000);
// สร้างตัวแปรเก็บค่าเป็น integer เพื่อเอาไปใช้ใน Task
TaskHandle_t Task1 = NULL;  // จองพื้นที่ของการทำงานในแต่ละ task ให้เป็นว่าง เพราะไม่มีอะไรให้ Handle
TaskHandle_t Task2 = NULL;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32_oat";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // client.subscribe(topic_sub_1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

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
String def_tb[][5] = {
  // name||address||type||value||prv_value
  // type for separate detail of data
  { "Alarm_1", "11", "2", "", "" },
  { "Alarm_2", "12", "2", "", "" },
  { "Alarm_3", "13", "2", "", "" },
  { "Alarm_4", "14", "2", "", "" },
  { "Alarm_5", "15", "2", "", "" },
  { "Data_1", "21", "3", "", "" },
  { "Data_2", "22", "3", "", "" },
  { "Data_3", "23", "3", "", "" },
  { "Data_4", "24", "3", "", "" },
  { "Data_5", "25", "3", "", "" },
  { "RUN", "1", "1", "", "" },
  { "STOP", "2", "1", "", "" },
  { "Alarm", "3", "1", "", "" },

};
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 18, 17);  // Key

  slave.start();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);

  delay(1000);
  xTaskCreatePinnedToCore(modbus_task, "Task1", 10000, NULL, 1, NULL, 0);  // ลำดับความสำคัญเลขมากก่อน
  xTaskCreatePinnedToCore(func2_Task, "Task2", 10000, NULL, 2, NULL, 0);
  //xTaskCreatePinnedToCore(func3_Task,"Task3",5000,NULL,2,&Task3,0);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
}

void modbus_task(void* pvParam) {
  while (1) {  // Run loop ถ้า while เป็นจริง
    slave.poll(got_data, max_data);

    for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]); i++) {
      def_tb[i][3] = got_data[(def_tb[i][1].toInt()) - 1];
      Serial.println(def_tb[i][3]);
    }
    vTaskDelay(pdMS_TO_TICKS(500));  //จะวิ่งทุกๆ 2000ms ตามกำหนด wait 1.5 ms
  }
}
void func2_Task(void* pvParam) {
  char topic_pub[50];
  strcpy(topic_pub, topic_pub_1);
  strcat(topic_pub, mc_no);
  while (1) {
    StaticJsonDocument<200> T1;  //https://arduinojson.org/v6/assistant/#/step3
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
