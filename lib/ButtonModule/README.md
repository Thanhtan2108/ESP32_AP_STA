# ButtonModule - WiFi Reset Button

Module này cung cấp chức năng điều khiển nút nhấn để reset cấu hình WiFi.

## Chức năng

- **Phát hiện nút nhấn**: Giám sát GPIO pin được chỉ định
- **Hold detection**: Phát hiện khi nút được giữ trong 3 giây
- **Callback system**: Gọi callback khi nút được nhấn giữ đủ lâu
- **Non-blocking**: Xử lý không blocking, phù hợp cho loop()

## Cách sử dụng

### 1. Khởi tạo

```cpp
ButtonModule_init(GPIO_PIN);  // GPIO 0 là BOOT button trên ESP32 DevKit
ButtonModule_setResetCallback(onButtonReset);
```

### 2. Gọi trong loop()

```cpp
ButtonModule_loop();  // Gọi mỗi loop để xử lý logic nút nhấn
```

### 3. Định nghĩa callback

```cpp
void onButtonReset() {
    // Logic khi nút được nhấn giữ 3 giây
    WiFiManagerModule_resetWiFiConfig();
}
```

## Cấu hình

Sửa `BUTTON_HOLD_TIME` trong `ButtonModule.cpp` để thay đổi thời gian giữ nút:

```cpp
#define BUTTON_HOLD_TIME 3000  // 3 giây
```

## Hardware Connection

- **GPIO 0** (BOOT button) - Ground: Khi nhấn = GND (LOW)
- Nút nhấn sử dụng **INPUT_PULLUP** nên khi nhấn sẽ là LOW

## API Reference

- `ButtonModule_init(uint8_t button_pin)`: Khởi tạo
- `ButtonModule_loop()`: Xử lý logic (gọi trong loop)
- `ButtonModule_setResetCallback(ButtonCallback callback)`: Gán callback
- `ButtonModule_isPressed()`: Kiểm tra nút đang bị nhấn
- `ButtonModule_getPressedDuration()`: Lấy thời gian nhấn giữ (ms)

## Hoạt động với WiFiManagerModule

Khi nút được nhấn giữ 3 giây:

1. Display hiển thị "Resetting..."
2. Xóa WiFi config đã lưu
3. Ngắt kết nối STA
4. Bật lại AP (ESP32_AP)
5. Mở config portal cho phép chọn WiFi khác
6. Reset display flags để hiển thị WiFi info

## Lưu ý

- Chỉ trigger một lần khi giữ >= 3 giây
- Khi thả nút, output log thời gian nhấn
- Non-blocking, không ảnh hưởng hiệu năng chính
