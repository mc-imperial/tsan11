#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
int nax = 0;

void setter() {
  nax = 1;
  x.store(1, std::memory_order_release);
}

void getter() {
  int comp = 1;
  while (!x.compare_exchange_strong(
      comp, 2, std::memory_order_relaxed, std::memory_order_relaxed))
    comp = 1;
  std::cout << "x: " << x.load(std::memory_order_relaxed)
            << ", nax: " << nax << std::endl;
}

int main(int argc, char **argv) {
  std::thread get(getter);
  std::thread set(setter);
  get.join();
  set.join();
  return 0;
}
