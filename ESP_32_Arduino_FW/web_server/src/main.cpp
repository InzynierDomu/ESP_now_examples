#include "config.h"

#include <WebServer.h>
#include <WiFi.h>
#include <esp_now.h>

struct msg_card_id
{
  uint8_t card_id[4];
};

WebServer server(80);

String lastMessage = "Brak danych";

msg_card_id message_content;

void OnDataRecv(const uint8_t* mac, const uint8_t* incoming_data, int len)
{
  Serial.print("msg received");
  memcpy(&message_content, incoming_data, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println("RFID tag: ");
  lastMessage = "";
  for (size_t i = 0; i < 4; i++)
  {
    Serial.println(message_content.card_id[i]);
    lastMessage += String(message_content.card_id[i]);
    lastMessage += ":";
  }
  Serial.println("New message: " + lastMessage);
}
String getPage()
{
  return "<!DOCTYPE html>\
    <html>\
    <head>\
    <meta charset='UTF-8'>\
    <title>ESP32 - ESP-NOW Receiver</title>\
    <script>\
        function updateData() {\
            fetch('/data')\
            .then(response => response.text())\
            .then(data => {\
                document.getElementById('message').innerText = data;\
            });\
        }\
        setInterval(updateData, 2000); \
    </script>\
    </head>\
    <body>\
    <h1>Received message:</h1>\
    <p id='message'>" +
         lastMessage + "</p>\
    </body>\
    </html>";
}
void handleRoot()
{
  server.send(200, "text/html", getPage());
}

void handleData()
{
  server.send(200, "text/plain", lastMessage);
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(config::ssid, config::password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to network!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop()
{
  server.handleClient();
}
