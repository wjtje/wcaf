#pragma once
#include <stdint.h>

namespace wcaf {
namespace interface {

class Output {
 public:
  Output(bool inverted) : inverted_{inverted} {}

  void write(bool state) {
    this->write_state((state ^ this->inverted_) ? 1.0f : 0.0f);
  };
  void write(float state) {
    if (this->inverted_)
      this->write_state(1.0f - state);
    else
      this->write_state(state);
  };

  virtual uint8_t get_pin();

 protected:
  bool inverted_;

  virtual void write_state(float state);
};

class BinaryOutput : public Output {
 protected:
  BinaryOutput(bool inverted) : Output(inverted) {}

  void write_state(float state) {
    this->write_state((state > 0.5) ? true : false);
  }
  virtual void write_state(bool state);
};

}  // namespace interface
}  // namespace wcaf
