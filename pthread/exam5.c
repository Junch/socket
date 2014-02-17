#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

const int NUM = 5;
int queue[NUM];
sem_t blank_number;
sem_t product_number;

void *produce(void *arg)
{
	int p = 0;
	for (;;) {
		sem_wait(&blank_number);
		queue[p] = rand() % 1000 + 1;
		printf("Produce %d\n", queue[p]);
		sem_post(&product_number);
		p = (p+1) % NUM;
		sleep(rand()%5);
	}
}

void *consume(void *arg)
{
	int c = 0;
	for (;;) {
		sem_wait(&product_number);
		printf("Consume %d\n", queue[c]);
		queue[c] = 0;
		sem_post(&blank_number);
		c = (c+1) % NUM;
		sleep(rand()%5);
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	pthread_t pid, cid;

	sem_init(&blank_number, 0, NUM);
	sem_init(&product_number, 0, 0);
	
	pthread_create(&pid, NULL, produce, NULL);
	pthread_create(&cid, NULL, consume, NULL);
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	sem_destroy(&blank_number);
	sem_destroy(&product_number);

	return 0;
}