#include <Arduino.h>
#include "esp_task_wdt.h"
#include "WiFiManagerModule.h"
#include "SensorModule.h"
#include "DisplayModule.h"
#include "WebServerModule.h"
#include "ButtonModule.h"

// ====== CONFIGURATION ======
#define LED_PIN 2      // GPIO 2 tích hợp LED trên nhiều board ESP32
#define BUTTON_PIN 18  // GPIO 18 (chân thông thường, hỗ trợ interrupt)
// Kết nối: GPIO 19 nối Ground qua nút nhấn (hoặc LED pull-down)
// ===========================

unsigned long lastRead = 0;
const unsigned long interval = 3000;
unsigned long wifiInfoDisplayTime = 0;
const unsigned long wifiInfoDuration = 30000; // Hiển thị WiFi info trong 30s lúc khởi động
bool wifiInfoShowed = false; // Flag để theo dõi đã hiển thị WiFi info chưa
bool wifiWasConnected = false; // Theo dõi trạng thái WiFi lần trước

// Callback function khi nút được nhấn giữ
void onButtonReset() {
  Serial.println("\n[MAIN] ===== BUTTON RESET TRIGGERED (3 giay) =====");
  Serial.println("[MAIN] Resetting WiFi configuration...");
  
  // Hiển thị thông báo lên OLED
  DisplayModule_clear();
  DisplayModule_showMessage("RESET WiFi", "Dang xoa config...");
  delay(500);
  
  // Reset WiFi config
  WiFiManagerModule_resetWiFiConfig();
  
  // Hiển thị thông báo kết quả
  DisplayModule_showMessage("WiFi Reset OK", "Dang khoi dong AP");
  delay(2000);
  
  // Reset display flags để hiển thị WiFi info lại
  wifiInfoShowed = false;
  wifiWasConnected = false;
  wifiInfoDisplayTime = millis();
  
  Serial.println("[MAIN] ===== RESET COMPLETE - Waiting for WiFi connection =====\n");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Enable Task Watchdog: 30s timeout
  esp_task_wdt_init(30, true); // timeout 30s, reset on trigger
  esp_task_wdt_add(NULL);     // add current loop task to WDT

  // seed RNG sớm để clientId MQTT (nếu dùng random) sẽ khác nhau
  randomSeed(analogRead(0));

  Serial.println("Initializing Wi-Fi Module...");
  WiFiManagerModule_initWiFi(); // Khởi tạo STA + AP fallback

  // Init display: truyền pin SDA, SCL (ví dụ cho ESP32 devkit: SDA=21, SCL=22)
  if (!DisplayModule_init(21, 22, 0x3C)) {
    Serial.println("[MAIN] Display init failed");
  } else {
    Serial.println("[MAIN] Display init ok");
  }

  // Khởi tạo sensor
  initSensor();

  // Init WebServer
  WebServerModule_init(80);
  WebServerModule_start();
  
  // Init Button Module
  ButtonModule_init(BUTTON_PIN);
  ButtonModule_setResetCallback(onButtonReset);
  Serial.println("[MAIN] Button Module initialized on GPIO " + String(BUTTON_PIN));
  
  // Bắt đầu hiển thị WiFi info
  wifiInfoDisplayTime = millis();
}

void loop() {
  // Kick watchdog
  esp_task_wdt_reset();

  // Xử lý Button
  ButtonModule_loop();

  // Xử lý request HTTP (LUÔN xử lý, kể cả khi WiFi mất)
  WebServerModule_loop();

  // Kiểm tra STA, reconnect nếu mất
  WiFiManagerModule_handleReconnect();

  // Kiểm tra trạng thái WiFi
  bool wifiConnected = WiFiManagerModule_isWiFiConnected();
  
  // Nếu WiFi bị mất (từ connected -> disconnected), reset để hiển thị WiFi info lại
  if (wifiWasConnected && !wifiConnected) {
    Serial.println("[ALERT] WiFi disconnected! Showing WiFi info again...");
    wifiInfoShowed = false;
    wifiInfoDisplayTime = millis();
  }
  wifiWasConnected = wifiConnected;
  
  if (wifiConnected) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Blink LED khi có WiFi
    delay(500);
  }

  // Hiển thị WiFi info trong 10s lúc khởi động hoặc khi WiFi mất
  if (!wifiInfoShowed && (millis() - wifiInfoDisplayTime) < wifiInfoDuration) {
    char sta_ip[16], ap_ip[16];
    IPAddress sta_addr = WiFiManagerModule_getLocalIP();
    IPAddress ap_addr = WiFiManagerModule_getAPIP();
    
    snprintf(sta_ip, sizeof(sta_ip), "%d.%d.%d.%d", 
             sta_addr[0], sta_addr[1], sta_addr[2], sta_addr[3]);
    snprintf(ap_ip, sizeof(ap_ip), "%d.%d.%d.%d", 
             ap_addr[0], ap_addr[1], ap_addr[2], ap_addr[3]);
    
    DisplayModule_showWiFiInfo(sta_ip, ap_ip, wifiConnected);
  } else {
    wifiInfoShowed = true;
  }

  // Đọc sensor & display mỗi 3s (sau khi hiển thị WiFi info)
  if (wifiInfoShowed && (millis() - lastRead >= interval)) {
    lastRead = millis();
    float temp = 0.0f, humi = 0.0f;

    if (readSensor(&temp, &humi)) {
      Serial.printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp, humi);
      
      // Cập nhật Web Server (luôn cập nhật dữ liệu từ sensor)
      WebServerModule_setSensorData(temp, humi);
      
      // Luôn hiển thị sensor data
      DisplayModule_showSensor(temp, humi);
      
      Serial.print("JSON: ");
      Serial.println(sensorDataToJSON());
    } else {
      Serial.println("[ERROR] Sensor read failed");
      DisplayModule_clear();
      DisplayModule_showSensor(-1, -1);
    }
  }
}
