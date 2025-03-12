#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "MIC_Iot";
const char* password = "Micdev@2024";
const char* mqtt_server = "192.168.0.166"; // My IP Broker in mqtt

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long prv_time = 0;
unsigned long prv_time1 = 0;
const char* msg ="a";
const char* topic_pub_1 = "Test/sent/oat1";
const char* topic_pub_2 = "Test/sent/oat2";
const char* topic_sub_1 = "Test/#";
//String data1 = "AAA";
// สร้างตัวแปรเก็บค่า Tick ที่แปลงจากเวลาที่เป็น millisecond
const TickType_t xDelay2000ms = pdMS_TO_TICKS(2000);    // นับครั้งที่ CPU stemp คือกำหนดเวลาเป็นหน่วย ms
// สร้างตัวแปรเก็บค่าเป็น integer เพื่อเอาไปใช้ใน Task
TaskHandle_t Task1 = NULL;    // จองพื้นที่ของการทำงานในแต่ละ task ให้เป็นว่าง เพราะไม่มีอะไรให้ Handle
TaskHandle_t Task2 = NULL;
//TaskHandle_t Task3 = NULL;

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
    String clientId = "ESP32_oat";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(topic_sub_1);
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

  delay(1000);  
  xTaskCreatePinnedToCore(func1_Task,"Task1",5000,NULL,1,NULL,0); // ลำดับความสำคัญเลขมากก่อน
  xTaskCreatePinnedToCore(func2_Task,"Task2",5000,NULL,2,NULL,0);
  //xTaskCreatePinnedToCore(func3_Task,"Task3",5000,NULL,2,&Task3,0);
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  /* publish normal
    client.publish(topic_pub_1,"Hi,MIC");
    delay(2000); */

  // ทำใหม่ ให้มีประสิทธิภาพมากขึ้น
//   if (millis() - prv_time >= 2000) {

//     client.publish(topic_pub_1,"Hello");
//     prv_time = millis();
//   }
//  if (millis() - prv_time1 >= 5000) {

//     client.publish(topic_pub_2,"Hi");
//     prv_time1 = millis();
//   } 
}

void func1_Task(void *pvParam){
  while(1){ // Run loop ถ้า while เป็นจริง
      client.publish(topic_pub_1,"Hello1"); // Assume ทำงานเสร็จ 500 ms
      Serial.println("H1");
      vTaskDelay(pdMS_TO_TICKS(2000));  //จะวิ่งทุกๆ 2000ms ตามกำหนด wait 1.5 ms
    }    
  }
void func2_Task(void *pvParam){
    while(1){      
      client.publish(topic_pub_2,"Hello2"); // Assume ทำงานเสร็จ 500 ms
      Serial.println("H2");
      vTaskDelay(pdMS_TO_TICKS(1000));
    }    
  }
/*void func3_Task(void *pvParam){
  while(1){
      Serial.println(String("hello from Task3")); // String เพราเราจะส่งออกสายอักขระ แต่ถ้าส่งผ่าน MQTT ต้องทำแบบด้านบน
      vTaskDelay(xDelay2000ms);  
    }    
  } */
