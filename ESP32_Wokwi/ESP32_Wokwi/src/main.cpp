#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHTesp.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* servidor = "http://host.wokwi.internal:5000/dados";

DHTesp dht;
const int DHT_PIN = 15;

void setup() {
  Serial.begin(115200);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  Serial.println("Conectando ao Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WI-FI OK!");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  TempAndHumidity dados = dht.getTempAndHumidity();

  float temperatura = dados.temperature;
  float umidade = dados.humidity;

  Serial.println("CHEGUEI NO HTTP");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("INICIANDO HTTP...");

    WiFiClient cliente;
    HTTPClient http;

    Serial.println("CRIANDO CONEXAO...");

    http.begin(cliente, servidor);

    Serial.println("HTTP BEGIN OK!");

    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"temperatura\":" + String(temperatura, 2) + ",";
    json += "\"umidade\":" + String(umidade, 2);
    json += "}";

    Serial.println("ENVIANDO POST...");

    int codigo = http.POST(json);

    Serial.print("Código HTTP: ");
    Serial.println(codigo);

    Serial.println("Resposta do Flask:");
    Serial.println(http.getString());

    http.end();

  } else {

    Serial.println("Wi-Fi desconectado!");

  }

  Serial.println("--------------------");

  delay(5000);
}