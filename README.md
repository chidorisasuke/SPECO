# SPECO - Smart Agriculture System

<p align="center">
  <img src="URL_TO_YOUR_PROJECT_IMAGE_OR_DEMO.png" alt="SPECO Device" width="450"/>
</p>

<p align="center">
  <strong>🏆 3rd Place Winner - National Essay Competition 🏆</strong>
</p>

<p align-="center">
  A multi-functional prototype designed for smart agriculture, capable of pest control, crop monitoring, and self-powering via a solar-tracking system. This project was awarded 3rd place and recommended for real-world implementation.
</p>

---

## About The Project

As a two-person team in a national essay competition, we proposed the **SPECO** ("Smart Pest Control") concept to address key challenges in modern farming. I was responsible for bringing our concept to life by designing and building this fully functional physical prototype.

SPECO is an all-in-one IoT device for paddy fields that integrates three core systems:
1.  A pest detection and deterrence system.
2.  A soil moisture monitoring system.
3.  A solar panel with an active sun-tracking system for energy self-sufficiency.

This repository contains the complete firmware for the ESP32-based device.

## Key Features

- **Automated Pest Deterrence:** Uses a PIR sensor to detect motion and an ultrasonic sensor to gauge distance. When a pest is identified, it triggers a buzzer and a servo-driven mechanism to scare it away.
- **Solar Tracking System:** A 2-axis solar tracker built with four LDRs and two servo motors automatically adjusts its position to maximize sun exposure throughout the day.
- **Soil Moisture Monitoring:** An onboard sensor provides real-time data on soil humidity, helping to optimize irrigation for paddy crops.
- **Dynamic Multi-Mode Operation:** The device cleverly uses an ultrasonic sensor to switch between its main functions based on the proximity of objects, optimizing its focus and power usage.
- **On-Device Status Display:** A 16x2 I2C LCD screen provides real-time feedback on the current operational mode and sensor data.

## Hardware and Technology

| Component | Purpose |
| :--- | :--- |
| **Microcontroller** | ESP32 |
| **Sensors** | PIR Motion Sensor, HC-SR04 Ultrasonic, Soil Moisture Sensor, 4x LDRs |
| **Actuators** | 3x Servo Motors, 5V Buzzer |
| **Display** | 16x2 I2C LCD Screen |
| **Firmware** | C++ / Arduino Framework |

## System Logic Explained

The core of SPECO's intelligence is its ability to switch modes based on its surroundings, measured by the ultrasonic sensor.

- **Mode A: Soil Monitoring (`Distance > 100 cm`)**
  When the area is clear, the device defaults to monitoring the soil moisture, providing crucial data for crop health.

- **Mode B: Pest Detection (`40 cm < Distance <= 100 cm`)**
  When an object approaches a medium distance, the system primes its pest detection capabilities, actively monitoring for motion with the PIR sensor.

- **Mode C: Solar Tracking (`Distance <= 40 cm`)**
  When an object is very close (or by default in bright light), the device focuses on optimizing its solar panel's orientation to charge its power source.

## Getting Started

To get this project running on your own hardware, follow these steps.

### Prerequisites

- **Hardware:** You will need all the components listed in the table above.
- **Software:** [Arduino IDE](https://www.arduino.cc/en/software) with the ESP32 board manager installed.
- **Libraries:**
  - `LiquidCrystal_I2C`
  - `ESP32Servo`
  - `WiFi` (though not used in the main logic, it's included for future expansion)

### Pinout

| Component | ESP32 Pin |
| :--- | :--- |
| PIR Sensor | `GPIO 19` |
| Ultrasonic TRIG | `GPIO 13` |
| Ultrasonic ECHO | `GPIO 12` |
| Soil Moisture | `GPIO 23` |
| Buzzer | `GPIO 14` |
| LDR Top-Left | `GPIO 36` |
| LDR Top-Right | `GPIO 39` |
| LDR Down-Left | `GPIO 35` |
| LDR Down-Right | `GPIO 34` |
| Base Servo | `GPIO 25` |
| Vertical Servo | `GPIO 26` |
| Pest Servo | `GPIO 27` |
| I2C LCD | SDA/SCL Pins |

### Installation

1.  **Clone the repo:**
    ```sh
    git clone [https://github.com/chidorisasuke/SPECO.git](https://github.com/chidorisasuke/SPECO.git)
    ```
2.  **Setup Hardware:** Connect all the components to the ESP32 according to the pinout table above.
3.  **Install Libraries:** In the Arduino IDE, go to `Sketch` > `Include Library` > `Manage Libraries...` and install the libraries listed in the prerequisites.
4.  **Upload Code:** Open the `.ino` file, select your ESP32 board and COM port, and click upload.

## Contact

Yahya Bachtiar - yahyabachtiar03@gmail.com

Project Link: https://github.com/chidorisasuke/SPECO
