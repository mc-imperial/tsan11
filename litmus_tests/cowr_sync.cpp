#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
std::atomic<int> msg(0);

void setter() {
  x.store(1, std::memory_order_relaxed);
  msg.store(1, std::memory_order_release);
  x.store(2, std::memory_order_relaxed);
}

void getter() {
  while (msg.load(std::memory_order_acquire) < 1)
    std::this_thread::yield();
  // Must not see the init or first store.
  int rx = x.load(std::memory_order_relaxed);
  std::cout << "rx: " << rx << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
