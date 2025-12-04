# MODULE THƯ VIỆN GIAO THỨC MQTT

## Kiểm tra dữ liệu MQTT: mở MQTT Explorer, subscribe esp32/sensor, quan sát dữ liệu nhảy mỗi 3 giây

### Bước 1: Tải và cài MQTT Explorer

- Truy cập: [https://mqtt-explorer.com/](https://mqtt-explorer.com/)

- Tải bản phù hợp với OS (Windows/macOS/Linux) → cài đặt như bình thường.

### Bước 2: Khởi chạy MQTT Explorer

- Mở phần mềm, bạn sẽ thấy màn hình Connection để tạo kết nối mới.

### Bước 3: Tạo kết nối tới broker

- Click “New Connection”.

- Điền các thông số broker mà bạn đã dùng trong code ESP32:

  - Name: `ESP32_Local` (tên bạn muốn)

  - Broker Address: `test.mosquitto.org`

  - Port: `1883`

  - Username / Password: để trống (vì broker test này public)

- Nhấn Connect.

### Bước 4: Subscribe topic

- Sau khi kết nối thành công, bạn sẽ thấy tree view các topic hoặc có thể subscribe thủ công:

  - Tìm hoặc nhập topic: `esp32/sensor`

  - Click Subscribe

### Bước 5: Quan sát dữ liệu

- Khi ESP32 đang chạy code của bạn, nó sẽ publish dữ liệu sensor mỗi 3 giây:

```json
{"temp":30.2,"humi":47.0}
{"temp":30.3,"humi":47.0}
{"temp":30.5,"humi":46.0}
```

- MQTT Explorer sẽ hiển thị realtime những payload này.

### Bước 6: Kiểm tra tính đúng đắn

- Dữ liệu nhảy mỗi 3 giây → khớp với delay trong loop.

- Dữ liệu JSON đúng format → kiểm tra key "temp" và "humi" có giá trị hợp lý.

- Nếu không nhận được dữ liệu:

  - Kiểm tra ESP32 đã kết nối Wi-Fi chưa (WiFiManagerModule_isWiFiConnected())

  - Kiểm tra broker đúng địa chỉ và port

  - Kiểm tra ESP32 có publish thành công: Serial Monitor sẽ show [MQTT] Publish: ...
