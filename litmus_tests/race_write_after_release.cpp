#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<int> msg(0);
int x;

void setter() {
  msg.store(1, std::memory_order_release);
  x = 1;
  msg.store(2, std::memory_order_relaxed);
}

void getter() {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  int msg = ::msg.load(std::memory_order_acquire);
  // If 2 is read, we have synchronised on msg via the relaxed store (as it is
  // part of the release sequence), but the release occurs before writing to x,
  // so there is a data race.
  std::cout << "msg: " << msg << ", x: " << x << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
