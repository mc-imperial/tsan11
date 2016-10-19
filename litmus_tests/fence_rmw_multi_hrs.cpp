#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> msg(0);
int x, y;

void setter1() {
  x = 1;
  atomic_thread_fence(std::memory_order_release);
  msg.fetch_add(1, std::memory_order_relaxed);
}

void setter2() {
  y = 1;
  atomic_thread_fence(std::memory_order_release);
  msg.fetch_add(1, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  while (msg.load(std::memory_order_acquire) != 2)
    std::this_thread::yield();
  // When 2 is read in msg, setter and getter must have synchronised, so the
  // non-atomic write to x must happend before the non-atomic read.
  std::cout << "x: " << x << ", y: " << y << std::endl;
}

int main(int argc, char **argv) {
  std::thread get(getter);
  std::thread set1(setter1);
  std::thread set2(setter2);
  get.join();
  set1.join();
  set2.join();
  return 0;
}
