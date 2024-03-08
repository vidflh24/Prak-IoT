#include<ThingSpeak.h>
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

unsigned long channel_num = 2458232;
const char* ssid = "M4TCHA24";
const char* password = "";
int led;
int led2;
unsigned int value;
WiFiClient client;
ESP8266WebServer server(80);

void setup() {
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.on("/", handleonconnect);
  ThingSpeak.begin(client);
  server.begin();

}

void loop() {
  server.handleClient();
  led = ThingSpeak.readFloatField(channel_num,1);
  if(led == 1){
    digitalWrite(14, HIGH);
  }
  else if (led == 0){
    digitalWrite(14, LOW);
  }

  led2 = ThingSpeak.readFloatField(channel_num,2);
  if(led2 == 1){
    digitalWrite(13, HIGH);
  }
  else if (led2 == 0){
    digitalWrite(13, LOW);
  }

}

void handleonconnect(){
  server.send(200,"text/html",SendHTML());
}

String SendHTML(void){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initialscale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button2 {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +=".button-on2 {background-color: #1abc9c;}\n";
  ptr +=".button-on2:active {background-color: #16a085;}\n";
  ptr +=".button-off2 {background-color: #34495e;}\n";
  ptr +=".button-off2:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Kendali 2 LED NodeMCU ESP8266 dengan ThingSpeak</h1>\n";
  ptr +="<h3>Mode Station (STA)</h3>\n";
  ptr +="<h4>Kendali LED D5</h4>\n";
  ptr +="<a class=\"button button-on\" href=\"https://api.thingspeak.com/update?api_key=ORJ3QVPK61L3PDMP&field1=1\">ON</a>\n";
  ptr +="<a class=\"button button-off\" href=\"https://api.thingspeak.com/update?api_key=ORJ3QVPK61L3PDMP&field1=0\">OFF</a>\n";
  ptr +="<h4>Kendali LED D7</h4>\n";
  ptr +="<a class=\"button button-on2\" href=\"https://api.thingspeak.com/update?api_key=ORJ3QVPK61L3PDMP&field2=1\">ON</a>\n";
  ptr +="<a class=\"button button-off2\" href=\"https://api.thingspeak.com/update?api_key=ORJ3QVPK61L3PDMP&field2=0\">OFF</a>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
