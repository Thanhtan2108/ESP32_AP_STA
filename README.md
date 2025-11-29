# BÁO CÁO CHÍNH CỦA PROJECT

## RESET FLASH CỦA ESP32

- Kết nối board với laptop

- Trên terminal của PlatformIO, chạy lệnh:

```bash
pio run --target erase
```

- Flash ESP32 sẽ được làm sạch như ban đầu

## Phát triển các module chức năng cho project

- Project được chia ra thành từng giai đoạn nhỏ để phát triển cho từng chức năng nhỏ cụ thể

- Dễ test và debug

\*Lưu ý quan trọng:

- Thư viện có thể được định nghĩa để tránh gọi lặp lại nhiều lần làm lỗi hệ thống

```cpp
#pragma once // khuyến khích
```

hoặc

```cpp
#ifndef ...
#define ...

#endif
```

- Vì code theo chuẩn procedural program C (chỉ có variable, prototype function và định nghĩa function), nhưng đây là cách code trong C nhưng xây dựng project bằng các file.cpp, trinh biên dịch trong cpp sẽ mã hóa các tên hàm khiến trình biên dịch c không hiểu. vì thể cần xây dựng 1 chuẩn để bảo vệ nguyên vẹn tên hàm c trong cpp bao bọc các prototype function

```cpp
#ifdef __cplusplus
extern "C" {
#endif

// các prototye function

#ifdef __cplusplus
}
#endif
```

- Quá trình phát triển như sau:

### Kết nối WiFi cho board ESP32

- [Chi tiết nội dung xem tại đây](./lib/WiFiManagerModule/README.md)

- [Code thư viện xem tại đây](./lib/WiFiManagerModule)

### Giao tiếp cảm biến DHT11

- [Chi tiết nội dung xem tại đây](./lib/SensorModule/README.md)

- [Code thư viện xem tại đây](./lib/SensorModule)

### Giao tiếp Oled 0.96"

- [Chi tiết nội dung xem tại đây](./lib/DisplayModule/README.md)

- [Code thư viện xem tại đây](./lib/DisplayModule)
