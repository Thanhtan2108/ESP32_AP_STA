#include "ButtonModule.h"

// ====== CẤU HÌNH ======
#ifndef BUTTON_HOLD_TIME
#define BUTTON_HOLD_TIME 3000  // Thời gian giữ nút để trigger reset (3 giây)
#endif
#ifndef BUTTON_DEBOUNCE_TIME
#define BUTTON_DEBOUNCE_TIME 50  // Debounce 50ms
#endif
// ======================

static uint8_t g_button_pin = 0;
static unsigned long g_button_press_time = 0;
static bool g_button_pressed = false;
static bool g_button_triggered = false; // Để tránh trigger nhiều lần
static ButtonCallback g_reset_callback = nullptr;
static unsigned long g_last_interrupt_time = 0;

// ISR (Interrupt Service Routine) - gọi ngay khi nút nhấn
void IRAM_ATTR buttonISR() {
    unsigned long now = millis();
    
    // Debounce: bỏ qua nếu interrupt xảy ra quá gần nhau
    if (now - g_last_interrupt_time < BUTTON_DEBOUNCE_TIME) {
        return;
    }
    g_last_interrupt_time = now;
    
    // Nếu nút được nhấn (LOW)
    if (digitalRead(g_button_pin) == LOW) {
        if (!g_button_pressed) {
            g_button_pressed = true;
            g_button_press_time = now;
            g_button_triggered = false;
            Serial.println("[ButtonModule] [ISR] Button pressed!");
        }
    }
    // Nếu nút được thả (HIGH)
    else {
        if (g_button_pressed) {
            unsigned long pressDuration = now - g_button_press_time;
            Serial.print("[ButtonModule] [ISR] Button released after ");
            Serial.print(pressDuration);
            Serial.println(" ms");
            g_button_pressed = false;
        }
    }
}

void ButtonModule_init(uint8_t button_pin) {
    g_button_pin = button_pin;
    pinMode(g_button_pin, INPUT_PULLUP); // GPIO với PULLUP (nút nhấn = LOW)
    
    // Đính kèm interrupt (trigger on CHANGE để bắt cả nhấn và thả)
    attachInterrupt(digitalPinToInterrupt(g_button_pin), buttonISR, CHANGE);
    
    Serial.print("[ButtonModule] Initialized on GPIO ");
    Serial.print(g_button_pin);
    Serial.println(" with INTERRUPT mode");
}

void ButtonModule_loop() {
    // Kiểm tra nếu nút đang được giữ
    if (g_button_pressed && !g_button_triggered) {
        unsigned long pressDuration = millis() - g_button_press_time;
        
        // Nếu giữ đủ lâu
        if (pressDuration >= BUTTON_HOLD_TIME) {
            g_button_triggered = true;
            Serial.println("[ButtonModule] [LOOP] Button hold detected! Calling reset callback...");
            if (g_reset_callback) {
                g_reset_callback();
            }
        }
    }
}

void ButtonModule_setResetCallback(ButtonCallback callback) {
    g_reset_callback = callback;
    Serial.println("[ButtonModule] Reset callback registered");
}

bool ButtonModule_isPressed() {
    return g_button_pressed;
}

unsigned long ButtonModule_getPressedDuration() {
    if (!g_button_pressed) {
        return 0;
    }
    return millis() - g_button_press_time;
}
