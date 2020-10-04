// Biblioteca de wifi
#include <ESP8266WiFiMulti.h>
// Biblioteca de leitura de temperatura e umidade
#include "dht.h"
// Biblioteca InfluxDB
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// Configuração Wi-fi
#define WIFI_SSID "Nome da Rede"
#define WIFI_PASSWORD "Senha da rede"


// Configuração InfluxDB
#define INFLUXDB_URL "http://Ip da máquina onde o influx está instalado:9999"
// Token de autenticação do InfluxDB (Use: InfluxDB UI -> Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN "Token de autentificação"
// Id da organização (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "Id Organização"
// Id do bucket do InfluxDB(Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "Bucket"

// Definição Time Zone
#define TZ_INFO "BRST+3BRDT+2,M10.3.0,M2.3.0"

// Instancia do cliente influx
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// Instância do Wifi
ESP8266WiFiMulti wifiMulti;

// Definição do pino para leitura
const int pinoDHT11 = D7;

// Instância DHT 
dht DHT;

// Definição do nome da measurement
Point sensor("Sensor 1");


void setup() 
  {

    // Conectando Wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    // Sincronização Time Zone
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  }

void loop() 
  {

    // Limpeza dos Fields
    sensor.clearFields();

    // Leitura dos dados de Temperatura e Umidade
    DHT.read11(pinoDHT11);

    // Adicionando os Fields de Umidade de Temperatura
    sensor.addField("Umidade",DHT.humidity);
    sensor.addField("Temperatura", DHT.temperature);

    // Transforma os dado para o padrão Line Protocol
    sensor.toLineProtocol();

    // Insere os dados no InfluxDB
    client.writePoint(sensor);    

    // Delay de 10 segundos
    delay(10000);
  }
