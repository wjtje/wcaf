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
  void write_analog(float state) {
    if (this->inverted_) state = 1.0f - state;
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_ARCH_ESP8266)
    analogWrite(this->gpio_, state * 255);
#else defined(ARDUINO_ESP32_DEV)
    Serial.println("Well, this doesn't work yet");
#endif
  }

  bool read_digital() { return digitalRead(this->gpio_) ^ this->inverted_; }
  float read_analog() {
#if defined(ARDUINO_ESP32_DEV)
    analogReadResolution(10);
#endif
    float state = analogRead(this->gpio_) / 1023.0f;
    if (this->inverted_) return 1.0f - state;
    return state;
  }

  uint8_t get_gpio() { return this->gpio_; }

 protected:
  uint8_t gpio_;
  bool inverted_;
};

}  // namespace gpio
}  // namespace wcaf