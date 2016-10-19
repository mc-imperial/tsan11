#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int nax[10];
std::atomic<int> x(0);

void setter(size_t idx) {
  for (size_t i = 0; i < 10; ++i) {
    ++nax[idx];
    x.fetch_add(1, std::memory_order_release);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  if (idx != 5)
    return;
  while (x.load(std::memory_order_relaxed) < 100)
    std::this_thread::yield();
  x.store(101, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  while (x.load(std::memory_order_acquire) < 101)
    std::this_thread::yield();
  std::cout << "x: " << x.load(std::memory_order_relaxed) << std::endl;
  for (size_t i = 0; i < 10; ++i)
    std::cout << "nax[" << i << "]: " << nax[i] << std::endl;
}

int main(int argc, char **argv) {
  for (size_t idx = 0; idx < 10; ++idx)
    nax[idx] = 0;
  std::vector<std::thread> setters;
  for (size_t idx = 0; idx < 10; ++idx)
    setters.push_back(std::thread(setter, idx));
  std::thread get(getter);
  for (size_t idx = 0; idx < 10; ++idx)
    setters[idx].join();
  get.join();
  return 0;
}
