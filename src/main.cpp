#include <Arduino.h>
#include "WiFiManagerModule.h"
#include "SensorModule.h"
#include "DisplayModule.h"
#include "MQTTModule.h"

// cấu hình MQTT (thay bằng broker/port/topic của bạn)
static const char* MQTT_BROKER = "test.mosquitto.org";
static const uint16_t MQTT_PORT = 1883;
static const char* MQTT_PUB_TOPIC = "esp32/sensor";

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

  // Init MQTT (khởi tạo client)
  if (!MQTTModule_init(MQTT_BROKER, MQTT_PORT, MQTT_PUB_TOPIC)) {
    Serial.println("[MAIN] MQTT init failed (unexpected)");
  } else {
    Serial.println("[MAIN] MQTT initialized");
  }

  // Kết nối MQTT LẦN ĐẦU nhưng CHỈ khi WiFi STA đã connect
  if (WiFiManagerModule_isWiFiConnected()) {
    if (MQTTModule_connect()) {
      Serial.println("[MAIN] MQTT connected");
    } else {
      Serial.println("[MAIN] MQTT connect failed (will retry in loop)");
    }
  } else {
    Serial.println("[MAIN] WiFi STA not connected yet — MQTT connect deferred");
  }
}

void loop() {
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
