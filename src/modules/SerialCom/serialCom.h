#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <Arduino.h>
#include <WiFi.h>
#include "BluetoothSerial.h"
#include "main.h"

// Define command codes using enum for better type safety
enum CommandCode {
    CMD_WIFI = 1,
    CMD_TIME,
    CMD_GSM,
    CMD_CC1101,
    CMD_HELP,
    CMD_UNKNOWN = -1
};

// Define operation codes
enum OperationCode {
    OP_READ = 1,
    OP_WRITE,
    OP_UNKNOWN_OP = -1
};

class serialCom {
public:
    serialCom();
    void begin(long baudRate, const String& btDeviceName);
    void update();

private:
    BluetoothSerial SerialBT;

    // Command processing
    void processInput(const String& input, Stream& serialPort);
    void executeCommand(CommandCode cmdCode, const String& commandName, const String& args, Stream& serialPort);

    // Command handlers
    void handleWiFiCommand(OperationCode opCode, const String& options, Stream& serialPort);
    void handleTimeCommand(OperationCode opCode, const String& options, Stream& serialPort);
    void handleGSMCommand(OperationCode opCode, const String& options, Stream& serialPort);
    void handleCC1101Command(OperationCode opCode, const String& options, Stream& serialPort);
    void showHelp(Stream& serialPort);

    // Utility functions
    CommandCode getCommandCode(const String& commandName);
    OperationCode getOperationCode(const String& operationStr);
    void parseOptions(const String& optionsStr, String& option1, String& value1, String& option2, String& value2);
    String getStateString(KEEPER_STATE state);
};

#endif // SERIALCOM_H
