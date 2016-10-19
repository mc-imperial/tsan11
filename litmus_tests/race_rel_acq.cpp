#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> msg(0);
int x;

void setter() {
  x = 1;
  msg.store(1, std::memory_order_release);
}

void getter() {
  msg.load(std::memory_order_acquire);
  // The load is not guaranteed to synchronise with the store. If it doesn't,
  // there is a data-race.
  std::cout << "x: " << x << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
