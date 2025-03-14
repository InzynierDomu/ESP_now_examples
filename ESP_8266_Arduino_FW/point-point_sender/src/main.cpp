#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

const uint8_t receiver_address[] = {0x50, 0x02, 0x91, 0xD5, 0xF7, 0xC3};

struct esp_now_message
{
  double value;
};

esp_now_message message_content;
size_t msg_size = sizeof(message_content);

void print_msg_info(uint8_t* mac_addr, uint8_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ERR_OK ? "Delivery Success" : "Delivery Fail");
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

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(print_msg_info);

  if (esp_now_add_peer((uint8_t*)receiver_address, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != ERR_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()
{
  message_content.value = 13.0; // dummy value
  uint8_t result = esp_now_send((uint8_t*)receiver_address, (uint8_t*)&message_content, msg_size);

  if (result == ERR_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }
  delay(2000);
}