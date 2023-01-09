#pragma once
#include <Arduino.h>

#if defined(ARDUINO_ESP32_DEV)
#include <functional>
#endif

#include "wcaf/core/component.h"
#include "wcaf/core/log.h"

namespace wcaf {
namespace interval {

class Interval : public Component {
 public:
  void setup();
  void loop();
  const char *get_tag() { return TAG; }
  static const char *TAG;

  void set_interval(uint32_t interval) { this->interval_ = interval; }
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  void set_argument(void *argument) { this->argument_ = argument; }
  void set_callback(void (*callback)(void *)) { this->callback_ = callback; }
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  void set_callback(std::function<void()> &&callback) {
    this->callback_ = callback;
  }
#endif

 protected:
  uint32_t last_interval_{0};

  uint32_t interval_{1000};
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  void *argument_{nullptr};
  void (*callback_)(void *);
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  std::function<void()> callback_;
#endif
};

}  // namespace interval
}  // namespace wcaf
