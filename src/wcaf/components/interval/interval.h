#pragma once
#include <Arduino.h>

#include "wcaf/core/component.h"
#include "wcaf/core/log.h"

namespace wcaf {
namespace interval {

class Interval : public Component {
 public:
  void setup();
  void loop();
  const char *get_tag() { return TAG; }
  static const char *TAG;

  void set_interval(uint32_t interval) { this->interval_ = interval; }
  void set_argument(void *argument) { this->argument_ = argument; }
  void set_callback(void (*callback)(void *)) { this->callback_ = callback; }

 protected:
  uint32_t last_interval_{0};

  uint32_t interval_{1000};
  void *argument_{nullptr};
  void (*callback_)(void *);
};

}  // namespace interval
}  // namespace wcaf
