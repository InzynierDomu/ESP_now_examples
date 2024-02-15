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

const uint8_t pin_rst = 9; // Pin RST - D3
const uint8_t pin_ss = 10; // Pin SS (CS) - D8
const uint8_t screen_width = 128;
const uint8_t screen_height = 64;

Adafruit_SSD1306 display(screen_width, screen_height, &Wire, -1);

MFRC522 rfid(pin_ss, pin_rst);

const byte my_card_id[4] = {0xE0, 0x64, 0xA7, 0x19};

uint8_t slave_address_1[] = {0x3C, 0xE9, 0x0E, 0x7F, 0x30, 0x80};
uint8_t slave_address_2[] = {0x3C, 0xE9, 0x0E, 0x7F, 0x30, 0x58};

msg_measurement message_content;
volatile bool value_changed;

void On_data_sent(uint8_t* mac_addr, uint8_t status)
{
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(
      macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");
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
    if (uid[i] != my_card_id[i])
    {
      return false;
    }
  }
  return true;
}

void setup()
{
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  value_changed = false;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(received_msg_callback);

  // esp_now_register_send_cb(On_data_sent);

  esp_now_add_peer(slave_address_1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(slave_address_2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  SPI.begin();
  rfid.PCD_Init();
}

void loop()
{
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
  {
    if (is_card_my_card(rfid.uid.uidByte))
    {
      msg_card_id msg;
      msg.card_id[0] = 19; // TODO put whole id
      Serial.println("Karta RFID o identyfikatorze E0 64 A7 19 została zbliżona do czytnika");
      auto result = esp_now_send(slave_address_1, (uint8_t*)&msg, sizeof(msg_card_id));
      Serial.print("msg status:");
      Serial.println(result);
      result = esp_now_send(slave_address_2, (uint8_t*)&msg, sizeof(msg_card_id));
      Serial.print("msg status:");
      Serial.println(result);
    }
    else
    {
      Serial.println("Karta RFID o identyfikatorze innym niż E0 64 A7 19 została zbliżona do czytnika");
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
