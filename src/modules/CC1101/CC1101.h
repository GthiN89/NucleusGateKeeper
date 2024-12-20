#ifndef C1101_H
#define C1101_H


#include "SPI.h"
#include "ELECHOUSE_CC1101_SRC_DRV.h"
#include "RCSwitch.h"
#include "main.h"

#include <sstream>
#include <ctime>
#include <vector>
#include <string> 

#define SAMPLE_SIZE 512

// Pin configuration for CC1101
#define CC1101_CS   14  // Chip Select
#define CC1101_MOSI 13   // Master Out Slave In
#define CC1101_MISO 12  // Master In Slave Out
#define CC1101_SCLK 27  // Serial Clock

#define CC1101_CCGDO0A 33      // GDO0



   //---------------------------------------------------------------------------//
  //-----------------------------ENUMBS----------------------------------------//
 //---------------------------------------------------------------------------//

// C1101 presets




// C1101 state mashine

// Current State
extern uint8_t C1101CurrentState;

  ////////////////////
 ////////FLAGS///////
////////////////////

extern bool CC1101_init;
extern bool CC1101_is_initialized;
extern bool CC1101_recieve_is_running;
extern bool CC1101_transmit_is_running;
extern bool CC1101_isiddle;
extern bool CC1101_interup_attached;

extern bool receiverEnabled;

   ////////////////////
  /////non class//////
 //////variables/////
////////////////////

//Interrupt data

extern int sample[SAMPLE_SIZE];
extern int error_toleranz;
extern int samplecount;
extern int receiverGPIO;

//AM650 is default preset 433.92 is default frequency
extern float CC1101_DRATE;
extern float CC1101_RX_BW;
extern float CC1101_DEVIATION;
extern int CC1101_PKT_FORMAT;
extern int CC1101_SYNC;
extern float CC1101_FREQ;
extern int CC1101_MODULATION;
extern float CC1101_MHZ;
extern String rawString;
class CC1101_CLASS {
public:


    //methods
    bool init();
    void setCC1101Preset(CC1101_PRESET preset);
    void enableReceiver();
    void signalanalyse();
    bool CheckReceived(void);
    void setFrequency(float freq);
    String generateFilename(float frequency, int modulation, float bandwidth);
    String generateRandomString(int length);
    String getPresetSettingsString();
    String getFrequeSettingsString();
    const char* presetToString(CC1101_PRESET preset);
    CC1101_PRESET convert_str_to_enum(const char * selected_str);
    bool isValidPreset(CC1101_PRESET preset);
    void loadPreset(CC1101_PRESET preset);
    void setCC1101DCcorrection(int v);
 


private:

    //classes
    SPIClass CC1101SPI;
    RCSwitch mySwitch;
    
    //variables      
    int smoothcount;
    unsigned long samplesmooth[SAMPLE_SIZE];
    String rawString = "";
    int minsample = 30;

    //methods
   
    void decodeProtocol(uint16_t *pulseTrain, size_t length);


};

#endif