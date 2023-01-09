#include "button.h"

namespace wcaf {
namespace button {

const char* Button::TAG = "Button";

void Button::setup() {}

void Button::loop() {
  bool status = this->input_->read_digital();

  if (this->last_button_state_ != status) {
    this->last_button_time_ = millis();
    this->last_button_state_ = status;
  }

  if ((millis() - this->last_button_time_) > this->debouce_time_) {
    if (this->status_ != status) {
      this->status_ = status;

      if (this->status_ == 0) {
        WCAF_LOG_INFO("%i was released", this->input_->get_pin());
        // The argument value is only needed for the AVR_UNO version
        if (this->on_release_.has_value())
          this->on_release_.value()(
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
              argument_
#endif
          );
      } else {
        WCAF_LOG_INFO("%i was pressed", this->input_->get_pin());
        if (this->on_press_.has_value())
          this->on_press_.value()(
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
              argument_
#endif
          );
        ;
      }
    }
  }
}

}  // namespace button
}  // namespace wcaf
