#include <atomic>
#include <iostream>
#include <thread>

int nax = 0;
std::atomic<int> x(0);

void setter() {
  nax = 1;
  x.fetch_add(1, std::memory_order_release);
}

void getter() {
  while (!x.load(std::memory_order_acquire))
    std::this_thread::yield();
  // RMW should act as release store.
  int x = ::x.load(std::memory_order_acquire);
  std::cout << "x: " << x << " nax: " << nax << std::endl;
}

int main(int argc, char **argv) {
  std::thread get(getter);
  std::thread set(setter);
  get.join();
  set.join();
  return 0;
}
