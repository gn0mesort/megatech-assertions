#include <thread>
#include <barrier>
#include <vector>

#include <megatech/assertions.hpp>

#include "config.hpp"

std::barrier ready{ CONFIG_TEST_MAX_THREADS };

void worker(const int i) {
  ready.arrive_and_wait();
  MEGATECH_ASSERT_MSG_PRINTF(1 != 1, "Thread %d", i);
}

int main() {
  auto threads = std::vector<std::jthread>{ };
  for (auto i = 0; i < CONFIG_TEST_MAX_THREADS; ++i)
  {
    threads.emplace_back(worker, i);
  }
  return 0;
}
