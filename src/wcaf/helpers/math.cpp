#include "math.h"

namespace wcaf {

float lerp(float completion, float start, float end) {
  return start + (end - start) * completion;
}

}  // namespace wcaf
