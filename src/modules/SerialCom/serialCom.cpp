#include "serialCom.h"
#include "modules/CC1101/CC1101.h"

serialCom::serialCom() {
    // Constructor body (if needed)
}

void serialCom::begin(long baudRate, const String& btDeviceName) {
    Serial.begin(baudRate);
    SerialBT.begin(btDeviceName);
    Serial.println("ESP32 Command Interface Started");
}

void serialCom::update() {
    static String inputBuffer = ""; // Buffer for user input
    const char* promptColor = "\033[32m"; // Green color
    const char* resetColor = "\033[0m";   // Reset to default
    // Process USB Serial input
    while (Serial.available()) {
        char received = Serial.read();

        if (received == '\b' && !inputBuffer.isEmpty()) { // Handle backspace
            inputBuffer.remove(inputBuffer.length() - 1);
            Serial.print("\b \b");
        } else if (received == '\r' || received == '\n') { // Process on Enter
            Serial.println();
            if (!inputBuffer.isEmpty()) {
                processInput(inputBuffer, Serial);
                inputBuffer = "";
            }
            Serial.print(promptColor);
            Serial.print("GateKeeper> ");
            Serial.print(resetColor); 
        } else if (isPrintable(received)) { // Echo printable characters
            inputBuffer += received;
            Serial.print(received);
        }
    }

    // Process Bluetooth Serial input
    while (SerialBT.available()) {
        char received = SerialBT.read();

        if (received == '\b' && !inputBuffer.isEmpty()) {
            inputBuffer.remove(inputBuffer.length() - 1);
            SerialBT.print("\b \b");
        } else if (received == '\r' || received == '\n') {
            SerialBT.println();
            if (!inputBuffer.isEmpty()) {
                processInput(inputBuffer, SerialBT);
                inputBuffer = "";
            }
            SerialBT.print(promptColor);
            SerialBT.print("GateKeeper> ");
            SerialBT.print(resetColor); 
        } else if (isPrintable(received)) {
            inputBuffer += received;
            SerialBT.print(received);
        }
    }
}


void serialCom::processInput(const String& input, Stream& serialPort) {
    String trimmedInput = input;
    trimmedInput.trim();
    if (trimmedInput.length() == 0) return;

    // Split input into command name and arguments
    int spaceIndex = trimmedInput.indexOf(' ');
    String commandName = (spaceIndex == -1) ? trimmedInput : trimmedInput.substring(0, spaceIndex);
    String args = (spaceIndex == -1) ? "" : trimmedInput.substring(spaceIndex + 1);

    // Get the command code and execute the command
    CommandCode cmdCode = getCommandCode(commandName);
    executeCommand(cmdCode, commandName, args, serialPort); // Pass commandName instead of args
}


CommandCode serialCom::getCommandCode(const String& commandName) {
    if (commandName.equalsIgnoreCase("wifi")) return CMD_WIFI;
    else if (commandName.equalsIgnoreCase("time")) return CMD_TIME;
    else if (commandName.equalsIgnoreCase("gsm")) return CMD_GSM;
    else if (commandName.equalsIgnoreCase("cc1101")) return CMD_CC1101;
    else if (commandName.equalsIgnoreCase("help")) return CMD_HELP;
    else return CMD_UNKNOWN;
}

OperationCode serialCom::getOperationCode(const String& operationStr) {
    if (operationStr.equals("-r")) return OP_READ;
    else if (operationStr.equals("-w")) return OP_WRITE;
    else return OP_UNKNOWN_OP;
}

void serialCom::executeCommand(CommandCode cmdCode, const String& commandName, const String& args, Stream& serialPort) {
    switch (cmdCode) {
        case CMD_WIFI: {
            // Extract operation and options
            int spaceIndex = args.indexOf(' ');
            String operationStr = (spaceIndex == -1) ? args : args.substring(0, spaceIndex);
            String options = (spaceIndex == -1) ? "" : args.substring(spaceIndex + 1);
            OperationCode opCode = getOperationCode(operationStr);
            handleWiFiCommand(opCode, options, serialPort);
            break;
        }
        case CMD_TIME: {
            int spaceIndex = args.indexOf(' ');
            String operationStr = (spaceIndex == -1) ? args : args.substring(0, spaceIndex);
            String options = (spaceIndex == -1) ? "" : args.substring(spaceIndex + 1);
            OperationCode opCode = getOperationCode(operationStr);
            handleTimeCommand(opCode, options, serialPort);
            break;
        }
        case CMD_GSM: {
            int spaceIndex = args.indexOf(' ');
            String operationStr = (spaceIndex == -1) ? args : args.substring(0, spaceIndex);
            String options = (spaceIndex == -1) ? "" : args.substring(spaceIndex + 1);
            OperationCode opCode = getOperationCode(operationStr);
            handleGSMCommand(opCode, options, serialPort);
            break;
        }
        case CMD_CC1101: {
            int spaceIndex = args.indexOf(' ');
            String operationStr = (spaceIndex == -1) ? args : args.substring(0, spaceIndex);
            String options = (spaceIndex == -1) ? "" : args.substring(spaceIndex + 1);
            OperationCode opCode = getOperationCode(operationStr);
            handleCC1101Command(opCode, options, serialPort);
            break;
        }
        case CMD_HELP:
            showHelp(serialPort);
            break;
        default:
            serialPort.println("Unknown command: " + commandName);
            serialPort.println("Type 'help' to see the list of available commands.");
            break;
    }
}

