#include <atomic>
#include <iostream>
#include <thread>

struct Coord { int x, y; Coord(int x, int y) : x(x), y(y) {} };

std::atomic<struct Coord *> c(NULL);
std::atomic<int> x(0), y(0);

void setter() {
  x.store(1, std::memory_order_relaxed);
  y.store(2, std::memory_order_relaxed);
  c.store(new struct Coord(1, 2), std::memory_order_release);
}

void getter() {
  while (c.load(std::memory_order_consume) == NULL)
    std::this_thread::yield();
  // As the is a data dependence between c and both c-> and c->y, we can see the
  // side-effects of the stores. The synchronisation does not extend to x and y
  // as there is no carries-a-dependence to the reads.
  struct Coord *c = ::c.load(std::memory_order_consume);
  int x = ::x.load(std::memory_order_relaxed);
  int y = ::y.load(std::memory_order_relaxed);
  std::cout << "c->x: " << c->x << ", c->y: " << c->y
            << ", x: " << x << ", y: " << y << std::endl;
  delete c;
}

int main(int argc, char **argv) {
  std::thread set(setter);
  std::thread get(getter);
  set.join();
  get.join();
  return 0;
}
