#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int g1x1, g1x2, g2x;

void setter() {
  x.store(1, std::memory_order_relaxed);
  x.store(2, std::memory_order_relaxed);
  x.store(3, std::memory_order_relaxed);
}

void getter1() {
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  g1x1 = x.load(std::memory_order_relaxed);
  atomic_thread_fence(std::memory_order_release);
  g1x2 = x.load(std::memory_order_relaxed);
  y.store(1, std::memory_order_relaxed);
}

void getter2() {
  while(!y.load(std::memory_order_acquire))
    std::this_thread::yield();
  // Fence should restrict to first read, but not second.
  g2x = x.load(std::memory_order_relaxed);
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get1(getter1);
  std::thread get2(getter2);
  set.join();
  get1.join();
  get2.join();
  std::cout << "g1x1: " << g1x1 << ", g1x2: " << g1x2 << ", g2x: " << g2x
            << std::endl;
  return 0;
}
