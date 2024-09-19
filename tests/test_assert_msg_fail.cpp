#include <megatech/assertions.hpp>

int main() {
  MEGATECH_ASSERT_MSG(1 != 1, "test passed");
  return 0;
}
