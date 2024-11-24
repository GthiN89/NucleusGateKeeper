#include "CC1101.h"
#include "main.h"
#include "SD.h"
#include "FlipperSubFile.h"

#define SD_MOSI 23
#define SD_MISO 19
#define SD_CLK  18
#define SD_CS   5

//outher class variables *not feel right, will do differentely in future

bool receiverEnabled;
static unsigned long lastTime = 0;
int sample[SAMPLE_SIZE];
int error_toleranz = 200;
int samplecount = 0;
int receiverGPIO;

float CC1101_DRATE = 3.79372;;
float CC1101_RX_BW = 650.00;;
float CC1101_DEVIATION = 47.60;
int CC1101_PKT_FORMAT = 0;
int CC1101_SYNC = 2;
float CC1101_FREQ = 433.92;
int CC1101_MODULATION = 2;
float CC1101_MHZ = 433.92;

bool CC1101_is_initialized = false;
bool CC1101_recieve_is_running = false;
bool CC1101_transmit_is_running = false;
bool CC1101_isiddle = true;
bool CC1101_interup_attached = false;





  ////////////////////
 //////Protocols/////
////////////////////

String protDecode[]={
    "Unknown",
    "01 Princeton, PT-2240",
    "02 AT-Motor?",
    "03",
    "04",
    "05",
    "06 HT6P20B",
    "07 HS2303-PT, i. e. used in AUKEY Remote",
    "08 Came 12bit, HT12E",
    "09 Nice_Flo 12bit",
    "10 V2 phoenix",
    "11 Nice_FloR-S 52bit",
    "12 Keeloq 64/66 falseok",
    "13 test CFM",
    "14 test StarLine",
    "15",
    "16 Einhell",
    "17 InterTechno PAR-1000",
    "18 Intertechno ITT-1500",
    "19 Murcury",
    "20 AC114",
    "21 DC250",
    "22 Mandolyn/Lidl TR-502MSV/RC-402/RC-402DX",
    "23 Lidl TR-502MSV/RC-402 - Flavien",
    "24 Lidl TR-502MSV/RC701",
    "25 NEC",
    "26 Arlec RC210",
    "27 Zap, FHT-7901",
    "28", // github.com/sui77/rc-switch/pull/115
    "29 NEXA",
    "30 Anima",
    "31 Mertik Maxitrol G6R-H4T1",
    "32", //github.com/sui77/rc-switch/pull/277
    "33 Dooya Control DC2708L",
    "34 DIGOO SD10 ", //so as to use this protocol RCSWITCH_SEPARATION_LIMIT must be set to 2600
    "35 Dooya 5-Channel blinds remote DC1603",
    "36 DC2700AC", //Dooya remote DC2700AC for Dooya DT82TV curtains motor
    "37 DEWENWILS Power Strip",
    "38 Nexus weather, 36 bit",
    "39 Louvolite with premable"
};

void IRAM_ATTR InterruptHandler()
{  

    if (!receiverEnabled)
    {
        return;
    }
    
    const long time = micros();
    const unsigned int duration = time - lastTime;

    if (duration > 100000)
    {
        samplecount = 0;
    }

    if (duration >= 100)
    {
        sample[samplecount++] = duration;
    }

    if (samplecount >= SAMPLE_SIZE)
    {
        return;
    }

    if (CC1101_MODULATION == 0)
    {
        if (samplecount == 1 && digitalRead(receiverGPIO) != HIGH)
        {
            samplecount = 0;
        }
    }

    lastTime = time;
}

bool CC1101_CLASS::CheckReceived()
{
    if (samplecount >= minsample && micros() - lastTime > 100000)
    {
        receiverEnabled = false;
        gateKeeperState = STATE_CAPTURE_COMPLETE;
        return 1;
    }
    else
    {
        return 0;
    }
}

