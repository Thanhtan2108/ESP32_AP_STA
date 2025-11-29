# MODULE THƯ VIỆN HIỂN THỊ OLED 0.96 INCH

Hiển thị dữ liệu nhiệt độ, độ ẩm đo được từ cảm biến lên màn hình OLED

## Phát triển code từ các API hỗ trợ thư viện có sẵn trong thư viện `Adafruit_SSD1306.h`

- Cần định nghĩa kích thước màn hình sử dụng theo chiều rộng và chiều cao là 128 x 64 pixel và địa chỉ của màn hình

```cpp
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
```

- Khởi tạo 1 đối tượng đại diện cho màn hình sử dụng từ thư viện `Adafruit_SSD1306.h`

```cpp
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
```

- Khởi tạo bắt đầu giao tiếp I2C cho màn hình

```cpp
Wire.begin(SDA_PIN, SCL_PIN); // 21, 22
```

- Khởi tạo cho màn hình bắt đầu hoạt động

```cpp
display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
```

- Xóa mọi thứ trên màn hình để hiển thị lại từ đầu (reset màn)

```cpp
display.clearDisplay();
```

- Cho phép hiển thị lên màn hình

```cpp
display.display();
```

- Thiết lập cỡ chữ hiển thị trên màn hình

```cpp
display.setTextSize(size);
```

- Set màu chữ

```cpp
display.setTextColor(color);
```

- Thiết lập vị trí con trỏ trên màn, vị trí bắt đầu hiển thị nội dung

```cpp
display.setCursor(row, col);
```

- Vẽ hình tròn trên màn hình

```cpp
display.fillCircle(x, y, r, color);
```
