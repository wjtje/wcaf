#pragma once
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_ARCH_ESP8266)

#include <Arduino.h>

#include "wcaf/interfaces/output.h"

namespace wcaf {
namespace output {

class PwmOutput : public interface::Output {
 public:
  PwmOutput(uint8_t gpio, bool inverted = false)
      : interface::Output(inverted), gpio_{gpio} {
    pinMode(gpio, OUTPUT);
  }

  uint8_t get_pin() { return this->gpio_; }

 protected:
  uint8_t gpio_;

  void write_state(float state) { analogWrite(this->gpio_, state * 255); }
};

}  // namespace output
}  // namespace wcaf

#endif
