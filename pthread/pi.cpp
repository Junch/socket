#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_N_THREADS 20
static long num_steps=800000000; 
double step = 1.0 / (double)num_steps;
double pi;
unsigned long length = 0;
double g_sum = 0;
pthread_mutex_t mtSum;

void* work(void* p)
{
    timeval tstart, tend;
    gettimeofday(&tstart, NULL);
    unsigned long ithread = reinterpret_cast<unsigned long>(p);
    unsigned long start = ithread * length;
    unsigned long end = length + start;

    printf("thread %lu start, start = %lu, length = %lu, g_sum = %lf\n", ithread, start, end, g_sum); 
    unsigned long i = start; 
    double x = 0.0, sum = 0.0;
    for ( ; i < end; i++) {
        x = (i + 0.5)*step;
        sum = sum + 4.0/(1.0 + x*x);
    } 
    gettimeofday(&tend, NULL);
    double tcost = tend.tv_sec - tstart.tv_sec + (double)(tend.tv_usec-tstart.tv_usec)/1000000.0; 
    printf("thread %lu calculate end, cost %10.6f seconds\n", ithread, tcost);

    pthread_mutex_lock(&mtSum);
    g_sum += sum;
    pthread_mutex_unlock(&mtSum);

    gettimeofday(&tend, NULL);
    tcost = tend.tv_sec - tstart.tv_sec + (double)(tend.tv_usec-tstart.tv_usec)/1000000.0;
    printf("thread %lu exit, sum = %lf, g_sum = %lf, cost %10.6f seconds\n", ithread, sum, g_sum, tcost);
    pthread_exit((void*)0);
}

int main(int argc, char* argv[])
{
    unsigned int nthreads;
    for (int i = 1; i < argc; i++) {
        char *ts = strstr(argv[i], "-p=");
        if (ts == NULL)
            continue;
        sscanf(ts,"-p=%d", &nthreads);
    } 

    printf("nthreads = %d\n", nthreads);

    timeval start;
    gettimeofday(&start, NULL);

    pthread_t threads[MAX_N_THREADS];
    pthread_mutex_init(&mtSum, NULL);
    
    length = (unsigned long)ceil(num_steps / nthreads);
    for (int i = 0; i < nthreads; i++){
        int rc = pthread_create(&threads[i], NULL, work, reinterpret_cast<void*>(i));
        if (rc) {
            printf("ERROR; return code from pthread_create(%d) is %d\n",i, rc); 
            exit(-1); 
        }
    }

    for (int i = 0; i < nthreads; i++) 
       pthread_join(threads[i], NULL); 

    pi = step * g_sum;

    timeval end;
    gettimeofday(&end, NULL);
    double tcost = end.tv_sec - start.tv_sec + (double)(end.tv_usec-start.tv_usec)/1000000.0;
    printf("Pi = %12.9f, cost %10.6f seconds\n", pi, tcost);

    pthread_mutex_destroy(&mtSum);
    return 0;
}