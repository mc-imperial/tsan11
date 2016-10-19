#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> t1_1(0), t1_2(0), t1_3(0);
std::atomic<int> t2_1(0), t2_2(0), t2_3(0);
std::atomic<int> t3_1(0), t3_2(0), t3_3(0);
std::atomic<bool> t3sync(false);
int res[9];

void setter1() {
  t1_1.store(1, std::memory_order_relaxed);
  t1_2.store(1, std::memory_order_relaxed);
  t1_3.store(1, std::memory_order_relaxed);
}

void setter2() {
  t2_1.store(1, std::memory_order_relaxed);
  t2_2.store(1, std::memory_order_relaxed);
  t2_3.store(1, std::memory_order_relaxed);
}

void setter3() {
  t3_1.store(1, std::memory_order_relaxed);
  t3_2.store(1, std::memory_order_relaxed);
  t3_3.store(1, std::memory_order_relaxed);
  t3sync.store(true, std::memory_order_release);
}

void reader() {
  // Creates a 'synchronise-with' edge between setter3 and the reader, causing
  // the stores to t3 to 'happen before' the loads here and so always read 1.
  // The others are not ordered so can return 0 or 1.
  while (!t3sync.load(std::memory_order_acquire))
    std::this_thread::yield();
  res[0] = t1_1.load(std::memory_order_relaxed);
  res[1] = t1_2.load(std::memory_order_relaxed);
  res[2] = t1_3.load(std::memory_order_relaxed);
  res[3] = t2_1.load(std::memory_order_relaxed);
  res[4] = t2_2.load(std::memory_order_relaxed);
  res[5] = t2_3.load(std::memory_order_relaxed);
  res[6] = t3_1.load(std::memory_order_relaxed);
  res[7] = t3_2.load(std::memory_order_relaxed);
  res[8] = t3_3.load(std::memory_order_relaxed);
}

int main(int argc, char **argv) {
  std::thread set3(setter3);
  std::thread set1(setter1);
  std::thread set2(setter2);
  std::thread read(reader);
  set3.join();
  set1.join();
  set2.join();
  read.join();
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j)
      std::cout << res[i * 3 + j] << ", ";
    std::cout << std::endl;
  }
  return 0;
}
