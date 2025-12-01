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

// Lấy IP AP
IPAddress WiFiManagerModule_getAPIP();

// Force mở portal để đổi Wi-Fi
void WiFiManagerModule_startConfigPortal();

// Nên gọi trong loop để tự reconnect nếu STA mất
void WiFiManagerModule_handleReconnect();

// Reset WiFi config: xóa WiFi đã lưu và mở lại AP portal
void WiFiManagerModule_resetWiFiConfig();

#ifdef __cplusplus
}
#endif

#endif

