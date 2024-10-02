#include <megatech/assertions.hpp>

int main() {
  MEGATECH_ASSERT_MSG_FORMAT(1 != 1, "test {}", "passed");
  return 0;
}
