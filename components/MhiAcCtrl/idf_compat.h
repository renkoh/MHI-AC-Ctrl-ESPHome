#pragma once

/**
 * ESP-IDF Compatibility Layer for Arduino API
 * 
 * This header provides Arduino-style wrappers around ESP-IDF GPIO and timer functions
 * to minimize changes to the core MHI protocol driver code.
 */

#include "driver/gpio.h"
#include "esp_timer.h"
#include <cstring>
#include <cmath>

// Make isnan available without std:: prefix for Arduino compatibility
using std::isnan;

// ============================================================================
// GPIO Mode Constants (Arduino compatibility) - MUST be before functions
// ============================================================================

#ifndef INPUT
#define INPUT     0
#define OUTPUT    1
#define INPUT_PULLUP  2
#endif

// ============================================================================
// Arduino Type Aliases
// ============================================================================

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef boolean
typedef bool boolean;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

// ============================================================================
// GPIO Functions
// ============================================================================

/**
 * Set GPIO pin direction (INPUT or OUTPUT)
 * Maps to Arduino pinMode()
 */
inline void idf_pinMode(uint8_t pin, uint8_t mode) {
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = (1ULL << pin);
  
  if (mode == INPUT) {
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  } else {  // OUTPUT
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  }
  io_conf.intr_type = GPIO_INTR_DISABLE;
  
  gpio_config(&io_conf);
}

/**
 * Set GPIO pin level (0 or 1)
 * Maps to Arduino digitalWrite()
 */
inline void idf_digitalWrite(uint8_t pin, uint8_t level) {
  gpio_set_level(static_cast<gpio_num_t>(pin), level ? 1 : 0);
}

/**
 * Read GPIO pin level (0 or 1)
 * Maps to Arduino digitalRead()
 */
inline uint8_t idf_digitalRead(uint8_t pin) {
  return gpio_get_level(static_cast<gpio_num_t>(pin));
}

// ============================================================================
// Timing Functions
// ============================================================================

/**
 * Get milliseconds elapsed since startup
 * Maps to Arduino millis()
 * 
 * Returns: milliseconds as unsigned long
 */
inline unsigned long idf_millis(void) {
  return esp_timer_get_time() / 1000;
}

// ============================================================================
// Flash Memory (PROGMEM) Compatibility
// ============================================================================

/**
 * For ESP-IDF, PROGMEM data is stored in flash by default.
 * No special handling needed - const arrays go to .rodata section automatically.
 * This macro is provided for source compatibility.
 */
#define PROGMEM

/**
 * Read 16-bit value from flash (PROGMEM)
 * Maps to Arduino pgm_read_word()
 * 
 * For ESP-IDF, returns the value as-is since const pointers directly access flash
 * Accepts any pointer type (void*, byte*, etc.) for compatibility
 */
inline uint16_t pgm_read_word(const void* p) {
  return *reinterpret_cast<const uint16_t*>(p);
}

// ============================================================================
// Bit Manipulation (Arduino compatibility)
// ============================================================================

/**
 * Extract high byte from 16-bit value
 * Maps to Arduino highByte()
 */
inline uint8_t highByte(uint16_t value) {
  return (value >> 8) & 0xFF;
}

/**
 * Extract low byte from 16-bit value
 * Maps to Arduino lowByte()
 */
inline uint8_t lowByte(uint16_t value) {
  return value & 0xFF;
}
