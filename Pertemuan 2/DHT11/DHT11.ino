#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Uji coba DHT11");
  dht.begin();

}

void loop() {
  delay(2000);
  float kelambaban = dht.readHumidity();
  float celcius_ = dht.readTemperature();
  float farenheit_ = dht.readTemperature(true);

  Serial.print("Kelambaban : ");
  Serial.print(kelambaban);
  Serial.print(" %\t");

  Serial.print("Suhu : ");
  Serial.print(celcius_);
  Serial.print(" C/");
  Serial.print(farenheit_);
  Serial.println(" F/");
}
