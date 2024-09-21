#pragma once
#include <Arduino.h>

#include "wcaf/core/component.h"
#include "wcaf/core/log.h"
#include "wcaf/helpers/vector.h"

namespace wcaf {
namespace application {

class Application : public Component {
 public:
  void setup();
  void loop();
  const char *get_tag() { return TAG; }
  static const char *TAG;

  void register_component(Component *component);
  void unregister_component(Component *component);

 protected:
  typedef Vector<Component *> t_Components;
  t_Components components_;
};

}  // namespace application
}  // namespace wcaf
