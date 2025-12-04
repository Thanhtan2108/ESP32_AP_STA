#include <Arduino.h>
#include "WiFiManagerModule.h"
#include "SensorModule.h"
#include "DisplayModule.h"
#include "WebServerModule.h"

unsigned long lastRead = 0;
const unsigned long interval = 3000;
unsigned long wifiInfoDisplayTime = 0;
const unsigned long wifiInfoDuration = 30000; // Hiển thị WiFi info trong 30s lúc khởi động
bool wifiInfoShowed = false; // Flag để theo dõi đã hiển thị WiFi info chưa
bool wifiWasConnected = false; // Theo dõi trạng thái WiFi lần trước

void setup() {
  Serial.begin(115200);
  delay(1000);

  // seed RNG sớm để clientId MQTT (nếu dùng random) sẽ khác nhau
  randomSeed(analogRead(0));

  Serial.println("Initializing Wi-Fi Module...");
  WiFiManagerModule_initWiFi(); // Khởi tạo STA + AP fallback

  // Init display: truyền pin SDA, SCL (ví dụ cho ESP32 devkit: SDA=21, SCL=22)
  if (!DisplayModule_init(21, 22, 0x3C)) {
    Serial.println("Display init failed");
  } else {
    Serial.println("Display init ok");
  }

  // Khởi tạo sensor
  initSensor();

  // Init WebServer
  WebServerModule_init(80);
  WebServerModule_start();
  
  // Bắt đầu hiển thị WiFi info
  wifiInfoDisplayTime = millis();
}

void loop() {
  // Kick watchdog
  esp_task_wdt_reset();

  // Xử lý request HTTP (LUÔN xử lý, kể cả khi WiFi mất)
  WebServerModule_loop();

  // Kiểm tra STA, reconnect nếu mất
  WiFiManagerModule_handleReconnect();

  // Nếu chưa có Wi-Fi STA, đừng cố connect MQTT
  if (!WiFiManagerModule_isWiFiConnected()) {
    // vẫn có thể hiển thị trên OLED hoặc chờ kết nối
    delay(1000);
    return;
  }

  // Gọi MQTT loop để xử lý keepalive & callback
  MQTTModule_loop();

  float temp = 0.0f, humi = 0.0f;
  if (readSensor(&temp, &humi)) {
    Serial.printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp, humi);
    // Hiển thị lên OLED nếu có
    DisplayModule_showSensor(temp, humi);

    // Publish dữ liệu lên MQTT
    if (MQTTModule_publishSensor(temp, humi)) {
      Serial.println("[MAIN] Published sensor data to MQTT");
    } else {
      Serial.println("[MAIN] Publish failed (will retry)");
    }
  } else {
    Serial.println("Sensor read failed");
  }

  // Hoặc in JSON
  Serial.print("JSON: ");
  Serial.println(sensorDataToJSON());

  delay(3000); // đọc mỗi 3s
}
