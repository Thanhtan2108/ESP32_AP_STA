#ifndef WiFiManagerModule_h
#define WiFiManagerModule_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

#ifdef __cplusplus
extern "C" {
#endif

// Khởi tạo Wi-Fi: STA + AP fallback
void WiFiManagerModule_initWiFi();

// Kiểm tra kết nối STA
bool WiFiManagerModule_isWiFiConnected();

// Lấy IP STA
IPAddress WiFiManagerModule_getLocalIP();

// Force mở portal để đổi Wi-Fi
void WiFiManagerModule_startConfigPortal();

// Nên gọi trong loop để tự reconnect nếu STA mất
void WiFiManagerModule_handleReconnect();

#ifdef __cplusplus
}
#endif

#endif
