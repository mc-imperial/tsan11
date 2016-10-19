#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> msg(0);
int x;

void setter() {
  x = 1;
  msg.store(1, std::memory_order_relaxed);
}

void getter() {
  while (!msg.load(std::memory_order_acquire))
   std::this_thread::sleep_for(std::chrono::milliseconds(2));
  // No synchronisation.
  std::cout << "x: " << x << std::endl;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
