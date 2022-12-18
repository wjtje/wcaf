#pragma once

#include <Arduino.h>
#include <stdarg.h>
#include <wcaf/core/component.h>
#include <wcaf/core/log.h>

namespace wcaf {

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

  void print(const char *tag, int line, uint_farptr_t format_addr,
             size_t format_size, va_list args) {
    auto format_buff = (char *)malloc(format_size);
    memcpy_PF(format_buff, format_addr, format_size);

    this->buff_pos_ = 0;
    this->printf_to_buff_("[%s:%03u]: ", tag, line);
    this->vsnprintf_to_buff_(format_buff, args);
    this->buff_[this->buff_size_ - 1] = 0x00;

    delete format_buff;
    Serial.println(this->buff_);
  }

 protected:
  uint64_t baud_rate_{115200};
  size_t buff_size_{128};
  char *buff_;
  int buff_pos_ = 0;

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
