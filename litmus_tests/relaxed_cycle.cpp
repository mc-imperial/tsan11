#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int nax, nay;

void setter1() {
  if (y.load(std::memory_order_relaxed))
    x.store(1, std::memory_order_relaxed);
  nax = x.load(std::memory_order_relaxed);
}

void setter2() {
  if (x.load(std::memory_order_relaxed))
    y.store(1, std::memory_order_relaxed);
  nay = y.load(std::memory_order_relaxed);
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  set1.join();
  set2.join();
  // Each store depends on the other, so no store should ever occur, but it does
  // (e.g. due to speculative loading).
  std::cout << "x: " << x << ", y: " << y << std::endl;
  return 0;
}
