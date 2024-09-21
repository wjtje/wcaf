#pragma once

namespace wcaf {

class Component {
 public:
  virtual void setup() = 0;
  virtual void loop() = 0;
  virtual const char* get_tag() = 0;
};

}  // namespace wcaf
