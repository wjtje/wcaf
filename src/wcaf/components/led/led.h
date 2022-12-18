#pragma once
#include <Arduino.h>

#include "wcaf/core/component.h"
#include "wcaf/core/log.h"
#include "wcaf/helpers/gpio.h"
#include "wcaf/helpers/math.h"

namespace wcaf {
namespace led {

class Led : public Component {
 public:
  void setup();
  void loop();
  const char *get_tag() { return TAG; }
  static const char *TAG;

  void set_gpio(gpio::GPIO *gpio) { this->gpio_ = gpio; }
  void set_duration(uint32_t duration) { this->duration_ = duration; }

  void set_state(float state);
  void turn_on();
  void turn_off();

  float get_state() { return this->state_; }

 protected:
  gpio::GPIO *gpio_;

  float prev_state_{0.0f};
  float target_state_{0.0f};
  float state_{0.0f};

  uint32_t start_millis_ = 0;
  bool state_changed_;
  int duration_ = 1000;
};

}  // namespace led
}  // namespace wcaf
