#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x(0), y(0);
int r;
int s1, s2, s3;
int t1, t2, t3;

void setter1() {
  x.store(1, std::memory_order_relaxed);
  std::this_thread::yield();
  x.store(2, std::memory_order_seq_cst);
  y.store(1, std::memory_order_seq_cst);
}

void setter2() {
  x.store(3, std::memory_order_relaxed);
  y.store(2, std::memory_order_seq_cst);
}

void setter3() {
  y.store(3, std::memory_order_seq_cst);
  r = x.load(std::memory_order_seq_cst);
}

void reader() {
  s1 = x.load(std::memory_order_relaxed);
  s2 = x.load(std::memory_order_relaxed);
  s3 = x.load(std::memory_order_relaxed);
  t1 = y.load(std::memory_order_relaxed);
  t2 = y.load(std::memory_order_relaxed);
  t3 = y.load(std::memory_order_relaxed);
}

int main( int argc, char **argv) {
  std::thread set1(setter1);
  std::thread set2(setter2);
  std::thread set3(setter3);
  std::thread read(reader);
  set1.join();
  set2.join();
  set3.join();
  read.join();
  // To enforce reading 1,2,3,1,2,3 for s1 to t3, the memory-order and thus
  // sequential-consistent ordering are constrained such that the the sc write
  // of 2 to x happens before the sc read from x into r.
  std::cout << "r: " << r << std::endl;
  std::cout << "s1: " << s1 << ", s2: " << s2 << ", s3: " << s3 << std::endl;
  std::cout << "t1: " << t1 << ", t2: " << t2 << ", t3: " << t3 << std::endl;
  return 0;
}
