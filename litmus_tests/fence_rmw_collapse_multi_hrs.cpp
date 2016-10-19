#include <atomic>
#include <iostream>
#include <thread>

int x, y;
std::atomic<int> msg(0);

void setter() {
  x = 1;
  atomic_thread_fence(std::memory_order_release);
  msg.store(1, std::memory_order_relaxed);  // 1st
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
  msg.store(3, std::memory_order_relaxed);  // 3rd
}

void extender() {
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  y = 1;
  atomic_thread_fence(std::memory_order_release);
  msg.fetch_add(1, std::memory_order_relaxed);  // 2nd
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(12));
  // May synchronise depending on the adversarial strat, if strat is to read old
  // stores, then most likely will sync.
  while (msg.load(std::memory_order_acquire) < 3)
    std::this_thread::yield();
  std::cout << "x: " << x << ", y: " << y << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread ext(extender);
  std::thread get(getter);
  set.join();
  ext.join();
  get.join();
  return 0;
}
