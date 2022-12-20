#pragma once
#include <Arduino.h>

#include "wcaf/interfaces/output.h"

namespace wcaf {
namespace output {

class GpioOutput : public interface::BinaryOutput {
 public:
  GpioOutput(uint8_t gpio, bool inverted = false)
      : interface::BinaryOutput(inverted), gpio_{gpio} {
    pinMode(gpio, OUTPUT);
  };

  uint8_t get_pin() { return this->gpio_; }

 protected:
  uint8_t gpio_;
  void write_state(bool state) { digitalWrite(this->gpio_, state); }
};

}  // namespace output
}  // namespace wcaf
