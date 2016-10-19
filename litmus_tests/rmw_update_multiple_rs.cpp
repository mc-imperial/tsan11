#include <atomic>
#include <iostream>
#include <thread>

int nax = 0;
int nay = 0;
std::atomic<int> x(0);

void setter1() {
  nax = 1;
  x.fetch_add(1, std::memory_order_release);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  nax = 2;
  x.fetch_add(1, std::memory_order_release);
}

void setter2() {
  nay = 1;
  x.fetch_add(1, std::memory_order_release);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  nay = 2;
  x.fetch_add(1, std::memory_order_release);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  while (x.load(std::memory_order_acquire) < 4)
    std::this_thread::yield();
  // RMW should have both active RSs.
  int x = ::x.load(std::memory_order_acquire);
  std::cout << "x: " << x << " nax: " << nax << " nay: " << nay << std::endl;
}

int main(int argc, char **argv) {
  std::thread get(getter);
  std::thread set1(setter1);
  std::thread set2(setter2);
  get.join();
  set1.join();
  set2.join();
  return 0;
}
