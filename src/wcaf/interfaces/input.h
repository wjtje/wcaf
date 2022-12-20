#pragma once

namespace wcaf {
namespace interface {

class Input {
 public:
  Input(bool inverted) : inverted_{inverted} {}

  virtual float read();
  virtual bool read_digital();

  virtual uint8_t get_pin();

 protected:
  bool inverted_;
};

class BinaryInput : public Input {
 public:
  BinaryInput(bool inverted) : Input(inverted) {}

  float read() { return (this->read_state() ^ this->inverted_) ? 1.0f : 0.0f; }
  bool read_digital() { return this->read_state() ^ this->inverted_; }

 protected:
  virtual bool read_state();
};

class FloatInput : public Input {
 public:
  FloatInput(bool inverted) : Input(inverted) {}

  float read() {
    if (this->inverted_) return 1.0f - this->read_state();
    return this->read_state();
  }
  bool read_digital() {
    return ((this->read_state()) ? true : false) ^ this->inverted_;
  }

 protected:
  virtual float read_state();
};

}  // namespace interface
}  // namespace wcaf
