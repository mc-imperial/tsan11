// We try to induce the following modification-order on x:
//  0 -> 1 -> 2 -> 4 -> 3
// This means that the release-sequence headed by the store of 1 to x should be:
//  1 -> 2
// As the relaxed store of 4 to x in the blocking thread blocks the release
// from reaching the store of 3. Therefore, if we read 3 from the acquire load
// in the reading thread, it will not read from the release sequence and so will
// not synchronise with the store of 1 to x. The read and write to nax will
// therefore form a data-race.

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<int> x(0);
int nax = 0;

void setter() {
  nax = 1;
  x.store(1, std::memory_order_release);
  x.store(2, std::memory_order_relaxed);
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  x.store(3, std::memory_order_relaxed);
}

void blocker() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  x.store(4, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(3));
  int x = ::x.load(std::memory_order_acquire);
  while (!x) {
    std::this_thread::yield();
    x = ::x.load(std::memory_order_acquire);
  }
  std::cout << "x: " << x << ", nax: " << nax << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread blk(blocker);
  std::thread get(getter);
  set.join();
  blk.join();
  get.join();
  return 0;
}
