#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> msg(0);
int x;

void setter() {
  x = 1;
  atomic_thread_fence(std::memory_order_release);
  msg.store(1, std::memory_order_relaxed);
}

void getter() {
  while (!msg.load(std::memory_order_acquire))
    std::this_thread::yield();
  // When 1 is read in msg, setter and getter must have synchronised, so the
  // non-atomic write to x must happend before the non-atomic read.
  std::cout << "x: " << x << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
