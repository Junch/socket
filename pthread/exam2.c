#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int num = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_a;
pthread_cond_t cond_b;
pthread_cond_t cond_c;
pthread_cond_t cond_d;

void *a(void *arg){
	for (;;) {
		pthread_mutex_lock(&mutex);
    	pthread_cond_wait(&cond_a, &mutex);
		printf("%d a\n", num);
		++num;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond_b);
	}
	return NULL;
}

void *b(void *arg){
	for (;;) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond_b, &mutex);
		printf("%d b\n", num);	
		++num; 
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond_c);
	}
	return NULL;
}

void *c(void *arg){
	for (;;) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond_c, &mutex);
		printf("%d c\n", num);
		--num; 
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond_d);
	}
	return NULL;
}

void *d(void *arg){
	for (;;) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond_d, &mutex);
		printf("%d d\n", num);
		--num; 
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond_a);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_a, NULL);
	pthread_cond_init(&cond_b, NULL);
	pthread_cond_init(&cond_c, NULL);
	pthread_cond_init(&cond_d, NULL);

	pthread_t tid1,tid2,tid3,tid4;

	pthread_create(&tid1, NULL, a, NULL);
	pthread_create(&tid2, NULL, b, NULL);
	pthread_create(&tid3, NULL, c, NULL);
	pthread_create(&tid4, NULL, d, NULL);

    pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond_a);
	pthread_mutex_unlock(&mutex);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);

	pthread_cond_destroy(&cond_a);
	pthread_cond_destroy(&cond_b);
	pthread_cond_destroy(&cond_c);
	pthread_cond_destroy(&cond_d);
    pthread_mutex_destroy(&mutex);
	return 0;
}