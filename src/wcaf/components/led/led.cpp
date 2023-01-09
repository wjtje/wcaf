#include "led.h"

namespace wcaf {
namespace led {

const char *Led::TAG = "Led";

void Led::setup() {
  this->output_->write(false);
  if (this->interval_ == nullptr) this->interval_ = new interval::Interval();

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  this->interval_->set_argument(this);
  this->interval_->set_callback([](void *argument) {
    auto led = (led::Led *)argument;
    led->toggle();
  });
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  this->interval_->set_callback([this]() { this->toggle(); });
#endif
}

void Led::loop() {
  if (this->blinking_) this->interval_->loop();
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

  this->output_->write(this->state_);
}

void Led::set_state(float state) {
  if (this->target_state_ == state) return;

  WCAF_LOG_INFO("%i, setting target state to: %0.1f", this->output_->get_pin(),
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