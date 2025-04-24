#pragma once
#include <Arduino.h>

#include "wcaf/components/interval/interval.h"
#include "wcaf/core/component.h"
#include "wcaf/core/log.h"
#include "wcaf/helpers/math.h"
#include "wcaf/interfaces/output.h"

namespace wcaf {
namespace led {

class Led : public Component {
 public:
  virtual void setup() override;
  virtual void loop() override;
  virtual const char *get_tag() override { return TAG; }
  static const char *TAG;

  void set_output(interface::Output *output) { this->output_ = output; }
  void set_duration(uint32_t duration) { this->duration_ = duration; }
  void set_interval(uint32_t interval) {
    if (this->interval_ == nullptr) this->interval_ = new interval::Interval();
    this->interval_->set_interval(interval);
  }

  void set_state(float state);
  float get_state() { return this->state_; }

  void turn_on();
  void turn_off();
  void toggle() {
    if (this->get_state() < 0.5f) {
      this->turn_on();
    } else {
      this->turn_off();
    }
  }

  void start_blinking() { this->blinking_ = true; }
  void stop_blinking() { this->blinking_ = false; }
  bool get_blinking() { return this->blinking_; }
  void toggle_blinking() { this->blinking_ = !this->blinking_; }

 protected:
  interface::Output *output_;
  interval::Interval *interval_{nullptr};

  bool blinking_{false};

  float prev_state_{0.0f};
  float target_state_{0.0f};
  float state_{0.0f};

  uint32_t start_millis_ = 0;
  bool state_changed_;
  int duration_ = 1000;
};

}  // namespace led
}  // namespace wcaf
