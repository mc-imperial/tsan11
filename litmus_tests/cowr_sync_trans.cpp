#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
std::atomic<int> msg1(0);
std::atomic<int> msg2(0);

void setter() {
  x.store(1, std::memory_order_relaxed);
  msg1.store(1, std::memory_order_release);
  x.store(2, std::memory_order_relaxed);
}

void getter1() {
  while (msg1.load(std::memory_order_acquire) < 1)
    std::this_thread::yield();
  msg2.store(1, std::memory_order_release);
}

void getter2() {
  while (msg2.load(std::memory_order_acquire) < 1)
    std::this_thread::yield();
  // Must not see the init or first store.
  int rx = x.load(std::memory_order_relaxed);
  std::cout << "rx: " << rx << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get1(getter1);
  std::thread get2(getter2);
  set.join();
  get1.join();
  get2.join();
  return 0;
}
