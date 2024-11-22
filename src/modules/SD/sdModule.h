#ifndef SD_HEADER
#define SD_HEADER

#include "Arduino.h"
#include "SdFat.h"
#include "sdios.h"

#define SD_MOSI 23
#define SD_MISO 19
#define SD_CLK  18
#define SD_CS   5

const int8_t DISABLE_CS_PIN = -1;
const uint8_t SD_CS_PIN = SD_CS;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(4))


static ArduinoOutStream cout(Serial);


class SDcard {
    public:
        SdFs sd;
        cid_t cid;
        csd_t csd;
        scr_t scr;
        uint8_t cmd6Data[64];
        uint32_t eraseSize;
        uint32_t ocr;

        bool sdinit();
        void cidDmp(); 
        void csdDmp();

    private:

};

#endif

