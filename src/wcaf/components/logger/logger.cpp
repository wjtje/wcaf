#include "logger.h"

namespace wcaf {
namespace logger {

const char *Logger::TAG = "Logger";

void Logger::pre_setup() {
  global_logger = this;
  if (this->baud_rate_ != 0) Serial.begin(this->baud_rate_);
}

void Logger::setup() {}

void Logger::loop() {}

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
void Logger::add_message_handler(void *argument,
                                 void (*lambda)(void *, const char *,
                                                const char *)) {
  message_handler_ handler = {.argument = argument, .lambda = lambda};
  this->message_handlers_.push_back(handler);
}

void Logger::print(uint8_t level, const char *tag, int line, const char *format,
                   va_list args) {
  if (level > 3) level = 3;
  this->buff_pos_ = 0;
  this->printf_to_buff_("%s%s (%lu)[%s:%03u]: ", LOG_LEVEL_COLORS[level],
                        LOG_LEVEL_LETTERS[level], millis(), tag, line);

  this->buff_pos_ += vsnprintf_P(
      this->buff_ + this->buff_pos_,
      sizeof(this->buff_) - this->buff_pos_ - sizeof(WCAF_LOG_RESET_COLOR) - 1,
      format, args);

  this->printf_to_buff_(WCAF_LOG_RESET_COLOR);
  this->buff_[sizeof(buff_) - 1] = 0x00;

  if (this->baud_rate_ != 0) {
    Serial.println(this->buff_);
    Serial.flush();
  }

  for (auto &handler : this->message_handlers_) {
    handler.lambda(handler.argument, tag, this->buff_);
  }
}
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
void Logger::add_message_handler(
    std::function<void(const char *tag, const char *message)> &&lambda) {
  this->message_handlers_.push_back(lambda);
}

void Logger::print(uint8_t level, const char *tag, int line, const char *format,
                   va_list args) {
  if (level > 3) level = 3;
  this->buff_pos_ = 0;
  this->printf_to_buff_("%s[%s][%s:%03u]: ", LOG_LEVEL_COLORS[level],
                        LOG_LEVEL_LETTERS[level], tag, line);
  this->vsnprintf_to_buff_(format, args);
  int remaining = this->buff_size_ - buff_pos_;
  int pos = (remaining > 7) ? this->buff_pos_ : this->buff_size_ - 8;
  memcpy(this->buff_ + pos, "\033[0m\n\0", 7);

  if (this->baud_rate_ != 0) {
    Serial.write(this->buff_);
    Serial.flush();
  }

  for (auto handler : this->message_handlers_) {
    handler(tag, this->buff_);
  }
}
#endif

void Logger::vsnprintf_to_buff_(const char *format, va_list args) {
  const int remaining = sizeof(buff_) - this->buff_pos_;
  if (remaining < 1) return;

  int used = vsnprintf(this->buff_ + this->buff_pos_, remaining, format, args);
  if (used < 0) return;
  if (used >= remaining) used = remaining;
  this->buff_pos_ += used;
}

Logger *global_logger = nullptr;

}  // namespace logger
}  // namespace wcaf