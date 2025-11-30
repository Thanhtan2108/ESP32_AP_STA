#ifndef WEBSERVER_MODULE_H
#define WEBSERVER_MODULE_H

#include <WebServer.h>
#include <Arduino.h>

// Khởi tạo server
void WebServerModule_init(uint16_t port = 80);

// Bắt đầu server
void WebServerModule_start();

// Xử lý request HTTP trong loop
void WebServerModule_loop();

// Cập nhật dữ liệu sensor để web server hiển thị
void WebServerModule_setSensorData(float temperature, float humidity);

#endif