bool CC1101_CLASS::init()
{
    ELECHOUSE_cc1101.setSpiPin(CC1101_SCLK, CC1101_MISO, CC1101_MOSI, CC1101_CS);
    ELECHOUSE_cc1101.Init();

    if (ELECHOUSE_cc1101.getCC1101())
    {
        ELECHOUSE_cc1101.setGDO0(CC1101_CCGDO0A);
        ELECHOUSE_cc1101.setSidle();
        CC1101_isiddle = true;
        CC1101_is_initialized = true;
        return true;
    }
    else
    {
        ELECHOUSE_cc1101.setSidle();
        CC1101_isiddle = true;
        return false;
    }
    
}

void CC1101_CLASS::setCC1101Preset(CC1101_PRESET preset) {
    C1101preset = preset;
}

void CC1101_CLASS::loadPreset() {
    switch (C1101preset) {
    case AM650:
         CC1101_MODULATION = 2;
         CC1101_DRATE = 3.79372;
         CC1101_RX_BW = 650.00;
         CC1101_DEVIATION = 1.58;
         break;
    case AM270:
         CC1101_MODULATION = 2;
         CC1101_DRATE = 3.79372;
         CC1101_RX_BW = 270.833333;
         CC1101_DEVIATION = 1.58;
         break;
    case FM238:
         CC1101_MODULATION = 0;
         CC1101_DRATE = 4.79794;
         CC1101_RX_BW = 270.833333;
         CC1101_DEVIATION = 2.380371;
         break;
    case FM476:
         CC1101_MODULATION = 0;
         CC1101_DRATE = 4.79794;
         CC1101_RX_BW = 270.833333;
         CC1101_DEVIATION = 47.60742;
         break;
    case FM95:
         CC1101_MODULATION = 0;
         CC1101_DRATE = 4.798;
         CC1101_RX_BW = 270;
         CC1101_DEVIATION = 9.521;
         CC1101_SYNC = 6;
         break;
    case FM15k:
         CC1101_MODULATION = 0;
         CC1101_DRATE = 3.794;
         CC1101_RX_BW = 135;
         CC1101_DEVIATION = 15.869;
         CC1101_SYNC = 7;
         break;
    case FSK12k:
         CC1101_MODULATION = 0;       
         CC1101_DRATE = 12.69;        
         CC1101_RX_BW = 200;          
         CC1101_DEVIATION = 12.69;    
         break;
    case FSK25k:
         CC1101_MODULATION = 0;       
         CC1101_DRATE = 25.39;        
         CC1101_RX_BW = 200;          
         CC1101_DEVIATION = 25.39;    
         CC1101_SYNC = 0x47;          
         break;     
    case FSK31k:
         CC1101_MODULATION = 0;       
         CC1101_DRATE = 31.73;        
         CC1101_RX_BW = 200;          
         CC1101_DEVIATION = 31.73;    
         break;
    case PAGER:
         CC1101_MODULATION = 0;
         CC1101_DRATE = 0.625;
         CC1101_RX_BW = 270;
         CC1101_DEVIATION = 5.157;
         CC1101_SYNC = 6;
         break;
    case HND1:
         CC1101_MODULATION = 0;
         CC1101_DRATE = 37.04;
         CC1101_RX_BW = 250;
         CC1101_DEVIATION = 30;
         CC1101_SYNC = 6;
         break;
    case HND2:
         CC1101_MODULATION = 0;
         CC1101_DRATE = 15.357;
         CC1101_RX_BW = 67;
         CC1101_DEVIATION = 15.869;
         CC1101_SYNC = 7;
         break;
    default:
          CC1101_MODULATION = 2;
         CC1101_DRATE = 3.79372;
         CC1101_RX_BW = 650.00;
         CC1101_DEVIATION = 1.58;
        break;
    }
    Serial.print("preset loaded");
    delay(20);
}

