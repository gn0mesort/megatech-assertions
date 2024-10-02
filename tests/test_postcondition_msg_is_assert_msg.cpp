#include <megatech/assertions.hpp>

int main() {
  MEGATECH_POSTCONDITION_MSG(1 != 1, "test passed");
  return 0;
}
