#include "config.h"

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <stdint.h>


struct msg_card_id
{
  uint8_t card_id[4];
};

struct msg_measurement
{
  double value;
};

esp_now_peer_info_t peerInfo;

bool led_status = false;
long timer = 0;
const uint32_t m_on_led_time = 2000; ///< checking interval

msg_card_id message_content;

Adafruit_BME280 bme;

volatile bool msg_recived;

void send_msg()
{
  msg_measurement msg;
  msg.value = bme.readTemperature();
  Serial.print(msg.value);
  uint8_t _address[sizeof(config::master_address)];
  memcpy(_address, config::master_address, sizeof(config::master_address));
  esp_now_send(_address, (uint8_t*)&msg, sizeof(msg_measurement));
}

void received_msg_callback(const uint8_t* mac, const uint8_t* incomingData, int len)
{
  memcpy(&message_content, incomingData, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("tag rfid: ");
  Serial.println(message_content.card_id[0]);
  timer = millis();
  led_status = true;
  digitalWrite(LED_BUILTIN, HIGH);
  msg_recived = true;
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

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin(23, 19);
  if (bme.begin(0x76, &Wire))
  {
    Serial.println("BME280 working ok");
  }

  esp_now_register_recv_cb(received_msg_callback);

  memcpy(peerInfo.peer_addr, config::master_address, sizeof(config::master_address));
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
  if (led_status)
  {
    if (millis() - timer > m_on_led_time)
    {
      led_status = false;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  if (msg_recived)
  {
    send_msg();
    msg_recived = false;
  }
}