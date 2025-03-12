#include "ModbusRtu.h"
#include "HardwareSerial.h"

#define VALUE_B 65536
Modbus slave(1, Serial1, 0);

const int max_data = 100;
uint16_t got_data[max_data];
uint32_t sum_data;
String hex_, fristPart, secondPart, Lot_num, Lot_ttl;
long ascii_1, ascii_2;

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
  // Serial1.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 18, 17); // Key

  slave.start();
}

void loop() {
  slave.poll(got_data, max_data);

  for (int i = 0; i < sizeof(def_tb) / sizeof(def_tb[0]);i++) {
    def_tb[i][3] = got_data[(def_tb[i][1].toInt()) - 1];
    Serial.println(def_tb[i][3]);
  }
  // delay(500);
}