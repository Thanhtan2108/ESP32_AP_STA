#include <Arduino.h>
#include "WiFiManagerModule.h"
#include "SensorModule.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing Wi-Fi Module...");
  WiFiManagerModule_initWiFi(); // Khởi tạo STA + AP fallback

  initSensor();
}

void loop() {
  // Kiểm tra STA, reconnect nếu mất
  WiFiManagerModule_handleReconnect();

  float temp, humi;
  if (readSensor(&temp, &humi)) {
    Serial.print("Temperature (C): ");
    Serial.println(temp);
    Serial.print("Humidity (%): ");
    Serial.println(humi);
  } else {
    Serial.println("Sensor read failed!");
  }

  // Hoặc in JSON
  Serial.print("JSON: ");
  Serial.println(sensorDataToJSON());

  delay(3000); // đọc mỗi 3s
}
