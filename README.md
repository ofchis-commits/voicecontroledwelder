# Welder Pulse Control System

## Hardware
- **Board**: Waveshare ESP32-P4-WiFi6-Touch-LCD-7B
- **Power Control**: 5kΩ Potentiometer
- **Output Range**: 0-15V pulse control
- **Communication**: Bluetooth (BLE)

## Features
- **Power Control**: Slider for output voltage setpoint (0-15V)
- **Pulse Frequency**: Adjustable slider (0.5-10 Hz)
- **Pulse Amplitude**: Slider from flat setpoint to ±20% above setpoint
- **Bluetooth Interface**: Remote control via BLE
- **Touch LCD Display**: Real-time monitoring and control

## Framework
- **Platform**: Espressif ESP-IDF (not Arduino)
- **Language**: C/C++
- **Build System**: CMake

## Project Structure
```
welder-pulse-control/
├── main/
│   ├── CMakeLists.txt
│   ├── idf_component.yml
│   ├── Kconfig.projbuild
│   └── src/
│       ├── main.c
│       ├── adc_control.c
│       ├── adc_control.h
│       ├── pwm_driver.c
│       ├── pwm_driver.h
│       ├── ble_server.c
│       ├── ble_server.h
│       ├── dac_control.c
│       ├── dac_control.h
│       ├── ui_display.c
│       └── ui_display.h
├── CMakeLists.txt
├── sdkconfig
├── sdkconfig.defaults
└── partitions.csv
```

## Getting Started

1. **Install ESP-IDF**:
   ```bash
   git clone https://github.com/espressif/esp-idf.git
   cd esp-idf
   ./install.sh
   . ./export.sh
   ```

2. **Clone and Build**:
   ```bash
   idf.py build
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

## Pin Configuration
- **ADC (Potentiometer)**: GPIO35 (ADC1_CH7)
- **DAC (Voltage Output)**: GPIO17 (DAC1) / GPIO18 (DAC2)
- **PWM (Pulse Control)**: GPIO1 (LEDC)
- **BLE**: Built-in ESP32 BLE controller
