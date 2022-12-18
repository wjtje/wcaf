#pragma once

namespace wcaf {

class Component {
 public:
  virtual void setup();
  virtual void loop();
  virtual const char* get_tag();
};

}  // namespace wcaf
