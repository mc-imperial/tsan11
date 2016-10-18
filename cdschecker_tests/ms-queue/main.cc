#include <stdlib.h>
#include <stdio.h>
#include "cds_threads.h"

#include "my_queue.h"
#include "model-assert.h"

static int procs = 4;
static queue_t *queue;
static thrd_t *threads;
static unsigned int *input;
static unsigned int *output;
static int num_threads;

int get_thread_num()
{
	//thrd_t curr = thrd_current();
	int i;
	for (i = 0; i < num_threads; i++)
		if (std::this_thread::get_id() == threads[i].get_id())
			return i;
	MODEL_ASSERT(0);
	return -1;
}

bool succ1, succ2;

static void main_task(void *param)
{
	unsigned int val;
	int pid = *((int *)param);
	if (!pid) {
		input[0] = 17;
		enqueue(queue, input[0]);
		succ1 = dequeue(queue, &output[0]);
		//printf("Dequeue: %d\n", output[0]);
	} else {
		input[1] = 37;
		enqueue(queue, input[1]);
		succ2 = dequeue(queue, &output[1]);
	}
}

int user_main(int argc, char **argv)
{
	int i;
	int *param;
	unsigned int in_sum = 0, out_sum = 0;

	queue = (queue_t *)calloc(1, sizeof(*queue));
	MODEL_ASSERT(queue);

	num_threads = procs;
	threads = (std::thread *)malloc(num_threads * sizeof(std::thread));
	param = (int *)malloc(num_threads * sizeof(*param));
	input = (unsigned *)calloc(num_threads, sizeof(*input));
	output = (unsigned *)calloc(num_threads, sizeof(*output));

	init_queue(queue, num_threads);
	for (i = 0; i < num_threads; i++) {
		param[i] = i;
		//threads[i] = std::thread(main_task, &param[i]);
                new (&threads[i])std::thread(main_task, &param[i]);
	}
	for (i = 0; i < num_threads; i++)
		threads[i].join();

	for (i = 0; i < num_threads; i++) {
		in_sum += input[i];
		out_sum += output[i];
	}
	for (i = 0; i < num_threads; i++)
		printf("input[%d] = %u\n", i, input[i]);
	for (i = 0; i < num_threads; i++)
		printf("output[%d] = %u\n", i, output[i]);
	if (succ1 && succ2)
		MODEL_ASSERT(in_sum == out_sum);
	else
		MODEL_ASSERT (false);

	free(param);
	free(threads);
	free(queue);

	return 0;
}
