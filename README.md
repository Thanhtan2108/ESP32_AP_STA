#

## RESET FLASH CỦA ESP32

- Kết nối board với laptop

- Trên terminal của PlatformIO, chạy lệnh:

```bash
pio run --target erase
```

- Flash ESP32 sẽ được làm sạch như ban đầu

## Kết nối WiFi cho board ESP32

- [Chi tiết nội dung xem tại đây](./lib/WiFiManagerModule/README.md)

- [Code thư viện xem tại đây](./lib/WiFiManagerModule)

## Giao tiếp cảm biến DHT11

- [Chi tiết nội dung xem tại đây](./lib/SensorModule/README.md)

- [Code thư viện xem tại đây](./lib/SensorModule)
