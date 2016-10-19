#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
std::atomic<int> acc(0);
std::atomic<int> syncv(0);

void setter1() {
  std::this_thread::yield();
  x.store(1, std::memory_order_seq_cst);
}

void setter2() {
  std::this_thread::yield();
  y.store(1, std::memory_order_seq_cst);
}

void getter1() {
  while (!x.load(std::memory_order_seq_cst))
    std::this_thread::yield();
  // If x has been set but y has not, acc will not be incremented. However, the
  // other getter checks for y first, and when y has been set, due to sequential
  // consistency, x must have been set. So acc must be at least 1 (possibly 2).
  if (y.load(std::memory_order_seq_cst))
    ++acc;
  ++syncv;
}

void getter2() {
  while (!y.load(std::memory_order_seq_cst))
    std::this_thread::yield();
  if (x.load(std::memory_order_seq_cst))
    ++acc;
  ++syncv;
}

void reader() {
  while (syncv.load(std::memory_order_seq_cst) < 2)
    std::this_thread::yield();
  int acc = ::acc.load(std::memory_order_seq_cst);
  std::cout << "acc: " << acc << std::endl;
}

int main(int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  std::thread get1(getter1);
  std::thread get2(getter2);
  std::thread read(reader);
  set1.join();
  set2.join();
  get1.join();
  get2.join();
  read.join();
  return 0;
}
