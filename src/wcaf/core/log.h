#pragma once

#include <wcaf/components/logger/logger.h>

namespace wcaf {

void wcaf_log(const char *tag, int line, uint_farptr_t format_addr,
              size_t format_size, ...);

#define WCAF_LOG(format, ...)                                      \
  (__extension__({                                                 \
    static const char __c[] PROGMEM = (format);                    \
    wcaf_log(TAG, __LINE__, pgm_get_far_address(__c), sizeof(__c), \
             ##__VA_ARGS__);                                       \
  }));
}  // namespace wcaf
