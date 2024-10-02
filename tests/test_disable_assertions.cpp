#define MEGATECH_ASSERTIONS_DISABLED (1)
#include <megatech/assertions.hpp>

int main() {
  MEGATECH_ASSERT(1 != 1);
  return 0;
}
