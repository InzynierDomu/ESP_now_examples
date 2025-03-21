#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

struct esp_now_message
{
  double value;
};

esp_now_message message_content;

bool led_status = false;

void toggle_led()
{
  digitalWrite(LED_BUILTIN, led_status ? HIGH : LOW);
  led_status = !led_status;
}

void print_received_msg(const uint8_t* mac, const uint8_t* incoming_data, int len)
{
  memcpy(&message_content, incoming_data, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("value: ");
  Serial.println(message_content.value);
  Serial.println("-----------");
  toggle_led();
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

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {}