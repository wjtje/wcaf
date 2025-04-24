#include "log.h"

namespace wcaf {
#if !defined(WCAF_NO_LOG)
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
void wcaf_log(uint8_t level, const char *tag, int line, const char *format,
              ...) {
  auto *log = logger::global_logger;
  if (log == nullptr) return;

  va_list args;
  va_start(args, format);
  log->print(level, tag, line, format, args);
  va_end(args);
}
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
void wcaf_log(uint8_t level, const char *tag, int line, const char *format,
              ...) {
  auto *log = logger::global_logger;
  if (log == nullptr) return;

  va_list args;
  va_start(args, format);
  log->print(level, tag, line, format, args);
  va_end(args);
}
#endif
#endif

}  // namespace wcaf
