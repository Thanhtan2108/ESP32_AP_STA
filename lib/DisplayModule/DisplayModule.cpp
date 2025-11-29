#include "DisplayModule.h"

// Thông số mặc định cho màn hình 0.96" thường là 128x64 (hầu hết các module dùng SSD1306)
#ifndef SCREEN_WIDTH
  #define SCREEN_WIDTH 128
#endif
#ifndef SCREEN_HEIGHT
  #define SCREEN_HEIGHT 64
#endif

// Địa chỉ I2C mặc định 0x3C (thay đổi nếu module của bạn khác)
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Lưu addr nếu cần (không bắt buộc)
static uint8_t g_i2c_addr = 0x3C;

bool DisplayModule_init(uint8_t i2c_sda_pin, uint8_t i2c_scl_pin, uint8_t i2c_addr) {
    g_i2c_addr = i2c_addr;

    // Khởi tạo I2C (nếu muốn custom pins)
    Wire.begin(i2c_sda_pin, i2c_scl_pin);

    // Khởi tạo display
    if (!display.begin(SSD1306_SWITCHCAPVCC, (uint8_t)g_i2c_addr)) {
        // Failed to initialize
        Serial.println("SSD1306 allocation failed");
        return false;
    }

    display.clearDisplay();
    display.display();

    // Thiết lập mặc định
    display.setTextSize(2);        // kích cỡ chữ (1..)
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    return true;
}

void DisplayModule_clear() {
    display.clearDisplay();
    display.display();
}

void DisplayModule_setTextSize(uint8_t size) {
    display.setTextSize(size);
}

// Helper vẽ ký tự "độ" (°) bằng 1 vòng nhỏ (hoặc chấm) ở vị trí bên phải của số.
// Vì bảng font mặc định đôi khi không chứa ký tự degree, cách an toàn là vẽ vòng tròn nhỏ.
static void _drawDegreeSymbol(int x, int y) {
    // draw a small filled circle to mimic degree symbol
    // x,y là toạ độ top-left reference; ta sẽ vẽ vòng nhỏ ở gần góc trên phải của số
    display.fillCircle(x, y, 2, SSD1306_WHITE);
}

// Hiển thị sensor (nhiet do, do am) bằng tiếng việt không dấu.
// Ví dụ hiển thị:
// Nhiet do: 25.3°C
// Do am  : 60%
void DisplayModule_showSensor(float temperature_c, float humidity_pct) {
    char buf[32];

    display.clearDisplay();

    // Dòng 1: "Nhiet do:" và giá trị
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Nhiet do:");

    // In giá trị nhiệt độ ở phía phải (căn chỉnh vị trí)
    // Tạo chuỗi value
    snprintf(buf, sizeof(buf), "%.1f", temperature_c);
    // đặt vị trí con trỏ ở dòng 2 (hoặc ở x phù hợp)
    // Chúng ta in giá trị lớn hơn ở dưới để rõ:
    display.setCursor(0, 22); // y=22 phù hợp cho text size 2
    display.print(buf);

    // Vẽ ký tự độ (vòng nhỏ) ở phía trên phải của giá trị
    // Tính toạ độ tương đối: lấy vị trí con trỏ + width của chuỗi
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, 0, 22, &x1, &y1, &w, &h);
    // Vẽ chấm nhỏ bên trên góc phải của số
    _drawDegreeSymbol(x1 + w + 6, 18); // 6 px offset để cách số 1 chút

    // In chữ C (ký tự ASCII 'C') bên cạnh ký tự độ
    display.setCursor(x1 + w + 10, 18);
    display.setTextSize(2);
    display.print("C");

    // Dòng 2 (hoặc dưới): "Do am:" và giá trị %
    display.setTextSize(2);
    display.setCursor(0, 44);
    display.print("Do am: ");
    snprintf(buf, sizeof(buf), "%.0f%%", humidity_pct);
    display.print(buf);

    display.display();
}
