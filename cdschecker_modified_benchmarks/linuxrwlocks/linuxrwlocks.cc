#include <stdio.h>
#include "cds_threads.h"
#include "cds_atomic.h"

#include "librace.h"

#define RW_LOCK_BIAS            0x00100000
#define WRITE_LOCK_CMP          RW_LOCK_BIAS

/** Example implementation of linux rw lock along with 2 thread test
 *  driver... */

typedef union {
	atomic_int lock;
} rwlock_t;

static inline int read_can_lock(rwlock_t *lock)
{
	return atomic_load_explicit(&lock->lock, memory_order_relaxed) > 0;
}

static inline int write_can_lock(rwlock_t *lock)
{
	return atomic_load_explicit(&lock->lock, memory_order_relaxed) == RW_LOCK_BIAS;
}

static inline void read_lock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(&rw->lock, 1, memory_order_acquire);
	while (priorvalue <= 0) {
		atomic_fetch_add_explicit(&rw->lock, 1, memory_order_relaxed);
		while (atomic_load_explicit(&rw->lock, memory_order_relaxed) <= 0) {
			thrd_yield();
		}
		priorvalue = atomic_fetch_sub_explicit(&rw->lock, 1, memory_order_relaxed);
	}
}

static inline void write_lock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(&rw->lock, RW_LOCK_BIAS, memory_order_acquire);
	while (priorvalue != RW_LOCK_BIAS) {
		atomic_fetch_add_explicit(&rw->lock, RW_LOCK_BIAS, memory_order_relaxed);
		while (atomic_load_explicit(&rw->lock, memory_order_relaxed) != RW_LOCK_BIAS) {
			thrd_yield();
		}
		priorvalue = atomic_fetch_sub_explicit(&rw->lock, RW_LOCK_BIAS, memory_order_relaxed);
	}
}

static inline int read_trylock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(&rw->lock, 1, memory_order_acquire);
	if (priorvalue > 0)
		return 1;

	atomic_fetch_add_explicit(&rw->lock, 1, memory_order_relaxed);
	return 0;
}

static inline int write_trylock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(&rw->lock, RW_LOCK_BIAS, memory_order_acquire);
	if (priorvalue == RW_LOCK_BIAS)
		return 1;

	atomic_fetch_add_explicit(&rw->lock, RW_LOCK_BIAS, memory_order_relaxed);
	return 0;
}

static inline void read_unlock(rwlock_t *rw)
{
	atomic_fetch_add_explicit(&rw->lock, 1, memory_order_release);
}

static inline void write_unlock(rwlock_t *rw)
{
	atomic_fetch_add_explicit(&rw->lock, RW_LOCK_BIAS, memory_order_release);
}

rwlock_t mylock;
int shareddata;

void a()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int i, rs;
	for(i = 0; i < 2; i++) {
		if ((i % 2) == 0) {
			read_lock(&mylock);
			rs = load_32(&shareddata);
			read_unlock(&mylock);
		} else {
			write_lock(&mylock);
			std::this_thread::yield();
			store_32(&shareddata,(unsigned int)i);
			write_unlock(&mylock);
		}
	}
}

int user_main(int argc, char **argv)
{
	//thrd_t t1, t2;
	atomic_init(&mylock.lock, RW_LOCK_BIAS);

        std::thread t1(a);
        std::thread t2(a);
	//thrd_create(&t1, (thrd_start_t)&a, NULL);
	//thrd_create(&t2, (thrd_start_t)&a, NULL);

	t1.join();
	t2.join();
	//thrd_join(t1);
	//thrd_join(t2);

	return 0;
}
