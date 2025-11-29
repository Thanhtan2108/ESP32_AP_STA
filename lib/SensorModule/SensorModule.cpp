#include "SensorModule.h"

// ====== CẤU HÌNH ======
// Chỉnh DHT_PIN nếu bạn nối cảm biến vào chân khác
#ifndef DHT_PIN
#define DHT_PIN 4
#endif

// Nếu dùng DHT11 để đọc DHTTYPE = DHT11, nếu DHT22 dùng DHT22
#ifndef DHTTYPE
#define DHTTYPE DHT11
#endif
// ======================

static DHT dht(DHT_PIN, DHTTYPE);

void initSensor(void) {
    Serial.print("Initializing DHT on pin ");
    Serial.print(DHT_PIN);
    Serial.print(" type ");
#if (DHTTYPE == DHT11)
    Serial.println("DHT11");
#elif (DHTTYPE == DHT22)
    Serial.println("DHT22");
#else
    Serial.println("UNKNOWN");
#endif
    dht.begin();
    Serial.println("Initialized successfully!");
    // Có thể chờ một chút để cảm biến ổn định
    delay(1500);
}

bool readSensor(float* temperature, float* humidity) {
    if (temperature == NULL || humidity == NULL) return false;

    // Đọc (đơn giản, blocking) — Adafruit DHT library
    float h = dht.readHumidity();
    float t = dht.readTemperature(); // default Celsius

    // Kiểm tra giá trị hợp lệ
    if (isnan(h) || isnan(t)) {
        Serial.println("DHT read failed (NaN)");
        return false;
    }

    *temperature = t;
    *humidity = h;
    return true;
}

SensorData_t readSensorStruct(void) {
    SensorData_t r;
    r.temperature = NAN;
    r.humidity = NAN;
    r.valid = false;

    float t, h;
    if (readSensor(&t, &h)) {
        r.temperature = t;
        r.humidity = h;
        r.valid = true;
    }
    return r;
}

String sensorDataToJSON(void) {
    SensorData_t d = readSensorStruct();
    if (!d.valid) {
        return String("{\"error\":\"read_failed\"}");
    }
    // Tạo chuỗi JSON đơn giản (không dùng ArduinoJson để tránh phụ thuộc)
    // giữ 1 decimal place
    char buf[80];
    // format with one decimal place
    snprintf(buf, sizeof(buf), "{\"temperature\":%.1f,\"humidity\":%.1f}", d.temperature, d.humidity);
    return String(buf);
}
