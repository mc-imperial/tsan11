#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
int rx;

void setter() {
  x.store(1, std::memory_order_relaxed);
  x.store(2, std::memory_order_seq_cst);
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
  x.store(3, std::memory_order_seq_cst);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  atomic_thread_fence(std::memory_order_seq_cst);
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
  rx = x.load(std::memory_order_relaxed);
  // Fence before second SC store means we can see first store.
  std::cout << "rx: " << rx << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
