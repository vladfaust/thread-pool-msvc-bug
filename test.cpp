#include <atomic>
#include <chrono>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./doctest.h"

#include "./thread_pool.cpp"

// TODO: Test the priority.
TEST_CASE("ThreadPool") {
  ThreadPool pool(std::thread::hardware_concurrency());

  std::atomic<int> sum = 0; // C++14§29.6.5¶4
  std::future<int> future;

  for (int i = 1; i <= 1024; i++) {
    future = pool.enqueue([&sum, i] {
      sum.fetch_add(i);
      return i;
    });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  CHECK(sum == 524800);
  CHECK(future.get() == 1024); // I need to convey the payload
}
