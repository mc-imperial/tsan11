#include <atomic>
#include <iostream>
#include <thread>

int nax = 0;
std::atomic<int> x(0);

void setter() {
  nax = 1;
  x.fetch_add(1, std::memory_order_release);
}

void extender() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  x.fetch_add(1, std::memory_order_relaxed);
}

void blocker() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  x.store(3, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(3));
  while (x.load(std::memory_order_acquire) < 3)
    std::this_thread::yield();
  // RMW in extender should extend the RS.
  int x = ::x.load(std::memory_order_acquire);
  std::cout << "x: " << x << " nax: " << nax << std::endl;
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
