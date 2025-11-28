# MODULE PHÁT TRIỂN ĐỂ KẾT NỐI WIFI Ở CHẾ ĐỘ AP + STA

## Chế độ WiFi AP

Là chế độ WiFi do ESP tự tạo ra và phát ra

Đây là chế độ khi ESP cần kết nối WiFi nhưng chưa kết nối được vào mạng WiFi router

ESP sẽ tạo ra 1 địa chỉ IP AP và người dùng có thể truy cập vào đây để kết nối WiFi cho ESP

## Chế độ WiFi STA

Là chế độ kết nối WiFi của ESP vào mạng WiFi router

## Tối ưu và lựa chọn tốt cho IoT

- Ban đầu, Board ESP chưa kết nối được với WiFi STA, vì thế nó sẽ tự cung cấp 1 địa chỉ IP AP và tên của WiFi AP.

- Thực hiện trên thiết bị kết nối là Điện thoại thông minh, Laptop, PC,... tiến hành kết nối vào WiFi thông qua tên WiFi AP này.

-> Sẽ có thông báo internet không có sẵn hoặc đại loại vậy

- Mở trình duyệt web và nhập vào địa chỉ IP AP. Nó sẽ mở WebServer

- Tại đây chọn option `ConfigureWiFi` để kết nối với mạng WiFi router, nhập mật khẩu và `Save`

- ESP32 sẽ được kết nối với WiFI router

## Quy trình code phát triển

- Dùng thư viện `WiFi.h` được tích hợp sẵn trong core Arduino

- Dùng thư viện `WiFiManager.h` được cài đặt

- Đầu tiên, cấu hình cho board vào chế độ AP + STA để board có thể phát WiFi ở chế độ AP và đồng thời kết nối với WiFi router STA

  - Khởi tạo 1 đối tượng từ WiFiManager

  ```cpp
  static WiFiManager wm; // dùng static để chỉ định đối tượng này là private
  ```
  
  - Thiết lập chế độ

  ```cpp
  WiFi.mode(MODE_AP_STA);
  ```

- Bắt đầu phát WiFi AP với tên của WiFi AP

```cpp
WiFi.softAP("ESP32_AP");
```

- Lấy địa chỉ IP của WiFi AP

```cpp
WiFi.softAPIP();
```

- Tự động kết nối WiFi AP khi mất kết nối

```cpp
wm.autoConnect("ESP32_AP");
```

- Mở portal để cấu hình WiFi router mới

```cpp
wm.startConfigPortal("ESP32_AP");
```

- Kiểm tra trạng thái kết nối WiFi router

```cpp
WiFi.status() == WL_CONNECTED; // kết nối thành công
WiFi.status() != WL_CONNECTED; // kết nối không thành công
```

- Lấy địa chỉ IP của WiFi router

```cpp
WiFi.localIP();
```

- Tự động kết nối WiFi router lại khi mất kết nối

```cpp
WiFi.reconnect();
```
