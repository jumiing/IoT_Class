#import <ArduinoBLE.h>

BLEService myService("180A");
BLEByteCharacteristic ledCharacteristic("2A57",BLERead | BLEWrite);
const int ledPin = LED_BUILTIN;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  if (!BLE.begin()) {
    Serial.println("BLE start error");
    while(1);
  }
  Serial.println("BLE good!");
  BLE.setLocalName("202015022하주민");
  BLE.setAdvertisingInterval(3200);


  myService.addCharacteristic(ledCharacteristic);
  ledCharacteristic.writeValue((byte)0x00);

  byte value;
  ledCharacteristic.readValue(value);
  Serial.println(value);

  BLE.setAdvertisedService(myService);
  BLE.addService(myService);
  BLE.advertise();

}

void loop() {
  // put your main code here, to run repeatedly:

  BLEDevice central = BLE.central();
  if(central) {
    Serial.print("central: ");
    Serial.println(central.address());
    while(central.connected()) {
      byte value;
      if(ledCharacteristic.written()) {
        ledCharacteristic.readValue(value);
        Serial.println(value);
        if (value) {
          digitalWrite(ledPin,HIGH);
        }
        else {
          digitalWrite(ledPin,LOW);
        }

      }
      delay(300);

    }
    central.disconnect();
  }
  delay(100);

}
