#include <Arduino.h>
#include "WiFiManagerModule.h"
#include "SensorModule.h"
#include "DisplayModule.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing Wi-Fi Module...");
  WiFiManagerModule_initWiFi(); // Khởi tạo STA + AP fallback

  // Init display: truyền pin SDA, SCL (ví dụ cho ESP32 devkit: SDA=21, SCL=22)
  if (!DisplayModule_init(21, 22, 0x3C)) {
    Serial.println("Display init failed");
  } else {
    Serial.println("Display init ok");
  }

  initSensor();
}

void loop() {
  // Kiểm tra STA, reconnect nếu mất
  WiFiManagerModule_handleReconnect();

  float temp, humi;
  if (readSensor(&temp, &humi)) {
    Serial.printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp, humi);
    DisplayModule_showSensor(temp, humi);
  } else {
    Serial.println("Sensor read failed");
  }

  // Hoặc in JSON
  Serial.print("JSON: ");
  Serial.println(sensorDataToJSON());

  delay(3000); // đọc mỗi 3s
}
