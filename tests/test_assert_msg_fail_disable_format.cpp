#define MEGATECH_ASSERTIONS_DISABLE_FORMAT (1)
#include <megatech/assertions.hpp>

int main() {
  MEGATECH_ASSERT_MSG(1 != 1, "test {}", "failed");
  return 0;
}
