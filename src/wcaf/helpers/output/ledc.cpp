#include "ledc.h"
#if defined(ARDUINO_ESP32_DEV)

namespace wcaf {
namespace output {

const char *LedcOutput::TAG = "LedcOutput";
uint8_t LedcOutput::channel_count_ = 0;

}  // namespace output
}  // namespace wcaf

#endif
