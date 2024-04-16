#include <ArduinoBLE.h>

struct SensorData {
  uint16_t companyId = 0xffff;
  uint16_t deviceId = 0x1111;
  int16_t temp = 26;
  int16_t hum = 50;
};

SensorData sensorData;

void printSensorData(struct SensorData *data) {
  Serial.print("Company ID: ");
  Serial.println(data->companyId);
  Serial.print("Device ID: ");
  Serial.println(data->deviceId);
  Serial.println("---------------------");
  Serial.print("Temperature: ");
  Serial.println(data->temp);
  Serial.print("Humidity: ");
  Serial.println(data->hum);
}

uint8_t buf[100];

void bleHandler(BLEDevice peripheral){
  if (!peripheral.address().equals("08:b6:1f:82:10:4e")) {
    return;
  }
  int length = peripheral.getAdvertisement(buf, 100);
  memcpy(&sensorData, buf + 5, sizeof(SensorData));
  printSensorData(&sensorData);

}

void setup() {
  Serial.begin(9600);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("BLE error");
    while(1);
  }
  Serial.println(BLE.address());

  BLE.setEventHandler(BLEDiscovered, bleHandler);
  BLE.scan(true);
}

void loop() {
  BLE.poll();
}

