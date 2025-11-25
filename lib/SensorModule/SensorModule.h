#pragma once

#include <Arduino.h>
#include <DHT.h>

#ifdef __cplusplus
extern "C" {
#endif

// Struct trả về dữ liệu cảm biến
typedef struct {
    float temperature;   // độ C
    float humidity;      // %RH
    bool valid;          // true nếu đọc thành công
} SensorData_t;

// Khởi tạo DHT sensor (gọi 1 lần trong setup)
void initSensor(void);

// Đọc sensor, trả về true nếu đọc thành công.
// Nếu thành công, temperature và humidity được gán giá trị.
bool readSensor(float* temperature, float* humidity);

// Đọc sensor và trả về JSON string: {"temperature":xx.x,"humidity":yy.y}
// Nếu đọc thất bại trả về {"error":"read_failed"}
String sensorDataToJSON(void);

// Tùy chọn: đọc và trả về struct
SensorData_t readSensorStruct(void);

#ifdef __cplusplus
}
#endif
