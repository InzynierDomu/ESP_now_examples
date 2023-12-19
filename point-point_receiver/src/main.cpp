#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

struct esp_now_message
{
  char text[32];
  int value;
  bool flag;
};

esp_now_message message_content;

void print_received_msg(const uint8_t* mac, const uint8_t* incoming_data, int len)
{
  memcpy(&message_content, incoming_data, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("text: ");
  Serial.println(message_content.text);
  Serial.print("value: ");
  Serial.println(message_content.value);
  Serial.print("flag: ");
  Serial.println(message_content.flag);
  Serial.println("-----------");
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(print_received_msg);
}

void loop() {}