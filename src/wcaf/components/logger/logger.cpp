#include "logger.h"

namespace wcaf {
namespace logger {

const char *Logger::TAG = "Logger";

void Logger::pre_setup() {
  global_logger = this;
  if (this->baud_rate_ != 0) Serial.begin(this->baud_rate_);
  this->buff_ = (char *)malloc(this->buff_size_);
}

void Logger::setup() {}

void Logger::loop() {}

Logger *global_logger = nullptr;

}  // namespace logger
}  // namespace wcaf