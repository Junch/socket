#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
char flag = 'A';

void *print(void *arg)
{
	char c = (char)arg;

	for (int i = 0; i < 10; ++i){
		pthread_mutex_lock(&lock);
		while (flag != c)
			pthread_cond_wait(&cond, &lock);
		printf("%c", c);
		if (++flag == 'D')
			flag = 'A';
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t pid[3];
	for (int i = 0; i < 3; ++i)
		pthread_create(&pid[i], NULL, print, (void *)('A' + i));

	for (int i = 0; i < 3; ++i)
		pthread_join(pid[i], NULL);

	printf("\n");

	return 0;
}