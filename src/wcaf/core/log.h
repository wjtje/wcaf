#pragma once

#include <wcaf/components/logger/logger.h>

namespace wcaf {
#ifdef WCAF_NO_LOG
#define WCAF_LOG(level, format, ...) \
  do {                               \
  } while (0)
#else
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
void wcaf_log(uint8_t level, const char *tag, int line, const char *format,
              ...);

#define WCAF_LOG(level, format, ...)                    \
  (__extension__({                                      \
    static const char __c[] PROGMEM = (format);         \
    wcaf_log(level, TAG, __LINE__, __c, ##__VA_ARGS__); \
  }));
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ESP32_DEV)
void wcaf_log(uint8_t level, const char *tag, int line, const char *format,
              ...);

#define WCAF_LOG(level, format, ...) \
  wcaf_log(level, TAG, __LINE__, format, ##__VA_ARGS__);
#endif
#endif

#define WCAF_LOG_DEFAULT(format, ...) WCAF_LOG(0, format, ##__VA_ARGS__);
#define WCAF_LOG_ERROR(format, ...) WCAF_LOG(1, format, ##__VA_ARGS__);
#define WCAF_LOG_WARNING(format, ...) WCAF_LOG(2, format, ##__VA_ARGS__);
#define WCAF_LOG_INFO(format, ...) WCAF_LOG(3, format, ##__VA_ARGS__);

}  // namespace wcaf
