#include "sdModule.h"

bool SDcard::sdinit() {
    uint32_t t = millis();
    
    // Attempt to initialize the SD card
    if (!sd.begin(SD_CONFIG)) {
        Serial.println(F("\nSD initialization failed."));
        Serial.println(F("Possible issues:"));
        Serial.println(F("- Card not inserted properly."));
        Serial.println(F("- Incorrect SD_CS_PIN."));
        Serial.println(F("- Wiring or soldering issue."));
        if (isSpi(SD_CONFIG)) {
            Serial.println(F("Check for SPI device conflicts."));
        }
        return false;
    }

    t = millis() - t;
    Serial.print(F("SD initialization time: "));
    Serial.print(t);
    Serial.println(F(" ms"));

    // Attempt to read card metadata
    if (!sd.card()->readCID(&cid) ||
        !sd.card()->readCSD(&csd) ||
        !sd.card()->readSCR(&scr)) {
        Serial.println(F("Failed to read card metadata (CID, CSD, SCR)."));
        return false;
    }

    return true;
}

void SDcard::cidDmp() {
    Serial.println(F("\nCard Information:"));
    Serial.print(F("Manufacturer ID: "));
    Serial.println(cid.mid, HEX);

    Serial.print(F("OEM ID: "));
    Serial.print(cid.oid[0]);
    Serial.println(cid.oid[1]);

    Serial.print(F("Product: "));
    for (uint8_t i = 0; i < 5; i++) {
        Serial.print(cid.pnm[i]);
    }
    Serial.println();

    Serial.print(F("Revision: "));
    Serial.print(cid.prvN());
    Serial.print(F("."));
    Serial.println(cid.prvM());

    Serial.print(F("Serial number: "));
    Serial.println(cid.psn(), HEX);

    Serial.print(F("Manufacturing date: "));
    Serial.print(cid.mdtMonth());
    Serial.print(F("/"));
    Serial.println(cid.mdtYear());

    Serial.println();
}

void SDcard::csdDmp() {
    eraseSize = csd.eraseSize();
    
    Serial.print(F("Card Size: "));
    Serial.print(0.000512 * csd.capacity());
    Serial.println(F(" MB (1 MB = 1,000,000 bytes)"));

    Serial.print(F("Flash Erase Size: "));
    Serial.print(eraseSize);
    Serial.println(F(" blocks"));

    Serial.print(F("Supports single block erase: "));
    Serial.println(csd.eraseSingleBlock() ? F("Yes") : F("No"));

    Serial.print(F("Data after erase: "));
    Serial.println(scr.dataAfterErase() ? F("Ones") : F("Zeros"));
}

bool SDcard::directoryExists(const char* dirPath) {
    // Check if directory exists
    if (sd.exists(dirPath)) {
        Serial.print(F("Directory exists: "));
        Serial.println(dirPath);
        return true;
    } else {
        Serial.print(F("Directory does not exist: "));
        Serial.println(dirPath);
        return false;
    }
}

bool SDcard::createDirectory(const char* dirPath) {
    // Attempt to create a directory
    if (sd.mkdir(dirPath)) {
        Serial.print(F("Directory created successfully: "));
        Serial.println(dirPath);
        return true;
    } else {
        Serial.print(F("Failed to create directory: "));
        Serial.println(dirPath);
        return false;
    }
}
