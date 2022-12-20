#pragma once
#include <Arduino.h>

#if defined(ARDUINO_ESP32_DEV)
#include <functional>
#endif

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

#ifdef ARDUINO_AVR_UNO
  void set_argument(void *argument) { this->argument_ = argument; }
  void on_press(void (*lambda)(void *)) { this->on_press_ = lambda; }
  void on_release(void (*lambda)(void *)) { this->on_release_ = lambda; }
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  void on_press(std::function<void()> &&lambda) { this->on_press_ = lambda; }
  void on_release(std::function<void()> &&lambda) {
    this->on_release_ = lambda;
  }
#endif

 protected:
  // Value for the component
  gpio::GPIO *gpio_;

  // Values for debouncing
  bool last_button_state_;
  uint32_t last_button_time_;
  uint32_t debouce_time_ = 50;

  // The current status of the button
  bool status_;

#ifdef ARDUINO_AVR_UNO
  void *argument_{nullptr};
  optional::Optional<void (*)(void *)> on_press_;
  optional::Optional<void (*)(void *)> on_release_;
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  optional::Optional<std::function<void()> > on_press_;
  optional::Optional<std::function<void()> > on_release_;
#endif
};

}  // namespace button
}  // namespace wcaf
