#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);

void setter1() {
  for (int i = 0; i < 500; ++i)
    x.store(i, std::memory_order_relaxed);
}

void setter2() {
  for (int i = 0; i < 500; ++i)
    x.store(2 * i, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  //while (x.load(std::memory_order_acquire) < 500)
  //  std::this_thread::yield();
  int x = ::x.load(std::memory_order_acquire);
  std::cout << "x: " << x << std::endl;
  ::x.load(std::memory_order_acquire);
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
