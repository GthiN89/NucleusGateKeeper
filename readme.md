# Nucleus Gate Keeper: Sub-GHz Code Capture Device

Nucleus Gate Keeper is a specialized, compact, and cost-effective device designed for capturing Sub-GHz RF codes used in remote-controlled systems, such as electric gates and other Sub-GHz-controlled devices. It is a side project born out of the Nucleus ESP32 codebase, leveraging much of its functionality to ensure rapid development and deployment.  

The device is compatible with **Flipper Zero** Sub-GHz `.sub` file format, allowing seamless integration with Flipper workflows for replay or further analysis.

## Project Overview

Nucleus Gate Keeper simplifies the process of capturing RF codes without spending extensive time near the target device. It uses affordable and readily available hardware, focusing on autonomous operation and ease of use.

**Status**: In Development  
Most of the core functionality is already part of the Nucleus codebase, enabling fast progress. Some features are still under development.

---

## Features Implemented

### Code Capture
- **Autonomous RF Code Capture**: The device independently detects and saves Sub-GHz signals.
- **SD Card Integration**: Captured codes are stored on an SD card for easy retrieval and analysis.
- **Flipper Zero Compatibility**: Captured codes are saved in `.sub` format, compatible with Flipper Zero for replay or analysis.

### Communication (In Progress)
- **Planned GSM/GPRS Module**: To enable real-time updates, location tracking, and remote communication with the device.

### Magnetic Field Monitoring (In Progress)
- **Planned Integration**: A magnetic sensor will track physical gate movements to correlate them with captured RF codes.

### Hardware Integration
- **CC1101 Transceiver**: Efficiently handles Sub-GHz RF signal detection and processing.
- **ESP32 Microcontroller**: Provides reliable processing and a solid foundation for the device's functionality.

---

## Hardware

### Components
- **ESP32**: A versatile microcontroller for processing and communication.
- **CC1101 Transceiver**: Captures Sub-GHz signals.
- **SD Card Reader**: Provides onboard storage for captured codes.
- **Battery**: Powers the device for autonomous operation.

### Why This Setup?
- **Proven Technology**: Leverages the established Nucleus ESP32 codebase for stable functionality.
- **Affordability**: Keeps costs low, making the device disposable if necessary.
- **Flipper Zero Integration**: Ensures seamless compatibility with widely used hacking tools.
- **Compact Design**: Ensures portability and ease of deployment.

---

## Current Functionalities

- **Autonomous Code Capture**: Detects and logs RF codes without manual intervention.
- **SD Card Support**: Saves RF codes directly to an SD card for retrieval and analysis.
- **Flipper Zero-Compatible `.sub` Files**: Codes are saved in a format ready for use with Flipper Zero.

---

## Planned Features
- **GSM/GPRS Communication**: Real-time updates and location tracking.
- **Magnetic Sensor Integration**: Correlate RF codes with gate movements.
- **Advanced RF Decoding**: Improved Sub-GHz protocol support and replay capabilities.
- **Broader Compatibility**: Support for additional Sub-GHz-controlled devices.

---

## Timing and Transmission Quality

The current system is focused on reliable code capture. Future updates will improve RF decoding precision and add replay functionality.

---

## Usage Instructions

1. **Setup**: Insert an SD card and ensure the battery is charged.
2. **Deployment**: Place the device near the gate or Sub-GHz-controlled device.
3. **Monitoring**: Allow the device to autonomously capture and log RF codes.
4. **Data Retrieval**: Collect the SD card to access captured RF codes in Flipper Zero-compatible `.sub` format.

---

## Limitations
- Communication and magnetic sensor features are under development.
- Compatibility is currently focused on Sub-GHz protocols; expansion is planned.

---

## Contribute or Stay Updated

This project is open-source, and contributions are welcome! Whether it's optimizing the code, suggesting features, or helping with testing, your input is valuable. Follow updates and access source code at **[GitHub Repository Link Placeholder]**.

Happy hacking!
