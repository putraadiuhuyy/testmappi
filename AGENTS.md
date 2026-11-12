# AGENTS.md

## Overview
- **Project Type**: Embedded IoT firmware for ESP32 using PlatformIO & Arduino framework.
- **Core Functionality**: Connects to Wi-Fi and publishes telemetry sensor data (pH, EC, light, temperature, humidity, water sensor, RSSI) via MQTT (`test.mosquitto.org`) using JSON (`ArduinoJson` v7).

## Tech Stack & Toolchain
- **Platform**: PlatformIO (`platform = espressif32`, `board = esp32dev`, `framework = arduino`)
- **Libraries**: 
  - `knolleary/PubSubClient@^2.8` (MQTT)
  - `bblanchon/ArduinoJson@^7.2.2` (JSON serialization)
  - `tzapu/WiFiManager@^2.0.17` (Wi-Fi provisioning via AP portal)

## Build & Test Commands
- **Build Firmware**: `pio run`
- **Upload Firmware**: `pio run --target upload`
- **Monitor Serial Output**: `pio device monitor` (Baud rate: `115200`)
- **Clean Build**: `pio run --target clean`

## Architecture & Code Structure
- `src/main.cpp`: Main firmware entry point handling MQTT communication, sensor telemetry payload construction, and loop logic.
- `include/wifi_manager_helper.h` & `src/wifi_manager_helper.cpp`: Wi-Fi connection handler using `WiFiManager`.
- `platformio.ini`: PlatformIO project configuration.
