#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* ssid = "o.o";
const char* password = "mauapakamuu";  // Ganti dengan kata sandi WiFi Anda

#define BOTtoken "6455135701:AAHfNHuaplhh-LrzwF-L0Zt58Ovh9PcN9rU"
#define CHAT_ID "-4150412540"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledSherina = D1;
const int ledNatalie = D2;
const int ledIrsyadul = D3;
const int ledPin = D4;

#define DHTPIN D6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

String idIrsyadul = "5059302058";
String idSherina = "1158427420";
String idNatalie = "1613086303";

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    String member_id = bot.messages[i].from_id;

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    Serial.println(text);

    if (text == "/led_1") {
      if (member_id == idIrsyadul) {
        bot.sendMessage(chat_id, "LED state set to ON", "");
        digitalWrite(ledIrsyadul, HIGH);
      } else {
        bot.sendMessage(chat_id, "Bukan Milik Anda", "");
      }
    }
    if (text == "/led_1_off") {
      if (member_id == idIrsyadul) {
        bot.sendMessage(chat_id, "LED state set to OFF", "");
        digitalWrite(ledIrsyadul, LOW);
      } else {
        bot.sendMessage(chat_id, "Bukan Milik Anda", "");
      }
    }

    if (text == "/led_2") {
      if (member_id == idSherina) {
        bot.sendMessage(chat_id, "LED state set to ON", "");
        digitalWrite(ledSherina, HIGH);
      } else {
        bot.sendMessage(chat_id, "Bukan Milik Anda", "");
      }
    }
    if (text == "/led_2_off") {
      if (member_id == idSherina) {
        bot.sendMessage(chat_id, "LED state set to OFF", "");
        digitalWrite(ledSherina, LOW);
      } else {
        bot.sendMessage(chat_id, "Bukan Milik Anda", "");
      }
    }

    if (text == "/led_3") {
      if (member_id == idNatalie) {
        bot.sendMessage(chat_id, "LED state set to ON", "");
        digitalWrite(ledNatalie, HIGH);
      } else {
        bot.sendMessage(chat_id, "Bukan Milik Anda", "");
      }
    }
    if (text == "/led_3_off") {
      if (member_id == idNatalie) {
        bot.sendMessage(chat_id, "LED state set to OFF", "");
        digitalWrite(ledNatalie, LOW);
      } else {
        bot.sendMessage(chat_id, "Bukan Milik Anda", "");
      }
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      digitalWrite(ledPin, HIGH);
    }

    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      digitalWrite(ledPin, LOW);
    }
    
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (text == "/dht_status"){
      String message = "";
      message += "Suhu: " + String(t) + "°C\n";
      message += "Kelembaban: " + String(h) + " %";
      bot.sendMessage(chat_id, message,"");
    } 
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);
  #endif

  pinMode(ledSherina, OUTPUT);
  pinMode(ledNatalie, OUTPUT);
  pinMode(ledIrsyadul, OUTPUT);
  pinMode(ledPin, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());String welcome = "Welcome to IoT Telegram Bot,\n\n"; 
  welcome += "List of Commands:\n"; 
  welcome += "/led_1 : Untuk Menyalakan LED 1 (Irsyadul)\n"; 
  welcome += "/led_1_off : Untuk Mematikan LED 1 (Irsyadul)\n"; 
  welcome += "/led_2 : Untuk Menyalakan LED 2 (Sherina)\n"; 
  welcome += "/led_2_off : Untuk Mematikan LED 2 (Sherina)\n"; 
  welcome += "/led_3 : Untuk Menyalakan LED 3 (Natalie)\n"; 
  welcome += "/led_3_off : Untuk Mematikan LED 3 (Natalie)\n"; 
  welcome += "/led_on : Untuk Menyalakan LED Utama\n"; 
  welcome += "/led_off : Untuk Mematikan Lampu Utama\n"; 
  welcome += "/dht_status : Melihat Suhu dan Kelembaban\n"; 
  bot.sendMessage(CHAT_ID, welcome);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}