void serialCom::handleWiFiCommand(OperationCode opCode, const String& options, Stream& serialPort) {
    switch (opCode) {
        case OP_READ:
            // Read Wi-Fi settings
            serialPort.println("***Not Implemented yet **Current Wi-Fi Settings:");
            serialPort.println("SSID: " + WiFi.SSID());
            serialPort.println("Password: " + WiFi.psk());
            break;
        case OP_WRITE: {
            // Write Wi-Fi settings
            String option1, value1, option2, value2;
            parseOptions(options, option1, value1, option2, value2);

            bool updated = false;
            if (option1 == "--ssid") {
                String newSSID = value1;
                // Implement actual Wi-Fi SSID change logic here
                // Example: Reconnect with new SSID
                WiFi.disconnect();
                WiFi.begin(newSSID.c_str(), WiFi.psk().c_str());
                serialPort.println("***Not Implemented yet **Wi-Fi SSID updated to: " + newSSID);
                updated = true;
            }
            if (option1 == "--password") {
                String newPassword = value1;
                // Implement actual Wi-Fi password change logic here
                WiFi.disconnect();
                WiFi.begin(WiFi.SSID().c_str(), newPassword.c_str());
                serialPort.println("***Not Implemented yet **Wi-Fi Password updated.");
                updated = true;
            }
            if (option2 == "--ssid") {
                String newSSID = value2;
                // Implement actual Wi-Fi SSID change logic here
                WiFi.disconnect();
                WiFi.begin(newSSID.c_str(), WiFi.psk().c_str());
                serialPort.println("***Not Implemented yet **Wi-Fi SSID updated to: " + newSSID);
                updated = true;
            }
            if (option2 == "--password") {
                String newPassword = value2;
                // Implement actual Wi-Fi password change logic here
                WiFi.disconnect();
                WiFi.begin(WiFi.SSID().c_str(), newPassword.c_str());
                serialPort.println("***Not Implemented yet **Wi-Fi Password updated.");
                updated = true;
            }
            if (!updated) {
                serialPort.println("***Not Implemented yet **No valid Wi-Fi options provided. Use '--ssid=<YourSSID>' and/or '--password=<YourPassword>'.");
            }
            break;
        }
        default:
            serialPort.println("***Not Implemented yet **Invalid Wi-Fi operation. Use '-r' to read or '-w' to write.");
            break;
    }
}

void serialCom::handleTimeCommand(OperationCode opCode, const String& options, Stream& serialPort) {
    switch (opCode) {
        case OP_READ:
            // Read system time
            serialPort.println("***Not Implemented yet **Current system time (ms since boot): " + String(millis()));
            break;
        case OP_WRITE: {
            // Write system time
            String option1, value1, option2, value2;
            parseOptions(options, option1, value1, option2, value2);

            if (option1 == "--value") {
                String timestampStr = value1;
                // Implement actual time setting logic here if using an RTC or NTP
                // Example placeholder:
                serialPort.println("System time updated to: " + timestampStr);
            }
            if (option2 == "--value") {
                String timestampStr = value2;
                // Implement actual time setting logic here
                serialPort.println("***Not Implemented yet **System time updated to: " + timestampStr);
            }
            if (option1 != "--value" && option2 != "--value") {
                serialPort.println("***Not Implemented yet **No time value provided. Use '--value=<timestamp>'.");
            }
            break;
        }
        default:
            serialPort.println("***Not Implemented yet **Invalid Time operation. Use '-r' to read or '-w' to write.");
            break;
    }
}

