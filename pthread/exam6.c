#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define NUM 1
int queue[NUM];
sem_t* blank_number;
sem_t* product_number;

void *produce(void *arg)
{
	int p = 0;
	for (;;) {
		sem_wait(blank_number);
		queue[p] = rand() % 1000 + 1;
		printf("Produce %d\n", queue[p]);
		sem_post(product_number);
		p = (p+1) % NUM;
		sleep(rand()%5);
	}
}

void *consume(void *arg)
{
	int c = 0;
	for (;;) {
		sem_wait(product_number);
		printf("Consume %d\n", queue[c]);
		queue[c] = 0;
		sem_post(blank_number);
		c = (c+1) % NUM;
		sleep(rand()%5);
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	sem_unlink("/blank_number");
	sem_unlink("/product_number");

	pthread_t pid, cid;

    // If O_EXCL is specified and the semaphore exists, sem_open() fails.
	if ((blank_number = sem_open("/blank_number", O_CREAT|O_EXCL , 0700, 1)) == SEM_FAILED ) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

	if ((product_number = sem_open("/product_number", O_CREAT|O_EXCL , 0700, 0)) == SEM_FAILED ) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

	pthread_create(&pid, NULL, produce, NULL);
	pthread_create(&cid, NULL, consume, NULL);
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	sem_close(blank_number);
	sem_close(product_number);

	sem_unlink("/blank_number");
	sem_unlink("/product_number");

	return 0;
}