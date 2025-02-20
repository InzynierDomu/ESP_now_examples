#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

const uint8_t receiver_address[] = {0xC8, 0xC9, 0xA3, 0x92, 0x14, 0x0A};

struct esp_now_message
{
  double value;
};

esp_now_message message_content;
size_t msg_size = sizeof(message_content);

esp_now_peer_info_t peerInfo;

void print_msg_info(const uint8_t* mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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

  esp_now_register_send_cb(print_msg_info);

  memcpy(peerInfo.peer_addr, receiver_address, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()
{
  message_content.value = 13.0; //dummy value
  esp_err_t result = esp_now_send(receiver_address, (uint8_t*)&message_content, msg_size);

  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }
  delay(2000);
}