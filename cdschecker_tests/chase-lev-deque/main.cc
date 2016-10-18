#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "cds_threads.h"
//#include <atomic>

#include "model-assert.h"

#include "deque.h"

Deque *q;
int a;
int b;
int c;

static void task(void * param) {
	a=steal(q);
}

int user_main(int argc, char **argv)
{
	q=create();
	std::thread t(task, (void *)0);
	push(q, 1);
	push(q, 2);
	push(q, 4);
	b=take(q);
	c=take(q);
	t.join();

	bool correct=true;
	if (a!=1 && a!=2 && a!=4 && a!= EMPTY)
		correct=false;
	if (b!=1 && b!=2 && b!=4 && b!= EMPTY)
		correct=false;
	if (c!=1 && c!=2 && c!=4 && a!= EMPTY)
		correct=false;
	if (a!=EMPTY && b!=EMPTY && c!=EMPTY && (a+b+c)!=7)
		correct=false;
	if (!correct)
		printf("a=%d b=%d c=%d\n",a,b,c);
	MODEL_ASSERT(correct);

	return 0;
}