void CC1101_CLASS::enableReceiver()
{
    if (!CC1101_is_initialized) {
        CC1101_CLASS::init();
    }
    CC1101_CLASS::loadPreset();



    memset(sample, 0, sizeof(SAMPLE_SIZE));
    samplecount = 0;

    if (CC1101_MODULATION == 2)
    {
        ELECHOUSE_cc1101.setDcFilterOff(0);
    } 

    if (CC1101_MODULATION == 0)
    {
        ELECHOUSE_cc1101.setDcFilterOff(1);
    }

    ELECHOUSE_cc1101.setSyncMode(CC1101_SYNC);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setPktFormat(CC1101_PKT_FORMAT); // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX.
                                                      // 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
                                                      // 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX.
                                                      // 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
                                                      // ELECHOUSE_cc1101.setSyncMode(3);       
    ELECHOUSE_cc1101.setModulation(CC1101_MODULATION); // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(CC1101_MHZ);               // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setDeviation(CC1101_DEVIATION);
    ELECHOUSE_cc1101.setDRate(CC1101_DRATE); // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
    ELECHOUSE_cc1101.setRxBW(CC1101_RX_BW);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
    pinMode(CC1101_CCGDO0A, INPUT);
    receiverGPIO = digitalPinToInterrupt(CC1101_CCGDO0A);    
    ELECHOUSE_cc1101.SetRx();
    /////////////////////////////
    receiverEnabled = true;
    //////////////////////////////
    attachInterrupt(CC1101_CCGDO0A, InterruptHandler, CHANGE);
    Serial.print("interrupt active");

}

void CC1101_CLASS::setFrequency(float freq)
{
    CC1101_MHZ = freq;
    ELECHOUSE_cc1101.setMHZ(CC1101_MHZ);
}


