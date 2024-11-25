# Nucleus Gate Keeper: Sub-GHz Code Capture Device

![Build Status](https://img.shields.io/github/actions/workflow/status/GthiN89/NucleusGateKeeper/ci.yml?branch=main)
![GitHub stars](https://img.shields.io/github/stars/GthiN89/NucleusGateKeeper?style=social)

Nucleus Gate Keeper is a specialized, compact, and cost-effective device designed for **autonomously capturing Sub-GHz RF codes** used in remote-controlled systems, such as electric gates and other Sub-GHz-controlled devices. It is a side project born out of the Nucleus ESP32 codebase, leveraging much of its functionality to ensure rapid development and deployment.

The device is compatible with **Flipper Zero** Sub-GHz `.sub` file format, allowing seamless integration with Flipper workflows for replay or further analysis.

## 📋 Project Overview

Nucleus Gate Keeper simplifies the process of capturing RF codes without spending extensive time near the target device. It uses affordable and readily available hardware, focusing on **autonomous operation** and ease of use.

**Status**: In Development  
The device can autonomously receive and save codes, supports basic settings via serial or Bluetooth serial, and recognizes around 30 known protocols. While AM protocols work fine, FM protocols are currently buggy.

---

## 🚀 Features Implemented

### 🛡️ Autonomous Code Capture
- **Autonomous RF Code Capture**: The device independently detects and saves Sub-GHz signals without manual intervention.
- **SD Card Integration**: Captured codes are stored on an SD card for easy retrieval and analysis.
- **Flipper Zero Compatibility**: Captured codes are saved in `.sub` format, compatible with Flipper Zero for replay or analysis.
- **Protocol Recognition**: Supports around 30 known Sub-GHz protocols. When a known protocol is detected, it is recorded and visible in the filename.
- **Basic Settings Management**: Allows users to adjust basic settings through USB Serial or Bluetooth Serial connections.

### 📡 Communication
- **Serial and Bluetooth Serial**: Provides dual interfaces for configuring the device and accessing captured data.

### 📂 Known Protocols Handling
- **AM Protocols**: Fully functional and reliable.
- **FM Protocols**: Currently buggy and under refinement.

---

## 🛠️ Hardware

### 📦 Components
- **ESP32**: A versatile microcontroller for processing and communication.
- **CC1101 Transceiver**: Captures Sub-GHz signals.
- **SD Card Reader**: Provides onboard storage for captured codes.
- **Battery**: Powers the device for autonomous operation.

### 🔧 Why This Setup?
- **Proven Technology**: Leverages the established Nucleus ESP32 codebase for stable functionality.
- **Affordability**: Keeps costs low, making the device disposable if necessary.
- **Flipper Zero Integration**: Ensures seamless compatibility with widely used hacking tools.
- **Compact Design**: Ensures portability and ease of deployment.

---

## 🔍 Current Functionalities

- **Autonomous Code Capture**: Detects and logs RF codes without manual intervention.
- **SD Card Support**: Saves RF codes directly to an SD card for retrieval and analysis.
- **Flipper Zero-Compatible `.sub` Files**: Codes are saved in a format ready for use with Flipper Zero.
- **Protocol Detection and Naming**: Recognizes around 30 known protocols. Detected protocols are reflected in the filename for easy identification.
- **Basic Settings Over Serial/Bluetooth Serial**: Users can configure device settings through USB Serial or Bluetooth Serial interfaces.

**Note**: FM protocols are currently buggy and may not function reliably. AM protocols are fully operational.

---

## 🛤️ Planned Features

- **GSM/GPRS Communication**: Real-time updates and location tracking.
- **Magnetic Sensor Integration**: Correlate RF codes with gate movements.
- **Advanced RF Decoding**: Improved Sub-GHz protocol support and replay capabilities.
- **Broader Compatibility**: Support for additional Sub-GHz-controlled devices.

---

## ⏰ Timing and Transmission Quality

The current system is focused on reliable code capture. Future updates will improve RF decoding precision and add replay functionality.

---

## 📖 Usage Instructions

1. **Setup**: Insert an SD card and ensure the battery is charged.
2. **Deployment**: Place the device near the gate or Sub-GHz-controlled device.
3. **Monitoring**: The device autonomously captures and logs RF codes.
4. **Data Retrieval**: Collect the SD card to access captured RF codes in Flipper Zero-compatible `.sub` format.
5. **Configuration**: Adjust basic settings via USB Serial or Bluetooth Serial interfaces as needed.

---

## ⚠️ Limitations

- **FM Protocols**: Currently buggy and may not capture FM signals reliably.
- **Communication and Magnetic Sensor Features**: Under development.
- **Protocol Support**: Currently focused on around 30 Sub-GHz protocols; expansion is planned.

---

## 🤝 Contribute or Stay Updated

This project is open-source, and contributions are welcome! Whether it's optimizing the code, suggesting features, or helping with testing, your input is valuable. Follow updates and access source code at [https://github.com/GthiN89/NucleusGateKeeper](https://github.com/GthiN89/NucleusGateKeeper).

Happy hacking!

---

## 📝 Additional Notes

- **Protocol Identification**: When a known protocol is detected, the filename will include the protocol name for easy identification (e.g., `capture_AM650.sub`).
- **Serial and Bluetooth Serial Configuration**: Ensure that your Bluetooth device is paired with the ESP32 using the configured PIN for secure communication.
- **Security Enhancements**: Future updates may include more robust security features for Bluetooth communication.

---