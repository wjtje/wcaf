#include "interval.h"

namespace wcaf {
namespace interval {

const char *Interval::TAG = "Interval";

void Interval::setup() {}

void Interval::loop() {
  uint32_t now = millis();
  if (now - this->last_interval_ > this->interval_) {
    this->callback_(this->argument_);
    this->last_interval_ = now;
  }
}

}  // namespace interval
}  // namespace wcaf
