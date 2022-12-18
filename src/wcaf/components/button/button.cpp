#include "button.h"

namespace wcaf {
namespace button {

const char* Button::TAG = "Button";

void Button::setup() { this->gpio_->set_mode(INPUT_PULLUP); }

void Button::loop() {
  bool status = this->gpio_->read_digital();

  if (this->last_button_state_ != status) {
    this->last_button_time_ = millis();
    this->last_button_state_ = status;
  }

  if ((millis() - this->last_button_time_) > this->debouce_time_) {
    if (this->status_ != status) {
      this->status_ = status;

      if (this->status_ == 0) {
        WCAF_LOG("%i was released", this->gpio_->get_gpio());
        if (this->on_release_.has_value()) this->on_release_.value()(argument_);
      } else {
        WCAF_LOG("%i was pressed", this->gpio_->get_gpio());
        if (this->on_press_.has_value()) this->on_press_.value()(argument_);
      }
    }
  }
}

}  // namespace button
}  // namespace wcaf
