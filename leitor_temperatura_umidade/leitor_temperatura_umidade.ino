// Biblioteca de wifi
#include <ESP8266WiFiMulti.h>
// Biblioteca de leitura de temperatura e umidade
#include "dht.h"
// Biblioteca InfluxDB
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// Configuração Wi-fi
#define WIFI_SSID "Nome da rede"
#define WIFI_PASSWORD "Senha rede"


// Configuração InfluxDB
#define INFLUXDB_URL "http://Ip da máquina onde o InfluxDB está instalado:9999"
// Token de autenticação do InfluxDB (Use: InfluxDB UI -> Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN "Token de autenficação do InfluxDB"
// Id da organização (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "ID Organização"
// Id do bucket do InfluxDB(Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "ID do Bucket"

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

    // Adicionando os Fields contendo os Metadados
    sensor.addField("Nome","Sensor 1");
    sensor.addField("Modelo MCU","Wemos D1 R1");
    
    sensor.addField("Modelo Sensor","DHT 11");
    sensor.addField("Unidade de medida temperatura","Celsius");
    sensor.addField("Unidade de medida umidade","Umidade Relativa");
    sensor.addField("Faixa medição temperatura"," 0° a 50°");
    sensor.addField("Faixa medição umidade"," 20% a 90%");
   
    sensor.addField("Margem de erro temperatura","± 2.0 ºC");
    sensor.addField("Margem de erro umidade","± 5,0% UR");
    sensor.addField("Tempo de resposta","2s");
    
    sensor.addField("Data Instalação","04/10/2020");
    sensor.addField("Data Última Troca Sensor","04/10/2020");
    sensor.addField("Data Última Troca MCU","04/10/2020");

  
    // Adicionando os Fields de Umidade de Temperatura
    sensor.addField("Umidade",DHT.humidity);
    sensor.addField("Temperatura", DHT.temperature);

    // Transforma os dados para o padrão Line Protocol
    sensor.toLineProtocol();

    // Insere os dados no InfluxDB
    client.writePoint(sensor);    

    // Delay de 10 segundos
    delay(10000);
  }
