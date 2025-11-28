# MODULE THƯ VIỆN CẢM BIẾN DHT

Mục tiêu đo được nhiệt độ, độ ẩm thông qua các API có sẵn từ thư viện `DHT.h`

- Chỉ định chân kết nối cảm biến với board

```cpp
#define DHT_PIN 4
```

- Chỉ định loại cảm biến DHT sử dụng

```cpp
#define DHTTYPE DHT11
```

- Khởi tạo 1 đối tượng đại diện cho cảm biến từ thư viện DHT

```cpp
static DHT dht(DHT_PIN, DHTTYPE);
```

- Bắt đầu kích hoạt quá trình giao tiếp, khởi động cảm biến

```cpp
dht.begin();
```

- Đọc độ ẩm từ cảm biến

```cpp
dht.readHumidity();
```

- Đọc nhiệt độ từ cảm biến

```cpp
dht.readTemperature();
```

- **Lưu ý: cầ xử lý NaN cho dữ liệu**

```cpp
if (isnan(dht.readHumidity()) || isnan(dht.readTemperature())) {
    return false;
}
```
