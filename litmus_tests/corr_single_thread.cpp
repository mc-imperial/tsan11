#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);

void setter() {
  x.store(1, std::memory_order_relaxed);
  x.store(2, std::memory_order_relaxed);
  x.store(3, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  // Following reads must observe mo.
  int rx1 = x.load(std::memory_order_relaxed);
  int rx2 = x.load(std::memory_order_relaxed);
  std::cout << "rx1: " << rx1 << ", rx2: " << rx2 << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
