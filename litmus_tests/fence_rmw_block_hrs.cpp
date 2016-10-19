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

void extender() {
  std::this_thread::sleep_for(std::chrono::milliseconds(4));
  msg.fetch_add(1, std::memory_order_relaxed);
}

void blocker() {
  std::this_thread::sleep_for(std::chrono::milliseconds(8));
  msg.store(3, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(12));
  while (msg.load(std::memory_order_acquire) < 3)
    std::this_thread::yield();
  // RMW in extender should extend the RS.
  std::cout << "x: " << x << std::endl;
}

int main(int argc, char **argv) {
  std::thread get(getter);
  std::thread ext(extender);
  std::thread blk(blocker);
  std::thread set(setter);
  get.join();
  ext.join();
  blk.join();
  set.join();
  return 0;
}