void CC1101_CLASS::signalanalyse(){
 #define signalstorage 10

  int signalanz=0;
  int timingdelay[signalstorage];
  float pulse[signalstorage];
  long signaltimings[signalstorage*2];
  int signaltimingscount[signalstorage];
  long signaltimingssum[signalstorage];
  long signalsum=0;

  for (int i = 0; i<signalstorage; i++){
    signaltimings[i*2] = 100000;
    signaltimings[i*2+1] = 0;
    signaltimingscount[i] = 0;
    signaltimingssum[i] = 0;
  }
  for (int i = 1; i<samplecount; i++){
    signalsum+=sample[i];
  }

  for (int p = 0; p<signalstorage; p++){

  for (int i = 1; i<samplecount; i++){
    if (p==0){
      if (sample[i]<signaltimings[p*2]){
        signaltimings[p*2]=sample[i];
      }
    }else{
      if (sample[i]<signaltimings[p*2] && sample[i]>signaltimings[p*2-1]){
        signaltimings[p*2]=sample[i];
      }
    }
  }

  for (int i = 1; i<samplecount; i++){
    if (sample[i]<signaltimings[p*2]+error_toleranz && sample[i]>signaltimings[p*2+1]){
      signaltimings[p*2+1]=sample[i];
    }
  }

  for (int i = 1; i<samplecount; i++){
    if (sample[i]>=signaltimings[p*2] && sample[i]<=signaltimings[p*2+1]){
      signaltimingscount[p]++;
      signaltimingssum[p]+=sample[i];
    }
  }
  }
  
  int firstsample = signaltimings[0];
  
  signalanz=signalstorage;
  for (int i = 0; i<signalstorage; i++){
    if (signaltimingscount[i] == 0){
      signalanz=i;
      i=signalstorage;
    }
  }

  for (int s=1; s<signalanz; s++){
  for (int i=0; i<signalanz-s; i++){
    if (signaltimingscount[i] < signaltimingscount[i+1]){
      int temp1 = signaltimings[i*2];
      int temp2 = signaltimings[i*2+1];
      int temp3 = signaltimingssum[i];
      int temp4 = signaltimingscount[i];
      signaltimings[i*2] = signaltimings[(i+1)*2];
      signaltimings[i*2+1] = signaltimings[(i+1)*2+1];
      signaltimingssum[i] = signaltimingssum[i+1];
      signaltimingscount[i] = signaltimingscount[i+1];
      signaltimings[(i+1)*2] = temp1;
      signaltimings[(i+1)*2+1] = temp2;
      signaltimingssum[i+1] = temp3;
      signaltimingscount[i+1] = temp4;
    }
  }
  }

  for (int i=0; i<signalanz; i++){
    timingdelay[i] = signaltimingssum[i]/signaltimingscount[i];
  }

  if (firstsample == sample[1] and firstsample < timingdelay[0]){
    sample[1] = timingdelay[0];
  }

  bool lastbin=0;
  for (int i=1; i<samplecount; i++){
    float r = (float)sample[i]/timingdelay[0];
    int calculate = r;
    r = r-calculate;
    r*=10;
    if (r>=5){calculate+=1;}
    if (calculate>0){
      if (lastbin==0){
        lastbin=1;
      }else{
      lastbin=0;
    }
    }
  }
  Serial.println();
  Serial.print("Samples/Symbol: ");
  Serial.println(timingdelay[0]);
  Serial.println();

  smoothcount=0;
  for (int i=1; i<samplecount; i++){
    float r = (float)sample[i]/timingdelay[0];
    int calculate = r;
    r = r-calculate;
    r*=10;
    if (r>=5){calculate+=1;}
    if (calculate>0){
      samplesmooth[smoothcount] = calculate*timingdelay[0];
      smoothcount++;
    }
  }

uint16_t pulseTrain[SAMPLE_SIZE]; 
size_t length = smoothcount;

for (size_t i = 0; i < smoothcount; i++) {
    pulseTrain[i] = static_cast<uint16_t>(samplesmooth[i]);
}


String rawString = "";
Serial.println("");
    for (int i = 0; i < smoothcount; i++) {
            rawString += (i > 0 ? (i % 2 == 1 ? " -" : " ") : "");
            rawString += samplesmooth[i];
            Serial.print(samplesmooth[i]);
            Serial.print(", ");
        }


decodeProtocol(pulseTrain, length);
        
   delay(100);
        Serial.println(rawString);
           delay(100);
         SD.begin(SD_CS);
         FlipperSubFile subFile;
    



String filename = generateFilename(CC1101_MHZ, CC1101_MODULATION, CC1101_RX_BW);
String fullPath = "/ReceivedCodes/" + filename;


File outputFile = SD.open(fullPath, FILE_WRITE);
if (outputFile) {
    std::vector<byte> customPresetData;
    if (C1101preset == CUSTOM) {
        customPresetData.insert(customPresetData.end(), {
            CC1101_MDMCFG4, ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG4),
            CC1101_MDMCFG3, ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG3),
            CC1101_MDMCFG2, ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG2),
            CC1101_DEVIATN, ELECHOUSE_cc1101.SpiReadReg(CC1101_DEVIATN),
            CC1101_FREND0,  ELECHOUSE_cc1101.SpiReadReg(CC1101_FREND0),
            0x00, 0x00
        });

        std::array<byte, 8> paTable;
        ELECHOUSE_cc1101.SpiReadBurstReg(0x3E, paTable.data(), paTable.size());
        customPresetData.insert(customPresetData.end(), paTable.begin(), paTable.end());
    }
    subFile.generateRaw(outputFile, C1101preset, customPresetData, rawString, CC1101_MHZ);
    outputFile.close();
    SD.end();
  gateKeeperState = STATE_INIT_SUCESS;
}
}

