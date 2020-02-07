#include <Arduino.h>
#include <IoTSense.h>

IoTSense iotSense;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  iotSense.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  iotSense.run();
}