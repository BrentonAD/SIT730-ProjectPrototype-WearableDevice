/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/brent/OneDrive/Documents/_UNIVERSITY/_MASTEROFAI/2022Tri1/SIT730-EmbeddedSystemDevelopment/Project/ParticleFirmware/WearableDevice/src/WearableDevice.ino"
/*
 * Project WearableDevice
 * Description: Firmware for wearable device, for alerting user when bar pitch is beyond a certain threshold
 * Author: Brenton Adey BrentonAD
 */
#include <math.h>

void setup();
void loop();
#line 8 "c:/Users/brent/OneDrive/Documents/_UNIVERSITY/_MASTEROFAI/2022Tri1/SIT730-EmbeddedSystemDevelopment/Project/ParticleFirmware/WearableDevice/src/WearableDevice.ino"
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler;

pin_t MOTOR_PIN = A1;

const size_t SCAN_RESULT_MAX = 30;
BleScanResult scanResults[SCAN_RESULT_MAX];

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  BLE.setScanTimeout(50);
  int count = BLE.scan(scanResults, SCAN_RESULT_MAX);
  for (int i = 0; i < count; i++){
    uint8_t buf[BLE_MAX_ADV_DATA_LEN];
    size_t len;
    len = scanResults[i].advertisingData.get(BleAdvertisingDataType::MANUFACTURER_SPECIFIC_DATA, 
                                             buf, BLE_MAX_ADV_DATA_LEN);
    if (len == 7) {
      // This is the dummy id and data type we set from the advertiser
      if (buf[0] == 0xff && buf[1] == 0xff && buf[2] == 0x42) {
        float barPitch;
        memcpy(&barPitch, &buf[3], 4);
        Log.info("Bar Pitch: %f", barPitch);
        if (abs(barPitch)>10 && abs(barPitch)<30){
          analogWrite(MOTOR_PIN, abs(barPitch)*8);
        }
        else if (abs(barPitch)>=30) {
          analogWrite(MOTOR_PIN, 255);
        }
        else {
          analogWrite(MOTOR_PIN, 0);
        }
      }
    }
  }
}