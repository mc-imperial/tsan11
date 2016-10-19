#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
std::atomic<int> msg1(0);
std::atomic<int> msg2(0);
int rx1, rx2, rx3;

void setter() {
  x.store(1, std::memory_order_relaxed);
  x.store(2, std::memory_order_relaxed);
  x.store(3, std::memory_order_relaxed);
}

void getter1() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  rx1 = x.load(std::memory_order_relaxed);
  msg1.store(1, std::memory_order_release);
  rx2 = x.load(std::memory_order_relaxed);
  msg2.store(1, std::memory_order_release);
}

void getter2() {
  while (msg1.load(std::memory_order_acquire) < 1)
    std::this_thread::yield();
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  // Read must see the same or later in mo than rx1, but possibly earlier in mo
  // than rx2.
  rx3 = x.load(std::memory_order_relaxed);
  msg1.store(2, std::memory_order_release);
}

void reader() {
  while (msg1.load(std::memory_order_acquire) < 2)
    std::this_thread::yield();
  while (msg2.load(std::memory_order_acquire) < 1)
    std::this_thread::yield();
  std::cout << "rx1: " << rx1 << ", rx2: " << rx2
            << ", rx3: " << rx3 << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get1(getter1);
  std::thread get2(getter2);
  std::thread read(reader);
  set.join();
  get1.join();
  get2.join();
  read.join();
  return 0;
}
