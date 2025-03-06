
#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include "ESPNowW.h"

uint8_t receiver_mac[] = {0x3C, 0xE9, 0x0E, 0x7F, 0x30, 0x80};

void setup()
{
  Serial.begin(115200);
  Serial.println("ESPNow sender Demo");
#ifdef ESP8266
  WiFi.mode(WIFI_STA); // MUST NOT BE WIFI_MODE_NULL
#elif ESP32
  WiFi.mode(WIFI_MODE_STA);
#endif
  WiFi.disconnect();
  ESPNow.init();
  ESPNow.add_peer(receiver_mac);
}

void loop()
{
  static uint8_t a = 0;
  delay(100);
  ESPNow.send_message(receiver_mac, &a, 1);
  Serial.println(a++);
}