void CC1101_CLASS::decodeProtocol(uint16_t *pulseTrain, size_t length) {
    if (length == 0) {
        Serial.println("No pulses to decode.");
        return;
    }

    bool foundSeparator = false;
    size_t startIndex = 0, endIndex = 0;

    // Separate single code by identifing of pause
    for (size_t i = 0; i < length; i++) {
        if (pulseTrain[i] > 2500) {
            if (!foundSeparator) {
                foundSeparator = true;
                startIndex = i + 1;
            } else {
                endIndex = i;
                break;
            }
        }
    }

    if (foundSeparator && endIndex > startIndex) {
        size_t newLength = endIndex - startIndex;
        for (size_t i = 0; i < newLength; i++) {
            pulseTrain[i] = pulseTrain[startIndex + i];
        }
        length = newLength;
    } else {
        length = 0;
    }

    // Enable RC Switch decoding
    mySwitch.enableReceive();

    // Ensure the pulse train length is within RC Switch's limits
    if (length > RCSWITCH_MAX_CHANGES) {
        Serial.println("Error: Pulse train too long for RC Switch.");
        return;
    }

    // Populate RC Switch's static timings array
    for (size_t i = 0; i < length; i++) {
        mySwitch.timings[i] = pulseTrain[i];
    }

    // Attempt to decode with RC Switch
    for (int protocol = 1; protocol <= mySwitch.getNumProtos(); protocol++) {
        if (mySwitch.receiveProtocol(protocol, length)) {
            
            unsigned long long receivedValue = mySwitch.getReceivedValue();
            Serial.println("Decoded Signal:");
            Serial.print("Protocol: ");
            Serial.println(protDecode[protocol]);
            Serial.print("Value: ");
            Serial.println(receivedValue);
            Serial.print("Bit Length: ");
            Serial.println(mySwitch.getReceivedBitlength());
            Serial.print("Pulse Length: ");
            Serial.println(mySwitch.getReceivedDelay());  
            

        if (receivedValue == 0) {
            Serial.println("Unknown encoding.");
        } else {
        
            // Print the value in binary
            Serial.print("Binary: ");
            for (int i = mySwitch.getReceivedBitlength() - 1; i >= 0; i--) {
                Serial.print((receivedValue >> i) & 1);
            }           

            char hexBuffer[20];
            snprintf(hexBuffer, sizeof(hexBuffer), "0x%lX", receivedValue); // Convert to hex
            Serial.println("\nHex: ");
            Serial.println(hexBuffer);


        if (receivedValue <= 0x7F) { 
            char asciiBuffer[2] = {0}; 
            asciiBuffer[0] = static_cast<char>(receivedValue); // Convert to character
            Serial.print("ASCII: '");
            Serial.write(asciiBuffer[0]); // Debug in Serial
            Serial.println("'");
        }
        // Reset for the next signal
        mySwitch.resetAvailable();
                return;
            }
        }
    }
}

String CC1101_CLASS::generateFilename(float frequency, int modulation, float bandwidth)
{
    char filenameBuffer[32];

    sprintf(filenameBuffer, "%d_%s_%s.sub", static_cast<int>(frequency * 100), presetToString(C1101preset),
            generateRandomString(8).c_str());

    return String(filenameBuffer);
}

const char* CC1101_CLASS::presetToString(CC1101_PRESET preset) {
    switch (preset) {
        case AM650: return "AM650";
        case AM270: return "AM270";
        case FM238: return "FM238";
        case FM476: return "FM476";
        case FM95:  return "FM95";
        case FSK12k: return "FSK12k";
        case FM15k: return "FM15k";
        case FSK25k: return "FSK25k";
        case FSK31k: return "FSK31k";
        case PAGER: return "PAGER";
        case HND1:  return "HND1";
        case HND2:  return "HND2";
        default:    return "Unknown";
    }
}

String CC1101_CLASS::generateRandomString(int length)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::stringstream ss;
    for (int i = 0; i < length; ++i) {
        int randomIndex = std::rand() % characters.size();
        char randomChar = characters[randomIndex];
        ss << randomChar;
    }

    return String(ss.str().c_str());
}