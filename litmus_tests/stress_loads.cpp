#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
std::atomic<int> rel(0);
int s11_x, s21_x;
int s12_x, s22_x;

void setter() {
  for (int i = 0; i < 500; ++i)
    x.store(i, std::memory_order_relaxed);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  for (int i = 500; i < 1000; ++i)
    x.store(i, std::memory_order_relaxed);
}

void getter1() {
  std::this_thread::sleep_for(std::chrono::milliseconds(25));
  for (int i = 0; i < 100; ++i) {
    ::x.load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s11_x = ::x.load(std::memory_order_relaxed);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  for (int i = 0; i < 100; ++i) {
    ::x.load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s12_x = ::x.load(std::memory_order_relaxed);
}

void getter2() {
  std::this_thread::sleep_for(std::chrono::milliseconds(25));
  for (int i = 0; i < 100; ++i) {
    ::x.load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s21_x = ::x.load(std::memory_order_relaxed);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  for (int i = 0; i < 100; ++i) {
    ::x.load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s22_x = ::x.load(std::memory_order_relaxed);
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get1(getter1);
  std::thread get2(getter2);
  set.join();
  get1.join();
  get2.join();
  std::cout << "s11_x: " << s11_x << ", s21_x: " << s21_x << std::endl;
  std::cout << "s12_x: " << s12_x << ", s22_x: " << s22_x << std::endl;
  return 0;
}
