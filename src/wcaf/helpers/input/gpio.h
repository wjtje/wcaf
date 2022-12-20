#pragma once
#include <Arduino.h>

#include "wcaf/interfaces/input.h"

namespace wcaf {
namespace input {

class GpioInput : public interface::BinaryInput {
 public:
  GpioInput(uint8_t gpio, bool inverted = false, uint8_t mode = INPUT)
      : interface::BinaryInput(inverted), gpio_{gpio} {
    pinMode(gpio, mode);
  };

  uint8_t get_pin() { return this->gpio_; }

 protected:
  uint8_t gpio_;
  bool read_state() { return digitalRead(this->gpio_); }
};

}  // namespace input
}  // namespace wcaf
