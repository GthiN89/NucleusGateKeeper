#include "FlipperSubFile.h"
#include <sstream>  


// Initialize the preset mapping (if this is not done elsewhere)
const std::map<CC1101_PRESET, std::string> FlipperSubFile::presetMapping = {
    {AM270, "FuriHalSubGhzPresetOok270Async"},
    {AM650, "FuriHalSubGhzPresetOok650Async"},
    {FM238, "FuriHalSubGhzPreset2FSKDev238Async"},
    {FM476, "FuriHalSubGhzPreset2FSKDev476Async"},
    {CUSTOM, "FuriHalSubGhzPresetCustom"}
};

void FlipperSubFile::generateRaw(
    File& file,
    CC1101_PRESET presetName,
    const std::vector<byte>& customPresetData,
    String& samples,
    float frequency
) {
    if (!file) {
        Serial.println("Error: File is not open.");
        return;
    }
    writeHeader(file, frequency);
    writePresetInfo(file, presetName, customPresetData);
    writeRawProtocolData(file, samples);
}

void FlipperSubFile::writeHeader(File& file, float frequency) {
    file.println("Filetype: Flipper SubGhz RAW File");
    file.println("Version: 1");
    file.print("Frequency: ");
    file.print(frequency * 1e6, 0);
    file.println();
}

void FlipperSubFile::writePresetInfo(File& file, CC1101_PRESET presetName, const std::vector<byte>& customPresetData) {
    file.print("Preset: ");
    file.println(getPresetName(presetName).c_str());

    if (presetName == CC1101_PRESET::CUSTOM) {
        file.println("Custom_preset_module: CC1101");
        file.print("Custom_preset_data: ");
        for (size_t i = 0; i < customPresetData.size(); ++i) {
            char hexStr[3];
            sprintf(hexStr, "%02X", customPresetData[i]);
            file.print(hexStr);
            if (i < customPresetData.size() - 1) {
                file.print(" ");
            }
        }
        file.println();
    }
}

void FlipperSubFile::writeRawProtocolData(File& file, String& samples) {
    file.println("Protocol: RAW");
    file.print("RAW_Data: ");

    std::string sample;
    int wordCount = 0;
    
    std::string cppString(samples.c_str());
    std::istringstream stream(cppString);

    while (std::getline(stream, sample, ' ')) {
        if (wordCount > 0 && wordCount % 512 == 0) {
            file.println();
            file.print("RAW_Data: ");
        }
        file.print(sample.c_str());
        file.print(' ');
        wordCount++;
    }
    file.println();
}

std::string FlipperSubFile::getPresetName(CC1101_PRESET preset) {
    auto it = presetMapping.find(preset);
    return (it != presetMapping.end()) ? it->second : "FuriHalSubGhzPresetCustom";
}