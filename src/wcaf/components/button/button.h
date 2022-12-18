#pragma once
#include <Arduino.h>

#include "wcaf/core/component.h"
#include "wcaf/core/log.h"
#include "wcaf/helpers/gpio.h"
#include "wcaf/helpers/optional.h"

namespace wcaf {
namespace button {

class Button : public Component {
 public:
  void setup();
  void loop();
  const char *get_tag() { return TAG; }
  static const char *TAG;

  void set_gpio(gpio::GPIO *gpio) { this->gpio_ = gpio; };
  void set_debounce(uint32_t debouce) { this->debouce_time_ = debouce; }

  void set_argument(void *argument) { this->argument_ = argument; }
  void set_on_press(void (*lambda)(void *)) { this->on_press_ = lambda; }
  void set_on_release(void (*lambda)(void *)) { this->on_release_ = lambda; }

 protected:
  // Value for the component
  gpio::GPIO *gpio_;

  // Values for debouncing
  bool last_button_state_;
  uint32_t last_button_time_;
  uint32_t debouce_time_ = 50;

  // The current status of the button
  bool status_;

  void *argument_{nullptr};
  optional::Optional<void (*)(void *)> on_press_;
  optional::Optional<void (*)(void *)> on_release_;
};

}  // namespace button
}  // namespace wcaf