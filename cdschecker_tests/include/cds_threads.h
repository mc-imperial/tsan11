// Header to handle CDSChecker bullshit.
// Handles most things, not just threads:
//  - Catches C11 threads and forwards to C++11 threads.
//  - Catches store_n and load_n and turns them into normal stores/loads.
//  - Replaces user_main with just main.

#ifndef __THREADS_H__
#define __THREADS_H__

#ifdef __cplusplus
#include <thread>

#include <map>
#include <string>
#include <cassert>

#define thrd_t std::thread
typedef int (*thrd_start_t)(void *); 

static std::map<std::string, std::thread> thrs;

#define thrd_create(T, F, A) new_thread(#T, F, A)
static void new_thread(const char *t, thrd_start_t f, void *args) {
  assert(thrs.emplace(std::string(++t), std::thread(*f, args)).second);
}

#define thrd_join(T) end_thread(#T)
static void end_thread(const char *t) {
  thrs[std::string(t)].join();
}

#define thrd_yield() std::this_thread::yield()

#define user_main main

#endif

#endif  // __THREADS_H__
