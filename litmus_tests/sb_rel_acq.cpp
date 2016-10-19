#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int nax, nay;

void setter1() {
  x.store(1, std::memory_order_release);
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  nay = y.load(std::memory_order_acquire);
}

void setter2() {
  y.store(1, std::memory_order_release);
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  nax = x.load(std::memory_order_acquire);
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
