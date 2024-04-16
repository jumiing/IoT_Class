#include <ArduinoBLE.h>
#include <DHT.h>
#define DHTTYPE DHT11

struct SensorData {
  uint16_t companyId = 0xffff; 
  uint16_t deviceId = 0x1234;
  int16_t temp;
  int16_t hum;
  int16_t lumin;
  int8_t sw;
};

SensorData sensorData;
int humS = 2;
int swS = 3;
int luminS = A1;

DHT dht(humS, DHTTYPE);

void printSensorData(struct SensorData *data) {
  Serial.println("---------------------");
  Serial.print("Temperature: ");
  Serial.println(data->temp);
  Serial.print("Humidity: ");
  Serial.println(data->hum);
  Serial.print("light: ");
  Serial.println(data->lumin);
  Serial.print("switch: ");
  if (data->sw==0)
    Serial.println("OFF");
  else
    Serial.println("ON");
}

void setup() {
  Serial.begin(9600);
  BLE.setLocalName("202015022하주민");
  dht.begin();
  pinMode(humS, INPUT);
  pinMode(swS, INPUT_PULLUP);
  pinMode(luminS, INPUT);
  sensorData.sw = 0;
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1);
  }
  BLE.setConnectable(false);
  BLE.setAdvertisingInterval(8000);
  String address = BLE.address();
  Serial.print("Local address is: ");
  Serial.println(address);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensorData.temp = dht.readTemperature();
  sensorData.hum= dht.readHumidity();
  sensorData.lumin=analogRead(luminS);
  if (!digitalRead(swS)) {
    sensorData.sw = !sensorData.sw;
  }

  BLEAdvertisingData advData;
  advData.setManufacturerData((uint8_t *)&sensorData, sizeof(sensorData));
  BLE.setAdvertisingData(advData);

  printSensorData(&sensorData);

  BLE.advertise();
  delay(1);
  BLE.stopAdvertise();
  delay(1000);
}