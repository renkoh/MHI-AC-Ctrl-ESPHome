// mhi_hal.h
// Thin hardware abstraction layer so the MHI-AC-Ctrl core compiles on both
// the Arduino framework (ESP8266, ESP32, ESP32-C3/S2/S3) and ESP-IDF
// (ESP32-C6 and other chips without Arduino support in ESPHome).
#pragma once

#include <stdint.h>

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include "driver/gpio.h"
#include "esp_timer.h"
#endif

static inline void mhi_pin_mode_input(int pin) {
#if defined(ARDUINO)
  pinMode(pin, INPUT);
#else
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = 1ULL << pin;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&io_conf);
#endif
}

static inline void mhi_pin_mode_output(int pin) {
#if defined(ARDUINO)
  pinMode(pin, OUTPUT);
#else
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = 1ULL << pin;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&io_conf);
#endif
}

static inline int mhi_digital_read(int pin) {
#if defined(ARDUINO)
  return digitalRead(pin);
#else
  return gpio_get_level(static_cast<gpio_num_t>(pin));
#endif
}

static inline void mhi_digital_write(int pin, int level) {
#if defined(ARDUINO)
  digitalWrite(pin, level);
#else
  gpio_set_level(static_cast<gpio_num_t>(pin), level);
#endif
}

// returns milliseconds since boot, wraps around every ~49.7 days (same as Arduino millis())
static inline uint32_t mhi_millis() {
#if defined(ARDUINO)
  return millis();
#else
  return static_cast<uint32_t>(esp_timer_get_time() / 1000);
#endif
}
