#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int num = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *print1(void *arg){
	pthread_mutex_lock(&mutex);
	++num; 
	printf("%d\n", num);
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	return NULL;
}

void *print2(void *arg){
	pthread_mutex_lock(&mutex);
	while(num != 1)
		pthread_cond_wait(&cond, &mutex);
	++num; 
	printf("%d\n", num);
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	return NULL;
}

void *print3(void *arg){
	pthread_mutex_lock(&mutex);
	while(num != 2)
		pthread_cond_wait(&cond, &mutex);
	++num; 
	printf("%d\n", num);
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	return NULL;
}

void *print4(void *arg){
	pthread_mutex_lock(&mutex);
	while(num != 3)
		pthread_cond_wait(&cond, &mutex);
	++num; 
	printf("%d\n", num);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	pthread_t tid1,tid2,tid3,tid4;

	pthread_create(&tid1, NULL, print1, NULL);
	pthread_create(&tid2, NULL, print2, NULL);
	pthread_create(&tid3, NULL, print3, NULL);
	pthread_create(&tid4, NULL, print4, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);

	pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
	return 0;
}