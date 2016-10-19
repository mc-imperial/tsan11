#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int nax, nay;

void setter1() {
  nay = ::y.load(std::memory_order_relaxed);
  x.store(1, std::memory_order_relaxed);
}

void setter2() {
  nax = ::x.load(std::memory_order_relaxed);
  y.store(1, std::memory_order_relaxed);
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  set1.join();
  set2.join();
  // One of the loads must have happened before any of the stores, yet we always
  // get x = y = 1. The loads are being buffer (LB).
  std::cout << "x: " << nax << ", y: " << nay << std::endl;
  return 0;
}
