#include "application.h"

namespace wcaf {
namespace application {

const char *Application::TAG = "Application";

void Application::setup() {
  WCAF_LOG("Setting up %i component(s)", this->components_.size());

  for (Component *component : this->components_) {
    WCAF_LOG("Setting up %s", component->get_tag());
    component->setup();
  }
}

void Application::loop() {
  for (Component *component : this->components_) {
    uint32_t start_component = millis();
    component->loop();

    if (millis() - start_component > 50) {
      WCAF_LOG("%s took a long time in the loop, > 50ms", component->get_tag());
    }
  }
}

void Application::register_component(Component *component) {
  this->components_.push_back(component);
}

}  // namespace application
}  // namespace wcaf
