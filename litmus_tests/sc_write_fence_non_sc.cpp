#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int sx, sy;

void setter() {
  y.store(1, std::memory_order_relaxed);
  y.store(2, std::memory_order_relaxed);
  x.store(1, std::memory_order_relaxed);
  x.store(2, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  atomic_thread_fence(std::memory_order_seq_cst);
  sx = x.load(std::memory_order_relaxed);
  sy = y.load(std::memory_order_relaxed);
  // Relies on the sleep forcing SC order. No SC in setter causes SC fence to do
  // nothing.
  std::cout << "sx: " << sx << ", sy: " << sy << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
