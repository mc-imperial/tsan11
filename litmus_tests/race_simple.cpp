#include <atomic>
#include <iostream>
#include <thread>

int x = 0;

void setter1() {
  x = 1;
}

void setter2() {
  x = 2;
}

int main(int argc, char **argv) {
  // Simple write-write race on x.
  std::thread set1(setter1);
  std::thread set2(setter2);
  set1.join();
  set2.join();
  std::cout << "x: " << x << std::endl;
  return 0;
}
