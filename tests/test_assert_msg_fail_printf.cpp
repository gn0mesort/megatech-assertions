#include <megatech/assertions.hpp>

int main() {
  MEGATECH_ASSERT_MSG_PRINTF(1 != 1, "test %s", "passed");
  return 0;
}
