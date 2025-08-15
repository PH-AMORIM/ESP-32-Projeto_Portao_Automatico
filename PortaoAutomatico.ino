#include <WiFi.h>
#include <WebServer.h>
#include <RCSwitch.h>
#include "esp_wifi.h"  // Adicionado para controle de potência

const char* ssid = "ControlePortao_ESP32";
const char* password = "PHAMORIM22";

WebServer server(80);
RCSwitch rfTransmitter = RCSwitch();

const int rfPin = 5;

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  // Aumenta a potência máxima de transmissão Wi-Fi
  esp_wifi_set_max_tx_power(78);

  Serial.println("🔹 Rede Wi-Fi criada:");
  Serial.println(WiFi.softAPIP());

  rfTransmitter.enableTransmit(rfPin);
  rfTransmitter.setPulseLength(350);
  rfTransmitter.setProtocol(1);
  rfTransmitter.setRepeatTransmit(10);

  server.on("/enviar", HTTP_GET, []() {
    if (server.hasArg("codigo")) {
      String codigoStr = server.arg("codigo");
      unsigned long codigo = codigoStr.toInt();

      Serial.print("🔹 Código recebido: ");
      Serial.println(codigo);

      rfTransmitter.send(codigo, 24);

      server.send(200, "text/plain", "✅ Código RF enviado com sucesso!");
    } else {
      server.send(400, "text/plain", "❌ Parâmetro 'codigo' ausente");
    }
  });

  server.begin();
  Serial.println("🚀 Servidor HTTP iniciado!");
}

void loop() {
  server.handleClient();
}
