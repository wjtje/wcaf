#pragma once
#include <Arduino.h>

namespace wcaf {
namespace gpio {

class GPIO {
 public:
  GPIO(uint8_t gpio) : gpio_(gpio), inverted_(false) {}
  GPIO(uint8_t gpio, bool inverted) : gpio_(gpio), inverted_(inverted) {}

  void set_mode(uint8_t mode) { pinMode(this->gpio_, mode); }

  void write_digital(bool state) {
    digitalWrite(this->gpio_, state ^ this->inverted_);
  }
  void write_analog(int state) {
    if (this->inverted_) state = 255 - state;
    analogWrite(this->gpio_, state);
  }

  bool read_digital() { return digitalRead(this->gpio_) ^ this->inverted_; }
  int read_analog() {
    if (this->inverted_) return 255 - analogRead(this->gpio_);
    return analogRead(this->gpio_);
  }

  uint8_t get_gpio() { return this->gpio_; }

 protected:
  uint8_t gpio_;
  bool inverted_;
};

}  // namespace gpio
}  // namespace wcaf