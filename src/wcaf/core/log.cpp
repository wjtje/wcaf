#include "log.h"

namespace wcaf {

#if defined(ARDUINO_AVR_UNO)
void wcaf_log(const char *tag, int line, uint_farptr_t format_addr,
              size_t format_size, ...) {
  auto *log = logger::global_logger;
  if (log == nullptr) return;

  va_list args;
  va_start(args, format_size);
  log->print(tag, line, format_addr, format_size, args);
  va_end(args);
}
#elif defined(ARDUINO_ARCH_ESP8266)
void wcaf_log(const char *tag, int line, const char *format, ...) {
  auto *log = logger::global_logger;
  if (log == nullptr) return;

  va_list args;
  va_start(args, format);
  log->print(tag, line, format, args);
  va_end(args);
}
#endif

}  // namespace wcaf
