#include <DHT.h>
#include <ESP8266WiFi.h>

String apiKey = "3V3VXCEJ7CWA2B6K";

const char *ssid = "M4TCHA24";
const char *pass = "244466666";
const char* server = "api.thingspeak.com";

#define DHTPIN D3
DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup(){
  Serial.begin(9600);
  delay(10);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop(){ 
  float k = dht.readHumidity();
  float s = dht.readTemperature();
  if (isnan(k) || isnan(s)){
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
  if (client.connect(server,80)){
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(s);
    postStr +="&field2=";
    postStr += String(k);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.print("Temperature: ");
    Serial.print(s);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(k);
    Serial.println("%. Send to Thingspeak.");
  }
  client.stop();
  Serial.println("Waiting...");
  delay(1000);
}