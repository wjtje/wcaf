#pragma once
#if defined(ARDUINO_ESP32_DEV)

#include <esp32-hal-ledc.h>
#include <stdint.h>

#include "wcaf/core/log.h"
#include "wcaf/interfaces/output.h"

namespace wcaf {
namespace output {

class LedcOutput : public interface::Output {
 public:
  LedcOutput(uint8_t gpio, bool inverted = false, double freq = 1000)
      : interface::Output(inverted), gpio_{gpio} {
    ledcSetup(LedcOutput::channel_count_, freq, 12);
    ledcAttachPin(gpio, LedcOutput::channel_count_);
    this->channel_ = LedcOutput::channel_count_;
    LedcOutput::channel_count_++;
  }

  uint8_t get_pin() { return this->gpio_; }

 protected:
  static uint8_t channel_count_;
  static const char *TAG;

  uint8_t gpio_;
  uint8_t channel_;

  void write_state(float state) {
    uint32_t duty = (4095 / 255) * min((int)(state * 255), 255);
    ledcWrite(this->channel_, duty);
  }
};

}  // namespace output
}  // namespace wcaf

#endif
