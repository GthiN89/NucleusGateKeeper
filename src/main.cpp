#include <Arduino.h>
#include "SPI.h"
#include <SoftwareSerial.h>
#include "ELECHOUSE_CC1101_SRC_DRV.h"
#include "modules/SD/sdModule.h"
#include "main.h"
#include "modules/CC1101/CC1101.h"


SDcard sdcard;
CC1101_CLASS CC1101;

void error(const char* msg) {
  Serial.println(msg);
  while(true);
}

void setup() {
  Serial.begin(115200);
  
  bool sdInit = false;
  bool CC1101Init = false;
  
  if(sdcard.sdinit()){
      bool sdInit = true;
      sdcard.cidDmp();
      sdcard.csdDmp();
      gateKeeperState = STATE_INIT_FAIL;
  }

  if(sdInit) {
      CC1101Init = CC1101.init();
  }

  if(!CC1101Init){
    gateKeeperState = STATE_INIT_SUCESS;
    Serial.println(F("Gate keeper initialization has beed sucessfull"));
  } else {
    Serial.println(F("Gate keeper initialization failed"));
    Serial.println(F("Status of devices:"));
    Serial.println(F("SD card: "));
    if(sdInit) {
      Serial.print(F("sucess"));
    } else {
      Serial.println(F("SD card initialization failed. Pleasse check your SD card"));
    }
        Serial.println(F("CC1101 transceiver"));
    if(CC1101Init) {
      Serial.print(F("sucess"));
    } else {
      Serial.println(F("CC1101 transceiver failed. Pleasse check your CC1101 transceiver"));
    }
    gateKeeperState = STATE_INIT_FAIL;
  }

}

void loop() {
}
