#include "sensor.h"

namespace wcaf {
namespace sensor {

const char *Sensor::TAG = "Sensor";

void Sensor::setup() {
  this->gpio_->set_mode(INPUT);

  if (this->interval_ == nullptr) this->interval_ = new interval::Interval();
  this->interval_->set_argument(this);
  this->interval_->set_callback([](void *parent) {
    auto sensor = (sensor::Sensor *)parent;
    auto value = sensor->gpio_->read_analog();
    WCAF_LOG("New value for %i: %i", sensor->gpio_->get_gpio(), value);
    if (sensor->on_value_.has_value())
      sensor->on_value_.value()(value
#ifdef ARDUINO_AVR_UNO
                                ,
                                sensor->argument_
#endif
      );
  });
}

void Sensor::loop() { this->interval_->loop(); }

}  // namespace sensor

}  // namespace wcaf
