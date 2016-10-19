#include <atomic>
#include <iostream>
#include <thread>

struct segment {
  int x[10];
  int y[10];
  int z[10];
};

struct segment seg;

void setter1() {
  std::this_thread::yield();
  struct segment seg = {
    {1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10},
    {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
    {21, 22, 23, 24, 25, 26, 27, 28, 29, 30}
  };
  ::seg = seg;
}

void setter2() {
  struct segment seg = {
    {31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
    {41, 42, 43, 44, 45, 46, 47, 48, 49, 50},
    {51, 52, 53, 54, 55, 56, 57, 58, 59, 60}
  };
  ::seg = seg;
}

int main(int argc, char **argv) {
  // Both of the assignments to ::seg will be a memcpy, and so there will be a
  // fairly large data race on a block of memory.
  std::thread set1(setter1);
  std::thread set2(setter2);
  set1.join();
  set2.join();
  for (int i = 0; i < 10; ++i) std::cout << seg.x[i] << ", ";
  std::cout << std::endl;
  for (int i = 0; i < 10; ++i) std::cout << seg.y[i] << ", ";
  std::cout << std::endl;
  for (int i = 0; i < 10; ++i) std::cout << seg.z[i] << ", ";
  std::cout << std::endl;
  return 0;
}
