#pragma once

namespace wcaf {
namespace optional {

template <typename T>
class Optional {
 public:
  Optional(){};
  Optional(T const &value) : has_value_(true), value_(value){};
  bool has_value() { return has_value_; }

  T *operator->() { return &value_; }
  T &value() { return value_; }

 protected:
  bool has_value_{false};
  T value_;
};

};  // namespace optional
}  // namespace wcaf
