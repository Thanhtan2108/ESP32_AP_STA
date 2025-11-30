#include "WebServerModule.h"

// Server
static WebServer* server = nullptr;

// Cache dữ liệu sensor
static float cachedTemperature = 0.0f;
static float cachedHumidity = 0.0f;

// ----------- HTTP Handlers -----------

// Trang HTML chính
static void handleRoot() {
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>ESP32 Sensor Dashboard</title>";
    html += "<style>";
    html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
    html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; display: flex; justify-content: center; align-items: center; padding: 10px; }";
    html += ".container { background: white; border-radius: 20px; box-shadow: 0 20px 60px rgba(0,0,0,0.3); padding: 30px; max-width: 900px; width: 100%; }";
    html += "h1 { color: #333; text-align: center; margin-bottom: 5px; font-size: clamp(24px, 5vw, 36px); }";
    html += ".subtitle { text-align: center; color: #888; margin-bottom: 30px; font-size: clamp(12px, 3vw, 16px); }";
    html += ".sensor-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; margin-bottom: 30px; }";
    html += ".sensor-card { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); border-radius: 15px; padding: clamp(20px, 5vw, 30px); color: white; text-align: center; box-shadow: 0 5px 15px rgba(102, 126, 234, 0.3); transition: transform 0.3s ease; }";
    html += ".sensor-card:hover { transform: translateY(-5px); box-shadow: 0 8px 20px rgba(102, 126, 234, 0.5); }";
    html += ".sensor-label { font-size: clamp(11px, 2.5vw, 14px); opacity: 0.9; margin-bottom: 10px; text-transform: uppercase; letter-spacing: 1px; font-weight: 600; }";
    html += ".sensor-value { font-size: clamp(32px, 8vw, 48px); font-weight: bold; margin-bottom: 5px; font-family: 'Courier New', monospace; }";
    html += ".sensor-unit { font-size: clamp(14px, 3vw, 18px); opacity: 0.8; }";
    html += ".info-box { background: linear-gradient(135deg, #f0f4ff 0%, #e8ecff 100%); border-left: 4px solid #667eea; border-radius: 12px; padding: clamp(15px, 3vw, 25px); font-size: clamp(12px, 2vw, 14px); color: #555; }";
    html += ".status { display: flex; align-items: center; gap: 10px; }";
    html += ".status-dot { width: 12px; height: 12px; border-radius: 50%; background: #4caf50; animation: pulse 2s infinite; flex-shrink: 0; }";
    html += ".status-text { font-weight: 600; color: #333; }";
    html += ".timestamp { margin-top: 12px; font-size: clamp(11px, 2vw, 13px); color: #999; font-style: italic; }";
    html += ".refresh-note { color: #888; font-size: clamp(11px, 2vw, 13px); margin-top: 15px; text-align: center; }";
    html += "@keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }";
    html += "@media (max-width: 600px) { .container { padding: 20px; } .sensor-grid { gap: 12px; margin-bottom: 20px; } }";
    html += "@media (min-width: 900px) { h1 { margin-bottom: 10px; } .sensor-grid { grid-template-columns: repeat(2, 1fr); } }";
    html += "</style>";
    html += "<script>";
    html += "function refreshData() {";
    html += "  fetch('/json').then(r => r.json()).then(d => {";
    html += "    document.getElementById('temp').textContent = d.temperature.toFixed(1);";
    html += "    document.getElementById('humi').textContent = d.humidity.toFixed(1);";
    html += "    document.getElementById('time').textContent = 'Cập nhật lúc: ' + new Date().toLocaleTimeString('vi-VN');";
    html += "  }).catch(e => console.log('Update error:', e));";
    html += "}";
    html += "setInterval(refreshData, 2000);";
    html += "refreshData();";
    html += "</script>";
    html += "</head>";
    html += "<body>";
    html += "<div class='container'>";
    html += "<h1>⚡ ESP32 Sensor</h1>";
    html += "<p class='subtitle'>Real-time Monitoring Dashboard</p>";
    html += "<div class='sensor-grid'>";
    html += "<div class='sensor-card'>";
    html += "<div class='sensor-label'>🌡️ Nhiệt độ</div>";
    html += "<div class='sensor-value' id='temp'>" + String(cachedTemperature, 1) + "</div>";
    html += "<div class='sensor-unit'>°C</div>";
    html += "</div>";
    html += "<div class='sensor-card'>";
    html += "<div class='sensor-label'>💧 Độ ẩm</div>";
    html += "<div class='sensor-value' id='humi'>" + String(cachedHumidity, 1) + "</div>";
    html += "<div class='sensor-unit'>%</div>";
    html += "</div>";
    html += "</div>";
    html += "<div class='info-box'>";
    html += "<div class='status'>";
    html += "<span class='status-dot'></span>";
    html += "<span class='status-text'>Trạng thái: Đang hoạt động</span>";
    html += "</div>";
    html += "<div class='timestamp' id='time'></div>";
    html += "<p class='refresh-note'>📊 Dữ liệu tự động cập nhật mỗi 2 giây</p>";
    html += "</div>";
    html += "</div>";
    html += "</body>";
    html += "</html>";
    server->send(200, "text/html; charset=UTF-8", html);
}

// JSON
static void handleJSON() {
    String json = "{\"temperature\":" + String(cachedTemperature, 1) +
                  ",\"humidity\":" + String(cachedHumidity, 1) + "}";
    server->send(200, "application/json", json);
}

// ----------- Public API -----------

void WebServerModule_init(uint16_t port) {
    if (server) delete server;       // xóa nếu đã tồn tại
    server = new WebServer(port);    // tạo mới trên heap
    server->on("/", handleRoot);
    server->on("/json", handleJSON);
}

void WebServerModule_start() {
    if(server) server->begin();
    Serial.println("[WebServerModule] HTTP server started");
}

void WebServerModule_loop() {
    if(server) server->handleClient();
}

void WebServerModule_setSensorData(float temperature, float humidity) {
    cachedTemperature = temperature;
    cachedHumidity = humidity;
}
