#include "config.h"

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <espnow.h>

struct msg_card_id
{
  uint8_t card_id[4];
};

struct msg_measurement
{
  double value;
};

Adafruit_SSD1306 display(config::screen_width, config::screen_height, &Wire, -1);

MFRC522 rfid(config::pin_ss, config::pin_rst);

msg_measurement message_content;
volatile bool value_changed;

unsigned long last_card_time = 0;
const unsigned long card_time_threshold = 5000;

void On_data_sent(uint8_t* mac_addr, uint8_t status)
{
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(
      macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  if (status)
  {
    Serial.println("Delivery Fail");
  }
  else
  {
    Serial.println("Delivery Success");
  }
}

void received_msg_callback(uint8_t* mac, uint8_t* incomingData, uint8_t len)
{
  memcpy(&message_content, incomingData, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("measurement: ");
  Serial.println(message_content.value);
  value_changed = true;
}

bool is_card_my_card(byte uid[])
{
  for (int i = 0; i < 4; i++)
  {
    if (uid[i] != config::my_card_id[i])
    {
      return false;
    }
  }
  return true;
}

void send_msg(const uint8_t* adress, uint8_t* payload, int lenght)
{
  uint8_t _address[6];
  memcpy(_address, adress, 6);
  auto result = esp_now_send(_address, payload, lenght);
  Serial.println(result == 0 ? "Delivery Success" : "Delivery Fail");
}

void connect(const uint8_t* adress)
{
  uint8_t _address[sizeof(adress)];
  memcpy(_address, adress, sizeof(adress));
  esp_now_add_peer(_address, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void setup()
{
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("testing");
  display.display();

  value_changed = false;

  WiFi.mode(WIFI_OFF);
  delay(100);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(received_msg_callback);

  connect(config::slave_address_1);
  connect(config::slave_address_2);

  SPI.begin();
  rfid.PCD_Init();
}

void loop()
{
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
  {
    if (millis() - last_card_time >= card_time_threshold)
    {
      if (is_card_my_card(rfid.uid.uidByte))
      {
        Serial.println("The correct card put on card scanner");
        send_msg(config::slave_address_1, rfid.uid.uidByte, rfid.uid.size);
        send_msg(config::slave_address_2, rfid.uid.uidByte, rfid.uid.size);
      }
      else
      {
        Serial.println("The incorrect card put on card scanner");
      }
      last_card_time = millis();
    }
  }

  if (value_changed)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(message_content.value);
    display.display();
    value_changed = false;
  }
}
