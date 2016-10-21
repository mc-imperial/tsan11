#include <stdio.h>

#include "cds_threads.h"

#include "barrier.h"

#include "librace.h"

spinning_barrier *barr;
int var = 0;

void threadA(void *arg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	store_32(&var, 1);
	barr->wait();
}

void threadB(void *arg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	barr->wait();
	printf("var = %d\n", load_32(&var));
}

#define NUMREADERS 3
int user_main(int argc, char **argv)
{
	std::thread A, B[NUMREADERS];
	int i;

	barr = new spinning_barrier(NUMREADERS + 1);

	A = std::thread(threadA, (void *)NULL);
	for (i = 0; i < NUMREADERS; i++)
		B[i] = std::thread(threadB, (void *)NULL);

	for (i = 0; i < NUMREADERS; i++)
		B[i].join();
	A.join();

	return 0;
}
