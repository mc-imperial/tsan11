#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int s2_y, g_y;

void setter1() {
  y.store(1, std::memory_order_relaxed);
  y.store(2, std::memory_order_relaxed);
}

void setter2() {
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  s2_y = y.load(std::memory_order_relaxed);
  x.store(1, std::memory_order_relaxed);
  atomic_thread_fence(std::memory_order_seq_cst);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
  while (x.load(std::memory_order_seq_cst) < 1)
    std::this_thread::yield();
  // Release acquire on x would force getter to see same or later store to y
  // than setter 2, but not with SC fence.
  g_y = y.load(std::memory_order_relaxed);
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  std::thread get(getter);
  set1.join();
  set2.join();
  get.join();
  // Print must go here or s2_y will race.
  std::cout << "s2_y: " << s2_y << ", g_y: " << g_y << std::endl;
  return 0;
}
