#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);

void setter1() {
  x.store(1, std::memory_order_relaxed);
}

void setter2() {
  x.store(2, std::memory_order_relaxed);
  x.store(3, std::memory_order_seq_cst);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  int rx = x.load(std::memory_order_seq_cst);
  // SC read cannot see 2, but can see 1.
  std::cout << "rx: " << rx << std::endl;
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  std::thread get(getter);
  set1.join();
  set2.join();
  get.join();
  return 0;
}
