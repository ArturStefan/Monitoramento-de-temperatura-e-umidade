#include <ESP8266WiFi.h>
#include "dht.h"

const char* ssid = "Nome da sua rede";
const char* password = "Senha da rede";

const int pinoDHT11 = D7;
 
dht DHT;
 
void setup()
  {

    Serial.begin(115200);
  
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
      }
  }
 
void loop()
  {
    DHT.read11(pinoDHT11);
    Serial.print("Umidade: ");
    Serial.print(DHT.humidity);
    Serial.print("%");
    Serial.print(" / Temperatura: ");
    Serial.print(DHT.temperature, 0);
    Serial.println("*C");
    delay(10000);
  }
