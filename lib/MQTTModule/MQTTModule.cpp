#include "MQTTModule.h"

// non-C++ users: keep static globals
static WiFiClient espClient;
static PubSubClient mqttClient(espClient);

static const char* mqtt_broker = nullptr;
static uint16_t mqtt_port = 1883;
static const char* mqtt_pub_topic = nullptr;

static mqtt_message_callback_t userCallback = nullptr;

// reconnect backoff state
static unsigned long lastConnectAttempt = 0;
static unsigned long connectDelayMs = 2000; // 2s initial

// default LWT params
static const char* will_topic = nullptr;
static const char* will_payload = nullptr;
static bool will_retained = false;

static void internalCallback(char* topic, byte* payload, unsigned int length) {
    if (userCallback) {
        userCallback(topic, payload, length);
    } else {
        // default debug print
        Serial.print("[MQTT] Message arrived on ");
        Serial.print(topic);
        Serial.print(": ");
        for (unsigned int i = 0; i < length; i++) Serial.print((char)payload[i]);
        Serial.println();
    }
}

bool MQTTModule_init(const char* broker, uint16_t port, const char* pubTopic) {
    if (!broker || !pubTopic) {
        Serial.println("[MQTT] ERROR: broker or pubTopic is null");
        return false;
    }
    mqtt_broker = broker;
    mqtt_port = port;
    mqtt_pub_topic = pubTopic;

    mqttClient.setClient(espClient);
    mqttClient.setServer(mqtt_broker, mqtt_port);
    mqttClient.setCallback(internalCallback);

    Serial.printf("[MQTT] inited broker=%s port=%u pubTopic=%s\n", mqtt_broker, mqtt_port, mqtt_pub_topic);
    return true;
}

void MQTTModule_setWill(const char* topic, const char* payload, bool retained) {
    will_topic = topic;
    will_payload = payload;
    will_retained = retained;
}

void MQTTModule_setCallback(mqtt_message_callback_t cb) {
    userCallback = cb;
}

// connect with basic backoff
bool MQTTModule_connect() {
    if (!mqtt_broker || !mqtt_pub_topic) {
        Serial.println("[MQTT] connect failed: not inited");
        return false;
    }

    if (mqttClient.connected()) return true;

    unsigned long now = millis();
    if (now - lastConnectAttempt < connectDelayMs) {
        return false; // chưa đến lúc thử lại
    }
    lastConnectAttempt = now;

    Serial.print("[MQTT] Connecting to broker... ");

    // tạo clientId ngẫu nhiên
    String clientId = "ESP32_";
    clientId += String((uint32_t)esp_random(), HEX);

    // nếu có LWT, đặt trước khi connect (PubSubClient connect có overload setLastWill? not direct)
    // PubSubClient::connect(clientId, willTopic, willQos, willRetain, willPayload);
    bool ok;
    if (will_topic && will_payload) {
        ok = mqttClient.connect(clientId.c_str(), NULL, NULL, will_topic, 0, will_retained, will_payload);
    } else {
        ok = mqttClient.connect(clientId.c_str());
    }

    if (ok) {
        Serial.println("OK");
        connectDelayMs = 2000; // reset delay
        // (re)subscribe if needed here
        return true;
    } else {
        int8_t state = mqttClient.state();
        Serial.printf("FAILED rc=%d\n", state);
        // Exponential backoff cap
        connectDelayMs = min(connectDelayMs * 2, 60000UL);
        return false;
    }
}

void MQTTModule_loop() {
    if (!mqttClient.connected()) {
        MQTTModule_connect();
    } else {
        mqttClient.loop();
    }
}

bool MQTTModule_publishSensor(float temperature, float humidity, bool retained) {
    if (!mqtt_pub_topic) return false;

    if (!mqttClient.connected()) {
        if (!MQTTModule_connect()) {
            Serial.println("[MQTT] publish fail: not connected");
            return false;
        }
    }

    // build payload (simple JSON)
    char payload[128];
    int n = snprintf(payload, sizeof(payload), "{\"temp\":%.1f,\"humi\":%.1f}", temperature, humidity);
    if (n <= 0 || n >= (int)sizeof(payload)) {
        Serial.println("[MQTT] payload error/too large");
        return false;
    }

    Serial.print("[MQTT] Publish: ");
    Serial.println(payload);

    // sử dụng overload có retained
    bool ok = mqttClient.publish(mqtt_pub_topic, payload, retained);
    if (!ok) {
        Serial.println("[MQTT] publish returned false");
    }
    return ok;
}

bool MQTTModule_subscribe(const char* topic) {
    if (!topic) return false;
    if (!mqttClient.connected()) {
        if (!MQTTModule_connect()) return false;
    }
    bool ok = mqttClient.subscribe(topic);
    Serial.printf("[MQTT] subscribe %s => %s\n", topic, ok ? "OK" : "FAILED");
    return ok;
}
