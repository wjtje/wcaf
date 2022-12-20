#pragma once

#if defined(ARDUINO_ESP32_DEV)
#include <functional>
#endif

#include "wcaf/components/interval/interval.h"
#include "wcaf/core/component.h"
#include "wcaf/core/log.h"
#include "wcaf/helpers/gpio.h"
#include "wcaf/helpers/optional.h"

namespace wcaf {
namespace sensor {

class Sensor : public Component {
 public:
  void setup();
  void loop();
  const char *get_tag() { return TAG; }
  static const char *TAG;

  void set_gpio(gpio::GPIO *gpio) { this->gpio_ = gpio; };
  void set_interval(uint32_t interval) {
    if (this->interval_ == nullptr) this->interval_ = new interval::Interval();
    this->interval_->set_interval(interval);
  }

#ifdef ARDUINO_AVR_UNO
  void set_argument(void *argument) { this->argument_ = argument; }
  void on_value(void (*lambda)(int, void *)) { this->on_value_ = lambda; }
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  void on_value(std::function<void(int)> &&lambda) { this->on_value_ = lambda; }
#endif

 protected:
  gpio::GPIO *gpio_;
  interval::Interval *interval_{nullptr};

#ifdef ARDUINO_AVR_UNO
  void *argument_{nullptr};
  optional::Optional<void (*)(int, void *)> on_value_;
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  optional::Optional<std::function<void(int)> > on_value_;
#endif
};

}  // namespace sensor

}  // namespace wcaf