#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

struct esp_now_message
{
  double value;
};

esp_now_message message_content;

void print_received_msg(uint8_t* mac, uint8_t* incoming_data, uint8_t len)
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

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ERR_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(print_received_msg);
}

void loop() {}