void serialCom::handleGSMCommand(OperationCode opCode, const String& options, Stream& serialPort) {
    switch (opCode) {
        case OP_READ:
            // Read GSM module status
            serialPort.println("***Not Implemented yet **Current GSM module status: ...");
            break;
        case OP_WRITE: {
            // Write GSM settings
            String option1, value1, option2, value2;
            parseOptions(options, option1, value1, option2, value2);

            bool updated = false;
            if (option1 == "--apn") {
                String newAPN = value1;
                // Implement actual GSM APN change logic here
                serialPort.println("***Not Implemented yet **GSM APN updated to: " + newAPN);
                updated = true;
            }
            if (option2 == "--apn") {
                String newAPN = value2;
                // Implement actual GSM APN change logic here
                serialPort.println("***Not Implemented yet **GSM APN updated to: " + newAPN);
                updated = true;
            }
            if (!updated) {
                serialPort.println("***Not Implemented yet **No APN provided. Use '--apn=<YourAPN>'.");
            }
            break;
        }
        default:
            serialPort.println("***Not Implemented yet **Invalid GSM operation. Use '-r' to read or '-w' to write.");
            break;
    }
}

void serialCom::handleCC1101Command(OperationCode opCode, const String& options, Stream& serialPort) {
    CC1101_CLASS CC1101;
    switch (opCode) {
        case OP_READ:
            // Read CC1101 settings
            serialPort.println("Current CC1101 settings:");
            serialPort.println("Preset:");
            serialPort.print(CC1101.getPresetSettingsString());
            serialPort.println("\nFreque:");
            serialPort.print(CC1101.getFrequeSettingsString());
            serialPort.println("State:"); 
            serialPort.print(getStateString(gateKeeperState));
            break;
        case OP_WRITE: {
            gateKeeperState == STATE_IDLE;
            // Write CC1101 settings
            String option1, value1, option2, value2;
            parseOptions(options, option1, value1, option2, value2);

            bool updated = false;
                // Function to check if a frequency is within the valid ranges
            auto isValidFrequency = [](float freq) -> bool {
                return (freq >= 300.0 && freq <= 348.0) || 
                       (freq >= 387.0 && freq <= 464.0) || 
                       (freq >= 779.0 && freq <= 928.0);
            };

            // Handle option1
            if (option1 == "--freq") {
                float freq = value1.toFloat();
                if (isValidFrequency(freq)) {
                    CC1101.setFrequency(freq);
                    serialPort.println("CC1101 frequency set to: " + String(freq));
                    updated = true;
                } else {
                    serialPort.println("Invalid frequency. Allowed ranges are 300-348, 387-464, 779-928 MHz.");
                }
            } else if (option1 == "--preset") {                
                String newPreset = value1;
                CC1101_PRESET presetEnum = CC1101.convert_str_to_enum(newPreset.c_str());
                if (CC1101.isValidPreset(presetEnum)) {
                    CC1101.setCC1101Preset(presetEnum);
                    CC1101.loadPreset(presetEnum);
                    CC1101.enableReceiver();
                    serialPort.println("CC1101 preset set to: " + newPreset);
                    updated = true;                    
                    delay(20);
                } else {
                    serialPort.println("Invalid preset. Use 'help' to see available presets.");
                }
            } else if (option1 == "--DC") {
                String settings = value1;
                if (settings.toInt() == 1 || settings.toInt() == 1) {
                    CC1101.setCC1101DCcorrection(settings.toInt());
                    serialPort.println("CC1101 DC correction off is: " + settings);
                    updated = true;
                } else {
                    serialPort.println("Invalid preset. Use 'help' to see available presets.");
                }
            }

            // Handle option2 (if present)
            if (option2 == "--freq") {
                float freq = value2.toFloat();
                if (isValidFrequency(freq)) {
                    CC1101.setFrequency(freq);
                    serialPort.println("CC1101 frequency set to: " + String(freq));
                    updated = true;
                } else {
                    serialPort.println("Invalid frequency. Allowed ranges are 300-348, 387-464, 779-928 MHz.");
                }
            } else if (option2 == "--preset") {
                String newPreset = value2;
                CC1101_PRESET presetEnum = CC1101.convert_str_to_enum(newPreset.c_str());
                if (CC1101.isValidPreset(presetEnum)) {
                    CC1101.setCC1101Preset(presetEnum);
                    CC1101.loadPreset(presetEnum);
                    CC1101.enableReceiver();
                    serialPort.println("CC1101 preset set to: " + newPreset);
                    updated = true;
                    delay(20);                    
                } else {
                    serialPort.println("Invalid preset. Use 'help' to see available presets.");
                }
            }


            if (!updated) {
                serialPort.println("No frequency or preset provided. Use '--freq=<YourFrequency>' or '--preset=<YourPreset>'.");
            }
            break;
            gateKeeperState == STATE_INIT_SUCESS;
        }
        default:
            serialPort.println("Invalid CC1101 operation. Use '-r' to read or '-w' to write.");
            break;
    }
}

