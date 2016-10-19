#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int nax;

void setter() {
  x.store(1, std::memory_order_relaxed);
  x.store(2, std::memory_order_seq_cst);
  x.store(3, std::memory_order_relaxed);
  y.store(1, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  while (y.load(std::memory_order_relaxed) < 1)
    std::this_thread::yield();
  // SC fence forces non-SC read to see last SC write or later.
  atomic_thread_fence(std::memory_order_seq_cst);
  nax = x.load(std::memory_order_relaxed);
  std::cout << "nax: " << nax << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
