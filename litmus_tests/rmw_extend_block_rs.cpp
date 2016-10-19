#include <atomic>
#include <iostream>
#include <thread>

int nax = 0;
int padding;  // Put x and nax into separate shadow caches.
std::atomic<int> x(0);

void setter() {
  nax = 1;
  x.store(1, std::memory_order_release);  // 1st
}

void blocker() {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  x.fetch_add(1, std::memory_order_release);  // 2nd
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  x.store(4, std::memory_order_relaxed);  // 4th, block RS
}

void extender() {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  x.fetch_add(1, std::memory_order_release);  // 3rd
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(40));
  // May synchronise depending on the adversarial strat, if strat is to read old
  // stores, then most likely will sync.
  while (x.load(std::memory_order_acquire) < 4)
    std::this_thread::yield();
  std::cout << "nax: " << nax << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread blk(blocker);
  std::thread ext(extender);
  std::thread get(getter);
  set.join();
  blk.join();
  ext.join();
  get.join();
  return 0;
}
