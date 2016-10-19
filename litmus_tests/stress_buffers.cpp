#include <atomic>
#include <iostream>
#include <thread>
#include <cstdlib>

static const int kVarCount = 2000;
std::atomic<int> *x;
std::atomic<int> *y;
std::atomic<int> rel;
int s11_x, s21_x;
int s12_x, s22_x;
int s11_y, s21_y;
int s12_y, s22_y;

void setter1() {
  for (int i = 0; i < kVarCount; ++i) {
    x[i].store(i, std::memory_order_relaxed);
    y[i].store(i, std::memory_order_relaxed);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  for (int i = 0; i < kVarCount; ++i) {
    y[i].store(i, std::memory_order_relaxed);
    x[i].store(i, std::memory_order_relaxed);
  }
}

void setter2() {
  for (int i = 0; i < kVarCount; ++i) {
    y[i].store(2 * i, std::memory_order_relaxed);
    x[i].store(2 * i, std::memory_order_relaxed);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  for (int i = 0; i < kVarCount; ++i) {
    x[i].store(2 * i, std::memory_order_relaxed);
    y[i].store(2 * i, std::memory_order_relaxed);
  }
}

void getter1() {
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  for (int i = 0; i < kVarCount; ++i) {
    x[i].load(std::memory_order_relaxed);
    y[i].load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s11_x = x[kVarCount / 2].load(std::memory_order_relaxed);
  s11_y = y[kVarCount / 2].load(std::memory_order_relaxed);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  for (int i = 0; i < kVarCount; ++i) {
    y[i].load(std::memory_order_relaxed);
    x[i].load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s12_x = x[kVarCount / 2].load(std::memory_order_relaxed);
  s12_y = y[kVarCount / 2].load(std::memory_order_relaxed);
}

void getter2() {
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  for (int i = 0; i < kVarCount; ++i) {
    y[i].load(std::memory_order_relaxed);
    x[i].load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s21_x = x[kVarCount / 2].load(std::memory_order_relaxed);
  s21_y = y[kVarCount / 2].load(std::memory_order_relaxed);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  for (int i = 0; i < kVarCount; ++i) {
    x[i].load(std::memory_order_relaxed);
    y[i].load(std::memory_order_relaxed);
    rel.store(1, std::memory_order_release);
  }
  s22_x = x[kVarCount / 2].load(std::memory_order_relaxed);
  s22_y = y[kVarCount / 2].load(std::memory_order_relaxed);
}

int main(int argc, char **argv) {
 // x = new std::atomic<int>[kVarCount];
 // y = new std::atomic<int>[kVarCount];
  x = (std::atomic<int> *)malloc(sizeof(std::atomic<int>) * kVarCount);
  y = (std::atomic<int> *)malloc(sizeof(std::atomic<int>) * kVarCount);
  for (int i = 0; i < kVarCount; ++i) {
  //  new(&x[i]) std::atomic<int>();
  //  new(&y[i]) std::atomic<int>();
  }
  std::thread set11(setter1);
  std::thread set21(setter2);
  std::thread get11(getter1);
  std::thread get21(getter2);
  set11.join();
  set21.join();
  get11.join();
  get21.join();
 // delete[] x;
 // delete[] y;
  for (int i = 0; i < kVarCount; ++i) {
  //  x[i].~atomic<int>();
  //  y[i].~atomic<int>();
  }
  free(x);
  free(y);
 // x = new std::atomic<int>[kVarCount];
 // y = new std::atomic<int>[kVarCount];
  x = (std::atomic<int> *)malloc(sizeof(std::atomic<int>) * kVarCount);
  y = (std::atomic<int> *)malloc(sizeof(std::atomic<int>) * kVarCount);
  for (int i = 0; i < kVarCount; ++i) {
  //  new(&x[i]) std::atomic<int>();
  //  new(&y[i]) std::atomic<int>();
  }
  std::thread set12(setter1);
  std::thread set22(setter2);
  std::thread get12(getter1);
  std::thread get22(getter2);
  set12.join();
  set22.join();
  get12.join();
  get22.join();
 // delete[] x;
 // delete[] y;
  for (int i = 0; i < kVarCount; ++i) {
  //  x[i].~atomic<int>();
  //  y[i].~atomic<int>();
  }
  free(x);
  free(y);
  std::cout << "s11_x: " << s11_x << ", s21_x: " << s21_x << std::endl;
  std::cout << "s12_x: " << s12_x << ", s22_x: " << s22_x << std::endl;
  std::cout << "s11_y: " << s11_y << ", s21_y: " << s21_y << std::endl;
  std::cout << "s12_y: " << s12_y << ", s22_y: " << s22_y << std::endl;
  return 0;
}
