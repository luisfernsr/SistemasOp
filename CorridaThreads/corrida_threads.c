#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

#define MAX_COUNT 100

int winner = -1; 
pthread_mutex_t winner_mutex;
pthread_barrier_t barrier;

void* thread_func(void* arg){
    int id = *(int*)arg;

    pthread_barrier_wait(&barrier);

    for(int i=0; i < MAX_COUNT; i++){
        printf("Thread %d: %d\n", id, i);
        usleep(rand() % 5000);
    }

    pthread_mutex_lock(&winner_mutex);
    if(winner == -1){
        winner = id;
    }
    pthread_mutex_unlock(&winner_mutex);
    return NULL;
}

int main(){
    int num_threads;
    printf("Número de threads: ");
    scanf("%d", &num_threads);

    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    pthread_mutex_init(&winner_mutex, NULL);
    pthread_barrier_init(&barrier, NULL, num_threads);

    for(int i = 0; i < num_threads; i++){
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
    }

    for(int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Winner: Thread %d\n", winner);

    pthread_mutex_destroy(&winner_mutex);
    pthread_barrier_destroy(&barrier);

    return 0;
}
