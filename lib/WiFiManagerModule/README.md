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
