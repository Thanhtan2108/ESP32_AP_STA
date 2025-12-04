#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#ifdef __cplusplus
extern "C" {
#endif

// Khởi tạo MQTT module (gọi 1 lần)
bool MQTTModule_init(const char* broker, uint16_t port, const char* pubTopic);

// Cố gắng kết nối; trả về true nếu connected
bool MQTTModule_connect();

// Gọi trong loop()
void MQTTModule_loop();

// Publish sensor với tuỳ chọn retained
bool MQTTModule_publishSensor(float temperature, float humidity, bool retained = false);

// Subscribe 1 topic (nếu cần)
bool MQTTModule_subscribe(const char* topic);

// Đăng ký callback xử lý message (signature trùng với PubSubClient callback)
typedef void (*mqtt_message_callback_t)(char* topic, byte* payload, unsigned int length);
void MQTTModule_setCallback(mqtt_message_callback_t cb);

// Đặt Last Will (topic, payload, retained)
void MQTTModule_setWill(const char* topic, const char* payload, bool retained);

#ifdef __cplusplus
}
#endif
