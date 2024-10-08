#pragma once

#include <Arduino.h>
#include <stdarg.h>

#include "wcaf/core/component.h"
#include "wcaf/core/log.h"
#include "wcaf/helpers/vector.h"

#define WCAF_LOG_COLOR_RED "31"     // ERROR
#define WCAF_LOG_COLOR_GREEN "32"   // INFO
#define WCAF_LOG_COLOR_YELLOW "33"  // WARNING

#define WCAF_LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define WCAF_LOG_RESET_COLOR "\033[0m"

namespace wcaf {

static const char *const LOG_LEVEL_COLORS[] = {
    "",                                     // NONE
    WCAF_LOG_COLOR(WCAF_LOG_COLOR_RED),     // ERROR
    WCAF_LOG_COLOR(WCAF_LOG_COLOR_YELLOW),  // WARNING
    WCAF_LOG_COLOR(WCAF_LOG_COLOR_GREEN),   // INFO
};
static const char *const LOG_LEVEL_LETTERS[] = {
    " ",  // NONE
    "E",  // ERROR
    "W",  // WARNING
    "I",  // INFO
};

void wcaf_log(const char *tag, int line, const char *format, ...);

namespace logger {

class Logger : public wcaf::Component {
 public:
  void pre_setup();
  void setup();
  void loop();
  const char *get_tag() { return TAG; }
  static const char *TAG;

  void set_baud_rate(uint64_t baud_rate) { this->baud_rate_ = baud_rate; }
  void set_buffer_size(size_t size) { this->buff_size_ = size; }

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  void add_message_handler(void *argument,
                           void (*lambda)(void *, const char *, const char *)) {
    message_handler_ handler = {.argument = argument, .lambda = lambda};
    this->message_handlers_.push_back(handler);
  }

  void print(uint8_t level, const char *tag, int line,
             uint_farptr_t format_addr, size_t format_size, va_list args) {
    auto format_buff = (char *)malloc(format_size);
    memcpy_PF(format_buff, format_addr, format_size);

    if (level > 3) level = 3;
    this->buff_pos_ = 0;
    this->printf_to_buff_("%s%s (%lu)[%s:%03u]: ", LOG_LEVEL_COLORS[level],
                          LOG_LEVEL_LETTERS[level], millis(), tag, line);
    this->vsnprintf_to_buff_(format_buff, args);
    this->printf_to_buff_(WCAF_LOG_RESET_COLOR);
    this->buff_[this->buff_size_ - 1] = 0x00;

    delete format_buff;
    if (this->baud_rate_ != 0) {
      Serial.println(this->buff_);
      Serial.flush();
    }

    for (auto &handler : this->message_handlers_) {
      handler.lambda(handler.argument, tag, this->buff_);
    }
  }
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  void add_message_handler(
      std::function<void(const char *tag, const char *message)> &&lambda) {
    this->message_handlers_.push_back(lambda);
  }

  void print(uint8_t level, const char *tag, int line, const char *format,
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

 protected:
  uint64_t baud_rate_{115200};
  size_t buff_size_{128};
  char *buff_;
  int buff_pos_ = 0;

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  struct message_handler_ {
    void *argument;
    void (*lambda)(void *, const char *, const char *);
  };

  Vector<message_handler_> message_handlers_;
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  Vector<std::function<void(const char *tag, const char *message)> >
      message_handlers_;
#endif

  inline void vsnprintf_to_buff_(const char *format, va_list args) {
    int remaining = this->buff_size_ - buff_pos_;
    if (remaining < 1) return;

    int used =
        vsnprintf(this->buff_ + this->buff_pos_, remaining, format, args);
    if (used < 0) return;
    if (used >= remaining) used = remaining;
    this->buff_pos_ += used;
  }

  inline void printf_to_buff_(const char *format, ...) {
    va_list args;
    va_start(args, format);
    this->vsnprintf_to_buff_(format, args);
    va_end(args);
  }
};

extern Logger *global_logger;

}  // namespace logger
}  // namespace wcaf
