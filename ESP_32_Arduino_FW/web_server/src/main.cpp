#include "config.h"

#include <WebServer.h>
#include <WiFi.h>
#include <esp_now.h>


struct msg_card_id
{
  uint8_t card_id[4];
};

// Tworzenie serwera HTTP
WebServer server(80);

// Ostatnia odebrana wiadomość
String lastMessage = "Brak danych";

msg_card_id message_content;

// Obsługa odbioru danych ESP-NOW
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len)
{
  Serial.print("msg received");
  memcpy(&message_content, incomingData, sizeof(message_content));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println("tag rfid: ");
  lastMessage = "";
  for (size_t i = 0; i < 4; i++)
  {
    Serial.println(message_content.card_id[i]);
    lastMessage += String(message_content.card_id[i]);
    lastMessage += ":";
  }
  Serial.println("Nowa wiadomość: " + lastMessage);
}

// Strona HTML
String getPage()
{
  return "<!DOCTYPE html>\
    <html>\
    <head>\
    <meta charset='UTF-8'>\
    <title>ESP32 - Odbiór ESP-NOW</title>\
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
    <h1>Odebrana wiadomość:</h1>\
    <p id='message'>" +
         lastMessage + "</p>\
    </body>\
    </html>";
}

// Obsługa strony głównej
void handleRoot()
{
  server.send(200, "text/html", getPage());
}

// Obsługa endpointu z danymi ESP-NOW
void handleData()
{
  server.send(200, "text/plain", lastMessage);
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA); // Przełącz na tryb stacji (klienta)
  WiFi.begin(config::ssid, config::password);
  Serial.print("Łączenie z WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nPołączono z siecią!");
  Serial.print("Adres IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // Inicjalizacja ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Błąd inicjalizacji ESP-NOW!");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // Start serwera HTTP
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop()
{
  server.handleClient(); // Obsługa żądań HTTP
}
