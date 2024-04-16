#include <ArduinoBLE.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!BLE.begin()) {
    Serial.println("BLE start error");
    while(1);
  }
  BLE.scanforUuid("180A");


}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice p=BLE.abailable();
  if(p) {
    Serial.println(p.localName());
    if(p.localName().equals("ㅏㅏㅏㅏㅏ")) {
      BLE.stopscan();
      if(p.connect()) {
        Serial.println("connected");
        //여기서 이용

        if(!p.discoverAttributes()) {
          p.disconnect();
          while(1);

        }
        BLEService remoteS = p.service("180A");
        BLECharacteristic ledC = remoteS.characteristic("2A57");
        byte v;
        while(1){
          v = ledC.readValue();
          Serial.println(v);
          ledC.writeValue((byte)0x00);
          ledC.readValue(v);
          Serial.println(v);
          Serial.println(v);
          ledC.writeValue((byte)0x01);
          ledC.readValue(v);
          Serial.println(v);
          delay(1000);

        }

        p.disconnect();
        BLE.scanforUuid("180A");
      }
      else {
        Serial.println("connect failed");

      }
    }


  }
  delay(100);

}
