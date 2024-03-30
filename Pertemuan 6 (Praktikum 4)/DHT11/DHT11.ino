#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

// Wifi network username dan password
#define WIFI_SSID "..."
#define WIFI_PASSWORD "123456789"
// Telegram BOT Token (dapat dari Botfather)
#define BOT_TOKEN "7184300444:AAHW2oNDXGnPi8Y_YKqT-jatN5Z8l16aUUQ"
// Chat ID untuk mengirim pesan
#define CHAT_ID "5321354535" // Ganti dengan ID chat Anda

const unsigned long BOT_MTBS = 1000; // waktu tunggu pesan

X509List cert(TELEGRAM_CERTIFICATE_ROOT); //SSL Certificate
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // penanda waktu terakhir

const int dhtPin = D1; //Pin DHT11 , sesuaikan
#define DHTTYPE DHT11 //Sensor DHT11
DHT dht(dhtPin, DHTTYPE);

String SendTempHumid()
{
    //pembacaan suhu dan kelembaban
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    /*if (isnan(h) || isnan(t))
    {
        Serial.println("Failed to read from DHT sensor!");
        t = 0;
        h = 0;
    }*/

    Serial.print("Humi: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.println(" °C ");

    return ("Suhu : " + String(t) + " C Kelembaban :" + String(h) + " %rH");
}

// routine untuk mengecek pesan masuk dari bot
void handleNewMessages(int numNewMessages)
{
    Serial.print("handleNewMessages ");
    Serial.println(numNewMessages);

    for (int i=0; i<numNewMessages; i++) {
      //memeriksa ID telegram yang mengirimkan pesan
      String chat_id = String(bot.messages[i].chat_id);
      if (chat_id != CHAT_ID){
        bot.sendMessage(chat_id, "Unauthorized user", "");
        continue;
      }
      //cetak pesan yang masuk pada Serial Monitor
      String user_text = bot.messages[i].text;
      Serial.println(user_text);
      String your_name = bot.messages[i].from_name;
      //Membuat pesan instruksi (welcome message)
      if (user_text == "/start") {
        String welcome = "Selamat datang, " + your_name + ".\n";
        welcome += "Gunakan perintah '/status' untuk membaca DHT11.\n\n";
        bot.sendMessage(chat_id, welcome, "");
      }
      if (user_text == "/status"){
         bot.sendMessage(chat_id, "\n" + SendTempHumid(), "");
      }
    }
}

void setup() 
{
    Serial.begin(9600);
    Serial.println();
    // attempt to connect to Wifi network:
    configTime(0, 0, "pool.ntp.org");       // get UTC time via NTP
    secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
    Serial.print("Connecting to Wifi SSID ");
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.print("\nWiFi connected. IP address: ");
    Serial.println(WiFi.localIP());
    // Check NTP/Time, usually it is instantaneous and you can delete the code below.
    Serial.print("Retrieving time: ");
    time_t now = time(nullptr);
    while (now < 24 * 3600)
    {
        Serial.print(".");
        delay(100);
        now = time(nullptr);
    }
    Serial.println(now);
}

void loop()
{
    if (millis() - bot_lasttime > BOT_MTBS)
    {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages)
        {
            Serial.println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }

        bot_lasttime = millis();
    }
}