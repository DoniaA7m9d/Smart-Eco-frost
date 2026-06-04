// **
#define BLYNK_TEMPLATE_ID   "xxxxxxxx"   
#define BLYNK_TEMPLATE_NAME "Greenhouse"     
#define BLYNK_AUTH_TOKEN    "YOUR.BLYNK.AUTH.TOKEN"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

#define BOTtoken "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID  "YOUR_TELEGRAM_CHAT_ID"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

RTC_DATA_ATTR int chillingHoursCounter = 0;
RTC_DATA_ATTR int fungusRiskCounter    = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  
  WiFi.begin(ssid, pass);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    timeout++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    goToSleep(5);
    return;
  }

  
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect(3000);

  client.setInsecure();

  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    goToSleep(15);
    return;
  }

  float dewPoint = temperature - ((100 - humidity) / 5);

  if (temperature < 7.0) {
    chillingHoursCounter++;
  }

  bool highFungusRisk = false;
  if (humidity >= 85.0 && temperature >= 15.0 && temperature <= 25.0) {
    fungusRiskCounter++;
    if (fungusRiskCounter >= 4) {
      highFungusRisk = true;
    }
  } else {
    fungusRiskCounter = 0;
  }

  if (Blynk.connected()) {
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, dewPoint);
    Blynk.virtualWrite(V4, chillingHoursCounter);
    Blynk.run();
  }

  if (temperature <= 4.0) {
    sendFrostAlert(temperature, humidity, dewPoint);
    goToSleep(5);
  } else if (highFungusRisk) {
    sendFungusAlert(temperature, humidity);
    fungusRiskCounter = 0;
    goToSleep(15);
  } else {
    goToSleep(15);
  }
}

void loop() {}

void sendFrostAlert(float temp, float hum, float dew) {
  
  String message = "*تحذير عاجل: خطر الصقيع!*\n\n";
  message += "الحرارة: " + String(temp, 1) + " C\n";
  message += "الرطوبة: " + String(hum, 1) + "%\n";
  message += "نقطة الندى: " + String(dew, 1) + " C\n\n";
  message += "*التوصيات:*\n";
  message += "1- تشغيل الري بالتنقيط فوراً.\n";
  message += "2- استخدام اغطية الاجروفايبر.\n";
  message += "3- رش مركبات السيلكا او البوتاسيوم.";
  bot.sendMessage(CHAT_ID, message, "Markdown");
}

void sendFungusAlert(float temp, float hum) {
  String message = "*تنبيه: خطر الفطريات مرتفع!*\n\n";
  message += "الرطوبة: " + String(hum, 1) + "% في بيئة دافئة لفترة طويلة.\n\n";
  message += "*التوصيات:*\n";
  message += "1- تهوية البيوت المحمية فوراً.\n";
  message += "2- ايقاف الري مؤقتاً.\n";
  message += "3- فحص الاوراق السفلية والتحضير لرش وقائي.";
  bot.sendMessage(CHAT_ID, message, "Markdown");
}

void goToSleep(int minutes) {
  esp_sleep_enable_timer_wakeup((uint64_t)minutes * 60 * 1000000ULL);
  esp_deep_sleep_start();
  }
