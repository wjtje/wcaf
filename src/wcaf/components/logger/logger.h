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
  virtual void setup() override;
  virtual void loop() override;
  virtual const char *get_tag() override { return TAG; }
  static const char *TAG;

  void set_baud_rate(uint64_t baud_rate) { this->baud_rate_ = baud_rate; }

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  void add_message_handler(void *argument,
                           void (*lambda)(void *, const char *, const char *));
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
  void add_message_handler(
      std::function<void(const char *tag, const char *message)> &&lambda);
#endif

  void print(uint8_t level, const char *tag, int line, const char *format,
             va_list args);

 protected:
  uint64_t baud_rate_{115200};
  char buff_[128];
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

  void printf_to_buff_(const char *format, ...) {
    va_list args;
    va_start(args, format);
    this->vsnprintf_to_buff_(format, args);
    va_end(args);
  }
  void vsnprintf_to_buff_(const char *format, va_list args);
};

extern Logger *global_logger;

}  // namespace logger
}  // namespace wcaf
