#include <Arduino.h>
#include "WiFiManagerModule.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing Wi-Fi Module...");
  WiFiManagerModule_initWiFi(); // Khởi tạo STA + AP fallback
}

void loop() {
    // Kiểm tra STA, reconnect nếu mất
    WiFiManagerModule_handleReconnect();
    delay(1000);
}
