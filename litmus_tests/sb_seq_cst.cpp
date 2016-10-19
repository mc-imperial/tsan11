#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int nax, nay;

void setter1() {
  x.store(1, std::memory_order_seq_cst);
  nay = y.load(std::memory_order_seq_cst);
}

void setter2() {
  y.store(1, std::memory_order_seq_cst);
  nax = x.load(std::memory_order_seq_cst);
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  set1.join();
  set2.join();
  // If both stores are buffered, we could see both 0.
  std::cout << "nax: " << nax << ", nay: " << nay << std::endl;
  return 0;
}