String serialCom::getStateString(KEEPER_STATE state)  {
        switch(state) {
            case STATE_INIT:
                return "STATE_INIT";
            case STATE_INIT_FAIL:
                return "STATE_INIT_FAIL";
            case STATE_INIT_SUCESS:
                return "STATE_INIT_SUCESS";
            case STATE_SDCARD_EROOR:
                return "STATE_SDCARD_EROOR";
            case STATE_IDLE:
                return "STATE_IDLE";
            case STATE_CAPTURE:
                return "STATE_CAPTURE";
            case STATE_CAPTURE_COMPLETE:
                return "STATE_CAPTURE_COMPLETE";
            case STATE_SIGNAL_ANALYZER:
                return "STATE_SIGNAL_ANALYZER";
            case STATE_SIGNAL_ANALYZER_COMPLETE:
                return "STATE_SIGNAL_ANALYZER_COMPLETE";
            case STATE_FILE_OPERATION:
                return "STATE_FILE_OPERATION";
            case STATE_READY_TO_ACTION:
                return "STATE_READY_TO_ACTION";
            default:
                return "UNKNOWN_STATE";
        }
}

void serialCom::showHelp(Stream& serialPort) {
    serialPort.println("Available commands:");
    serialPort.println("wifi -r                             : Read Wi-Fi settings");
    serialPort.println("wifi -w --ssid=<SSID>               : Set Wi-Fi SSID");
    serialPort.println("wifi -w --password=<Password>       : Set Wi-Fi password");
    serialPort.println("time -r                             : Read system time");
    serialPort.println("time -w --value=<timestamp>         : Set system time");
    serialPort.println("gsm -r                              : Read GSM module status");
    serialPort.println("gsm -w --apn=<APN>                  : Set GSM APN");
    serialPort.println("cc1101 -r                           : Read CC1101 settings");
    serialPort.println("cc1101 -w --freq=<frequency>        : Set CC1101 frequency");
    serialPort.println("cc1101 -w --preset=<preset>         : Set CC1101 preset");
    serialPort.println("Available presets:");
    serialPort.println("  AM650");
    serialPort.println("  AM270");
    serialPort.println("  FM238");
    serialPort.println("  FM476");
    serialPort.println("  FM95");
    serialPort.println("  FSK12k");
    serialPort.println("  FM15k");
    serialPort.println("  FSK25k");
    serialPort.println("  FSK31k");
    serialPort.println("  PAGER");
    serialPort.println("  HND1");
    serialPort.println("  HND2");
    serialPort.println("  CUSTOM");
    serialPort.println("cc1101 -w --DC=<0/1>                : Set CC1101 DC Correction OFF/ON");
    serialPort.println("help                                : Show this help message");
}

void serialCom::parseOptions(const String& optionsStr, String& option1, String& value1, String& option2, String& value2) {
    // Initialize output parameters
    option1 = "";
    value1 = "";
    option2 = "";
    value2 = "";

    int firstOptionIndex = optionsStr.indexOf("--");
    if (firstOptionIndex == -1) return;

    int secondOptionIndex = optionsStr.indexOf("--", firstOptionIndex + 2);
    String firstOptionStr = (secondOptionIndex == -1) ? optionsStr.substring(firstOptionIndex) : optionsStr.substring(firstOptionIndex, secondOptionIndex);
    String secondOptionStr = (secondOptionIndex == -1) ? "" : optionsStr.substring(secondOptionIndex);

    // Parse first option
    int equalIndex1 = firstOptionStr.indexOf('=');
    if (equalIndex1 != -1) {
        option1 = firstOptionStr.substring(0, equalIndex1);
        option1.trim(); // Modify in-place

        value1 = firstOptionStr.substring(equalIndex1 + 1);
        value1.trim(); // Modify in-place
    }

    // Parse second option if present
    if (secondOptionStr.length() > 0) {
        int equalIndex2 = secondOptionStr.indexOf('=');
        if (equalIndex2 != -1) {
            option2 = secondOptionStr.substring(0, equalIndex2);
            option2.trim(); // Modify in-place

            value2 = secondOptionStr.substring(equalIndex2 + 1);
            value2.trim(); // Modify in-place
        }
    }
}
