#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Isikan nama SSID dan password
const char* ssid = "";
const char* password = "";

// Initialize Telegram BOT
#define BOTtoken "xxxxx" //Ganti dengan ID chat Anda

#define CHAT_ID "xxxxx" // Ganti dengan ID chat Anda

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//menentukan delay 1 detik untuk memeriksa apakah ada pesan baru
int bot_delay = 1000;
unsigned long lastTimeBotRan;

const int ledPin1 = D5; //LED L1 pada board NodeMCU
bool ledState1 = LOW; //memberikan logika 0 pada LED L1

//Ketika ada pesan telegram masuk
void handleNewMessages(int numNewMessages) {
  Serial.println("Handling New Message");
  Serial.println(String(numNewMessages));

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
    welcome += "Gunakan perintah berikut untuk menyalakan LED.\n\n";
    welcome += "Kirim /L1_nyala untuk menyalakan LED L1\n";
    welcome += "Kirim /L1_mati untuk mematikan LED L1\n";
    welcome += "Kirim /status untuk melihat status L1 \n";
    bot.sendMessage(chat_id, welcome, "");
  }
  if (user_text == "/L1_nyala") {
    bot.sendMessage(chat_id, "L1 dinyalakan", "");
    ledState1 = HIGH;
    digitalWrite(ledPin1, ledState1);
  }

  if (user_text == "/L1_mati") {
    bot.sendMessage(chat_id, "L1 dimatikan", "");
    ledState1 = LOW;
    digitalWrite(ledPin1, ledState1);
  }
  if (user_text == "/status") {
    if (digitalRead(ledPin1)){
      bot.sendMessage(chat_id, "L1 menyala", "");
      }
      else{
        bot.sendMessage(chat_id, "L1 mati", "");
      }
    }
  }
}
void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);
  #endif
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, ledState1);
  // koneksi ke Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
    // menampilkan lokal IP di Serial Monitor
  Serial.println(WiFi.localIP());
}
void loop() {
  if (millis() > lastTimeBotRan + bot_delay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("Got Response!");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
