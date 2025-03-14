
#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include "ESPNowW.h"

uint8_t receiver_mac[] = {0x50, 0x02, 0x91, 0xD5, 0xF7, 0xC3};

void setup()
{
  Serial.begin(115200);
  Serial.println("ESPNow sender Demo");
#ifdef ESP8266
  WiFi.mode(WIFI_STA);
#elif ESP32
  WiFi.mode(WIFI_MODE_STA);
#endif
  WiFi.disconnect();
  ESPNow.init();
  ESPNow.add_peer(receiver_mac);
}

void loop()
{
  uint8_t a = 8; // dummy value
  delay(2000);
  ESPNow.send_message(receiver_mac, &a, 1);
  Serial.println(a++);
}
