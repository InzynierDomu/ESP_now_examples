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
  Serial.print("msg sending");
  msg_measurement msg;
  msg.value = bme.readTemperature();
  Serial.print(msg.value);
  esp_now_send(peerInfo.peer_addr, (uint8_t*)&msg, sizeof(msg_measurement));
}

void received_msg_callback(const uint8_t* mac, const uint8_t* incomingData, int len)
{
  Serial.print("msg received");
  memcpy(&message_content, incomingData, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println("tag rfid: ");
  for (size_t i = 0; i < 4; i++)
  {
    Serial.println(message_content.card_id[i]);
  }
  timer = millis();
  led_status = true;
  digitalWrite(LED_BUILTIN, HIGH);
  msg_recived = true;
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  while (!Serial)
  {}
  Serial.println("serial ready");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin();
  if (bme.begin(0x76, &Wire))
  {
    Serial.println("BME280 working ok");
  }
  else
  {
    Serial.println("BME280 error");
  }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
  }
  esp_now_register_recv_cb(received_msg_callback);

  memcpy(peerInfo.peer_addr, config::master_address, sizeof(config::master_address));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
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