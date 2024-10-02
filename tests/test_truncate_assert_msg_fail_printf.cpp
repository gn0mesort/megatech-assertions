#include <megatech/assertions.hpp>

#include "config.hpp"

int main() {
  MEGATECH_ASSERT_MSG_PRINTF(1 != 1, CONFIG_TEST_TRUNCATION_STRING);
  return 0;
}
