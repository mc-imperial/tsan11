#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> msg1(0);
std::atomic<int> msg2(0);
int x, y;

void setter1() {
  x = 1;
  atomic_thread_fence(std::memory_order_release);
  msg1.store(1, std::memory_order_relaxed);
}

void setter2() {
  y = 1;
  atomic_thread_fence(std::memory_order_release);
  msg2.store(1, std::memory_order_relaxed);
}

void getter() {
  while (!msg1.load(std::memory_order_relaxed))
    std::this_thread::yield();
  while (!msg2.load(std::memory_order_relaxed))
    std::this_thread::yield();
  atomic_thread_fence(std::memory_order_acquire);
  // When 1 is read in msg1 and msg2, the acquire fence will sync with both
  // setter1 and setter2.
  std::cout << "x: " << x << ", y: " << y << std::endl;
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  std::thread get(getter);
  set1.join();
  set2.join();
  get.join();
  return 0;
}
