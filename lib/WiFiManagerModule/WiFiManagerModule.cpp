#include "WiFiManagerModule.h"

// Global WiFiManager instance
static WiFiManager wm;

void WiFiManagerModule_initWiFi() {
    Serial.println("Initializing Wi-Fi Module...");
    Serial.println();

    // Chế độ STA + AP
    WiFi.mode(WIFI_AP_STA);

    // AutoConnect: nếu không có Wi-Fi lưu sẽ mở AP portal
    if (!wm.autoConnect("ESP32_AP")) {
        Serial.println("*wm: AutoConnect failed or no Wi-Fi saved");
        Serial.println("*wm: Starting AP fallback");
        WiFi.softAP("ESP32_AP");
        Serial.print("*wm: AP IP address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("*wm: STA connected successfully");
    }

    Serial.print("STA IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("ESP32 is connected to Wi-Fi STA.");
}

bool WiFiManagerModule_isWiFiConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

IPAddress WiFiManagerModule_getLocalIP() {
    return WiFi.localIP();
}

IPAddress WiFiManagerModule_getAPIP() {
    return WiFi.softAPIP();
}

// Force mở portal để cấu hình Wi-Fi mới
void WiFiManagerModule_startConfigPortal() {
    Serial.println("Starting config portal...");
    wm.startConfigPortal("ESP32_AP");
    Serial.print("STA IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}

// Nên gọi trong loop để tự reconnect khi mất STA
void WiFiManagerModule_handleReconnect() {
    if (!WiFiManagerModule_isWiFiConnected()) {
        Serial.println("STA lost, attempting reconnect...");
        WiFi.reconnect();
        delay(5000); // đợi 5s trước khi check lại
    }
}

// Reset WiFi config: xóa WiFi đã lưu, ngắt STA, mở AP portal
void WiFiManagerModule_resetWiFiConfig() {
    Serial.println("[WiFiManagerModule] ===== RESET WiFi CONFIG =====");
    
    // Xóa WiFi config đã lưu
    wm.resetSettings();
    Serial.println("[WiFiManagerModule] WiFi config erased");
    
    // Ngắt kết nối STA
    WiFi.disconnect(true); // true = tắt WiFi radio
    delay(500);
    Serial.println("[WiFiManagerModule] STA disconnected");
    
    // Bật lại WiFi AP + STA mode
    WiFi.mode(WIFI_AP_STA);
    delay(500);
    
    // Bật AP
    WiFi.softAP("ESP32_AP");
    Serial.println("[WiFiManagerModule] AP started: ESP32_AP");
    Serial.print("[WiFiManagerModule] AP IP: ");
    Serial.println(WiFi.softAPIP());
    
    // Mở config portal để người dùng kết nối mạng WiFi mới
    Serial.println("[WiFiManagerModule] Opening config portal for WiFi selection...");
    wm.startConfigPortal("ESP32_AP");
    
    Serial.println("[WiFiManagerModule] ===== WiFi CONFIG RESET COMPLETE =====");
}

