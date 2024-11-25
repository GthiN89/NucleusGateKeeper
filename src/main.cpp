#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "ELECHOUSE_CC1101_SRC_DRV.h"
#include "main.h"
#include "modules/CC1101/CC1101.h"
#include <SoftwareSerial.h>
#include "modules/SerialCom/serialCom.h"


#define TINY_GSM_MODEM_SIM800

HardwareSerial SerialGSM(1);  // RX, TX
// Define SPI pins
#define SD_MOSI 23
#define SD_MISO 19
#define SD_CLK  18
#define SD_CS   5

// State enumeration


// Range to attempt to autobaud
// NOTE:  DO NOT AUTOBAUD in production code.  Once you've established
// communication, set a fixed baud rate using modem.setBaud(#).
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 57600

#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_TEST_WIFI false
#define TINY_GSM_TEST_TCP true
#define TINY_GSM_TEST_SSL true
#define TINY_GSM_TEST_CALL true
#define TINY_GSM_TEST_SMS true
#define TINY_GSM_TEST_USSD true
#define TINY_GSM_TEST_BATTERY true
#define TINY_GSM_TEST_TEMPERATURE true
#define TINY_GSM_TEST_GSM_LOCATION true
#define TINY_GSM_TEST_GPS true
#define TINY_GSM_TEST_NTP true
#define TINY_GSM_TEST_TIME true
// disconnect and power down modem after tests
#define TINY_GSM_POWERDOWN true
#include <TinyGsmClient.h>


TinyGsm modem(SerialGSM);

KEEPER_STATE gateKeeperState;
CC1101_CLASS CC1101;
CC1101_PRESET  C1101preset;
serialCom mySerialCom;

void testFileOperations() {
    // Check if the test file exists
    if (SD.exists("/test.txt")) {
        Serial.println(F("File exists: /test.txt"));
    } else {
        Serial.println(F("File does not exist: /test.txt"));
    }

    // Create and write to the file
    Serial.println(F("Creating and writing to /test.txt..."));
    File file = SD.open("/test.txt", FILE_WRITE);
    if (file) {
        file.println("Hello, SD card!");
        file.println("Testing file write.");
        file.close();
        Serial.println(F("File written successfully."));
    } else {
        Serial.println(F("Failed to create /test.txt."));
    }

    // Read the file
    Serial.println(F("Reading /test.txt..."));
    file = SD.open("/test.txt", FILE_READ);
    if (file) {
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
        Serial.println(F("\nFile read successfully."));
    } else {
        Serial.println(F("Failed to open /test.txt for reading."));
    }

    // Delete the file
    Serial.println(F("Deleting /test.txt..."));
    if (SD.remove("/test.txt")) {
        Serial.println(F("/test.txt deleted successfully."));
    } else {
        Serial.println(F("Failed to delete /test.txt."));
    }

    // Test creating a directory
    Serial.println(F("Creating directory /testdir..."));
    if (SD.mkdir("/testdir")) {
        Serial.println(F("Directory /testdir created successfully."));
    } else {
        Serial.println(F("Failed to create directory /testdir."));
    }

    // Test removing a directory
    Serial.println(F("Removing directory /testdir..."));
    if (SD.rmdir("/testdir")) {
        Serial.println(F("Directory /testdir removed successfully."));
    } else {
        Serial.println(F("Failed to remove directory /testdir."));
    }

            if (SD.exists("/ReceivedCodes")) {
            Serial.println(F("Folder for saving codes exists."));
        } else if (SD.mkdir("/ReceivedCodes")) {
            Serial.println(F("Folder for saving codes created."));
        } else {
            gateKeeperState = STATE_SDCARD_EROOR;
            return;
        }
        SD.end();
}

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);
    mySerialCom.begin(115200, "ESP32_BT");
    while (!Serial) {
        delay(10); // Wait for Serial to initialize
    }

    // Configure SPI pins
    SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);

    Serial.println(F("Initializing SD card..."));

    // Try initializing the SD card
    if (!SD.begin(SD_CS)) {
        Serial.println(F("SD card initialization failed!"));
        return;
    }

    Serial.println(F("SD card initialized successfully."));

    testFileOperations(); // Optional for testing SD card

    bool CC1101Init = CC1101.init();
   if (CC1101Init) {
        gateKeeperState = STATE_INIT_SUCESS;
        delay(20);
        CC1101.setFrequency(433.92);
        delay(20);
        CC1101.setCC1101Preset(AM650);
        delay(20);
        CC1101.enableReceiver();
        delay(20);
        gateKeeperState = STATE_CAPTURE;
        delay(20);
        Serial.println("Gate keeper initialization successful.");
    } else {
        gateKeeperState = STATE_INIT_FAIL;
        Serial.println(F("CC1101 initialization failed."));
    }
 
}

void loop() {
    if (gateKeeperState == STATE_INIT_SUCESS) {
                CC1101.enableReceiver();
        delay(20);
        gateKeeperState = STATE_CAPTURE;
    }

    if (gateKeeperState == STATE_CAPTURE) {

//        Serial.print(digitalRead(CC1101_CCGDO0A));

        if (CC1101.CheckReceived()) {
            Serial.println(F("Signal received."));
            CC1101.signalanalyse();
            gateKeeperState = STATE_INIT_SUCESS;;
        }
    }



    if (gateKeeperState == STATE_SDCARD_EROOR) {
        Serial.println(F("SD card error detected. Restarting initialization..."));
        gateKeeperState = STATE_INIT_FAIL;
        delay(5000);
        setup(); // Re-run setup to retry
    }

    mySerialCom.update();
}


