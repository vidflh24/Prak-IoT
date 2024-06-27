#include <FirebaseRealtime.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#define FIREBASE_REALTIME_URL "https://tes-prak-iot-default-rtdb.firebaseio.com/"
#define FIREBASE_REALTIME_SECRET "AIzaSyBWzmCm5VUBEuQcQy30T3vYX5DdIDha2dc"
#define CW   1900
#define STOP 1450
#define CCW  1000

char ssid[] = "M4TCHA24";
char pass[] = "244466666";

FirebaseRealtime firebaseRealtime;
Servo myservo;


//int pin = D8;
int pin1 = D8;

int val_servo = 0;
//int val_D8 = 0;
int val_D8 = 0;

void setup() {
  Serial.begin(9600);
  firebaseRealtime.begin(FIREBASE_REALTIME_URL, FIREBASE_REALTIME_SECRET, ssid, pass);
  myservo.attach(D2);
  //pinMode(pin, INPUT);
  pinMode(pin1, INPUT);
  digitalWrite(pin1, LOW);
  myservo.writeMicroseconds(STOP);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop() {
  //Fetch
  //val_D8 = digitalRead(pin);
  val_D8 = digitalRead(pin1);
  /*if(val_D8 == HIGH){
    Serial.println("Menyala");
  }*/

  if(val_D8 == HIGH){
    Serial.println("Menyala");
  }

  DynamicJsonDocument fetchDoc(1024);
  int fetchResponseCode = firebaseRealtime.fetch("Prak_IoT", "", fetchDoc);
  int val_fetch = fetchDoc["Servo"];
  Serial.println("\nFetch - response code: " + String(fetchResponseCode));
  Serial.println("Servo value: " + String(val_fetch));
  val_servo = val_fetch;
  if (val_servo == 1){
    myservo.writeMicroseconds(CW);
  }
  else if(val_servo == 2){
    myservo.writeMicroseconds(CCW);
  }
  else{
    myservo.writeMicroseconds(STOP);
  }
  fetchDoc.clear();
}
