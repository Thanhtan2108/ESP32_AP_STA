#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifdef __cplusplus
extern "C" {
#endif

// Khởi tạo màn hình OLED (I2C). Gọi 1 lần trong setup().
bool DisplayModule_init(uint8_t i2c_sda_pin, uint8_t i2c_scl_pin, uint8_t i2c_addr);

// Hiển thị 2 giá trị: nhiet do (C) và do am (%).
// text in Vietnamese but without diacritics (no dấu)
void DisplayModule_showSensor(float temperature_c, float humidity_pct);

// Hiển thị thông tin WiFi (STA IP, AP IP)
void DisplayModule_showWiFiInfo(const char* sta_ip, const char* ap_ip, bool sta_connected);

// Hiển thị thông báo (title + message)
void DisplayModule_showMessage(const char* title, const char* message);

// Xóa màn hình
void DisplayModule_clear();

// Thay đổi kích cỡ font (optional)
void DisplayModule_setTextSize(uint8_t size);

#ifdef __cplusplus
}
#endif

