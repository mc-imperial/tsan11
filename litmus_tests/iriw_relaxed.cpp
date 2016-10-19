#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int nax1, nay1;
int nax2, nay2;

void setter1() {
  x.store(1, std::memory_order_relaxed);
}

void setter2() {
  y.store(1, std::memory_order_relaxed);
}

void getter1() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  nax1 = x.load(std::memory_order_relaxed);
  nay1 = y.load(std::memory_order_relaxed);
}

void getter2() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  nay2 = y.load(std::memory_order_relaxed);
  nax2 = x.load(std::memory_order_relaxed);
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  std::thread get1(getter1);
  std::thread get2(getter2);
  set1.join();
  set2.join();
  get1.join();
  get2.join();
  // The two getters see different stores. Violates sequential consistency.
  // Can see any combination of 0 and 1.
  std::cout << "nax1: " << nax1 << ", nay1: " << nay1 << std::endl;
  std::cout << "nax2: " << nax2 << ", nay2: " << nay2 << std::endl;
  return 0;
}
