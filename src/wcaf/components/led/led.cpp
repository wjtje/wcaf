#include "led.h"

namespace wcaf {
namespace led {

const char* Led::TAG = "Led";

void Led::setup() {
  this->gpio_->set_mode(OUTPUT);
  this->gpio_->write_digital(false);
}

void Led::loop() {
  if (!this->state_changed_) return;

  uint32_t now = millis();

  // Caculate the duration of the 'animation', this keeps the time it take to
  // speed up, and slow down always the same
  float duration =
      (float)this->duration_ * fabs(this->prev_state_ - this->target_state_);
  // Caclate the progress with a linear curve
  float progress = (float)(now - this->start_millis_) / duration;

  if (progress >= 1.0f || this->target_state_ == this->state_) {
    this->state_ = this->target_state_;
    this->state_changed_ = false;
  } else {
    this->state_ = lerp(progress, this->prev_state_, this->target_state_);
  }

  this->gpio_->write_analog(this->state_ * 255);
}

void Led::set_state(float state) {
  if (this->target_state_ == state) return;

  WCAF_LOG("%i, setting target state to: %0.1f", this->gpio_->get_gpio(),
           state);

  this->target_state_ = state;
  this->prev_state_ = this->state_;
  this->state_changed_ = true;
  this->start_millis_ = millis();
}

void Led::turn_on() { this->set_state(1.0f); }

void Led::turn_off() { this->set_state(0.0f); }

}  // namespace led
}  // namespace wcaf