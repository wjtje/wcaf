#include "log.h"

namespace wcaf {

void wcaf_log(const char *tag, int line, uint_farptr_t format_addr,
              size_t format_size, ...) {
  auto *log = logger::global_logger;
  if (log == nullptr) return;

  va_list arg;
  va_start(arg, format_size);
  log->print(tag, line, format_addr, format_size, arg);
  va_end(arg);
}

}  // namespace wcaf
