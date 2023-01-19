#include "sensor.h"

namespace wcaf {
namespace sensor {

const char *Sensor::TAG = "Sensor";

void Sensor::setup() {
  if (this->interval_ == nullptr) this->interval_ = new interval::Interval();
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  this->interval_->set_argument(this);
  this->interval_->set_callback([](void *parent) {
    auto sensor = (sensor::Sensor *)parent;
    auto value = sensor->input_->read();
    sensor->last_value_ = value;
    WCAF_LOG_INFO("New value for %i: %0.2f", sensor->input_->get_pin(), value);
    if (sensor->on_value_.has_value())
      sensor->on_value_.value()(value, sensor->argument_);
  });
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  this->interval_->set_callback([this]() {
    auto value = this->input_->read();
    this->last_value_ = value;
    WCAF_LOG_INFO("New value for %i: %0.2f", this->input_->get_pin(), value);
    if (this->on_value_.has_value()) this->on_value_.value()(value);
  });
#endif
}

void Sensor::loop() { this->interval_->loop(); }

}  // namespace sensor

}  // namespace wcaf
