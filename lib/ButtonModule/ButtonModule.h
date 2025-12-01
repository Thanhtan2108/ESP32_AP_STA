#ifndef ButtonModule_h
#define ButtonModule_h

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

// Khởi tạo nút nhấn (GPIO pin) với interrupt
void ButtonModule_init(uint8_t button_pin);

// Xử lý logic nút nhấn trong main loop (kiểm tra timeout)
void ButtonModule_loop();

// Callback function khi nút được nhấn giữ đủ lâu
// Người dùng cần gán callback này để xử lý logic reset WiFi
typedef void (*ButtonCallback)();
void ButtonModule_setResetCallback(ButtonCallback callback);

// Lấy trạng thái nút nhấn (true = đang nhấn, false = không nhấn)
bool ButtonModule_isPressed();

// Lấy thời gian nhấn giữ hiện tại (milliseconds)
unsigned long ButtonModule_getPressedDuration();

#ifdef __cplusplus
}
#endif

#endif
