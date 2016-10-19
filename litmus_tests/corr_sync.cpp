#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
std::atomic<int> msg(0);
int rx1, rx2;

void setter() {
  x.store(1, std::memory_order_relaxed);
  x.store(2, std::memory_order_relaxed);
  x.store(3, std::memory_order_relaxed);
}

void getter1() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  rx1 = x.load(std::memory_order_relaxed);
  msg.store(1, std::memory_order_release);
}

void getter2() {
  while (msg.load(std::memory_order_acquire) < 1)
    std::this_thread::yield();
  // Read must see later in mo than in getter1.
  rx2 = x.load(std::memory_order_relaxed);
  msg.store(2, std::memory_order_release);
}

void reader() {
  while (msg.load(std::memory_order_acquire) < 2)
    std::this_thread::yield();
  std::cout << "rx1: " << rx1 << ", rx2: " << rx2 << std::endl;
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
