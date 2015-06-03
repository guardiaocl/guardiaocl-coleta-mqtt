#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <dht.h>

#define DHT11_PIN 7

byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 54,175,116,131 }; //IP referente ao guardiao.cl

void callback(char* topic, byte* payload, unsigned int length) {
  
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);
dht DHT;

float tempC = 0.0;
float humidity = 0.0;

String clientName = String("IOTWBS0001");
String topicName = String("IOTWBS/collect/IOTWBS0001"); //Prefixo do Projeto + Canal de Coleta + Serial do Dispositivo

void setup()
{
  Ethernet.begin(mac);
  Serial.begin(9600);
}

void loop()
{
  char clientStr[34];
  clientName.toCharArray(clientStr,34);
  char topicStr[26];
  topicName.toCharArray(topicStr,26);
  //Lendo dados do sensor
  DHT.read11(DHT11_PIN);
  humidity = (float)DHT.humidity;
  tempC = (float)DHT.temperature;

  if (!client.connected()) {
    client.connect(clientStr,"suporte@webonesystem.com.br","497aa991-d8e6-4bb6-ad17-f01a13648333");
  }
  if (client.connected() ) {
    String json = buildJson();
    char jsonStr[200];
    json.toCharArray(jsonStr,200);
    boolean pubresult = client.publish(topicStr,jsonStr);
    if (pubresult)
      Serial.println("successfully sent");
    else
      Serial.println("unsuccessfully sent");
  }
  delay(5000);
}

String buildJson() {
  String data = "{";
  data+="\n";
  data+= "\"data\": {";
  data+="\n";
  data+="\"temperatura\": ";
  data+=(int)tempC;
  data+= ",";
  data+="\n";
  data+="\"humidade\": ";
  data+=(int)humidity;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}

