#include <stdio.h>
#include "cds_threads.h"

#include "mcs-lock.h"

/* For data race instrumentation */
#include "librace.h"

struct mcs_mutex *mutex;
static uint32_t shared;

void threadA(void *arg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	mcs_mutex::guard g(mutex);
	printf("store: %d\n", 17);
	store_32(&shared, 17);
	mutex->unlock(&g);
	mutex->lock(&g);
	printf("load: %u\n", load_32(&shared));
}

void threadB(void *arg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	mcs_mutex::guard g(mutex);
	printf("load: %u\n", load_32(&shared));
	mutex->unlock(&g);
	mutex->lock(&g);
	printf("store: %d\n", 17);
	store_32(&shared, 17);
}

int user_main(int argc, char **argv)
{
	mutex = new mcs_mutex();

	std::thread A(threadA, (void *)NULL);
	std::thread B(threadB, (void *)NULL);
	A.join();
	B.join();
	return 0;
}
