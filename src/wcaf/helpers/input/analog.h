#pragma once
#include <Arduino.h>

#include "wcaf/interfaces/input.h"

namespace wcaf {
namespace input {

class AnalogInput : public interface::FloatInput {
 public:
  AnalogInput(uint8_t gpio, bool inverted = false, uint8_t mode = INPUT)
      : interface::FloatInput(inverted), gpio_{gpio} {
#if defined(ARDUINO_ESP32_DEV)
    analogReadResolution(10);
#endif
    pinMode(gpio, mode);
  };

  uint8_t get_pin() { return this->gpio_; }

 protected:
  uint8_t gpio_;
  float read_state() { return (float)analogRead(this->gpio_) / 1023.0f; }
};

}  // namespace input
}  // namespace wcaf
