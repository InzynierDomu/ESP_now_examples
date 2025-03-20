# ESP_now_examples
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/InzynierDomu/ESP_now_examples/main.yml?logo=github&style=flat-square)
![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/InzynierDomu/ESP_now_examples?style=flat-square)
<a href="https://discord.gg/KmW6mHdg">![Discord](https://img.shields.io/discord/815929748882587688?logo=discord&logoColor=green&style=flat-square)</a>
![GitHub](https://img.shields.io/github/license/InzynierDomu/ESP_now_examples?style=flat-square)

## About
This repository contains a set of simple projects that demonstrate how to use ESP-NOW. The projects are designed to be easy to understand and follow, even for beginners. The projects can be used as a starting point for learning about ESP-NOW, or they can be modified to meet your specific needs.

<div align="center">
<h2>Support</h2>

<p>If any of my projects have helped you in your work, studies, or simply made your day better, you can buy me a coffee. <a href="https://buycoffee.to/inzynier-domu" target="_blank"><img src="https://buycoffee.to/img/share-button-primary.png" style="width: 195px; height: 51px" alt="Postaw mi kawę na buycoffee.to"></a></p>
</div>

## Examples list
### Framework Arduino
* get_mac - To pair boards and send messages to specific boards, you will need the MAC addresses of this boards.
#### ESP8266
* point-point_sender - send message via esp now to reciver
* point-point_receiver - receive message via esp now
* Central - reads rfid tag, sends message to specific boards ESP32/slave example, receives temperature measurements and displays them
* Central_broadcast - reads rfid tag, sends broadcast message(to all), receives temperature measurements and displays them
* Central_to_server - reads rfid tag, message to specific boards ESP32/web_server example
#### ESP32
* point-point_sender - send message via esp now to reciver
* point-point_receiver - receive message via esp now
* slave - Receives a message from the ESP8266/Central and replies with a temperature measurement
* web_server - Receives a message from the ESP8266/Central and shows incoming rfid code on web server
#### Wrapper
Versions with a wrapper that works for both ESP8266 and ESP32
* point-point_sender - send message via esp now to reciver
* point-point_receiver - receive message via esp now
### ESP IDF 
TBD
  
## Schem
### Slave
| BME280      | ESP32 Lolin      |
|----------------|----------------|
| GND  | GND  |
| VCC  | 3.3V  |
| SCL  | 23  |
| SDA  | 19 |

### Central

| SSD1306 (OLED)    | EVW-008 (ESP8266)     |
|----------------|----------------|
| GND  | GND  |
| VCC  | 3.3V  |
| SCL  | SCL (GPIO5) |
| SDA  | SDA (GPIO4) |

| MFRC522 (RFID)     | EVW-008 (ESP8266)     |
|----------------|----------------|
| GND  | GND  |
| VCC  | 3.3V  |
| SCK  | CLK |
| SDA  | IO10 (GPIO10) |
| MOSI  | MOSI |
| MISO  | MISO |
| RST  | IO9 (GPIO9) |

## Manual
To change mac addresses, network names etc. use the config files in the project folders.

## IDE
The project is prepared for the Platform IO environment. A video on how to install such an environment can be watched on this [video PL](https://youtu.be/Em9NuebT2Kc).
<br><br>

Formatting is done using clang-format. The description of the tool configuration is in the [video PL](https://youtu.be/xxuaOG0WjIE).
<br><br>