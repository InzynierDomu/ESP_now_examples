#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include "ESPNowW.h"

struct esp_now_message
{
  double value;
};

esp_now_message message_content;
uint8_t mac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

#ifdef ESP8266
void print_received_msg(uint8_t* mac, uint8_t* incoming_data, uint8_t len)
#elif ESP32
void print_received_msg(const uint8_t* mac, const uint8_t* incoming_data, int len)
#endif
{
  memcpy(&message_content, incoming_data, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("value: ");
  Serial.println(message_content.value);
  Serial.println("-----------");
}

void setup()
{
  Serial.begin(115200);

#ifdef ESP8266
  WiFi.mode(WIFI_STA); // MUST NOT BE WIFI_MODE_NULL
#elif ESP32
  WiFi.mode(WIFI_MODE_STA);
#endif

  if (ESPNow.init() != ERR_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  ESPNow.reg_recv_cb(print_received_msg);
}

void loop() {}