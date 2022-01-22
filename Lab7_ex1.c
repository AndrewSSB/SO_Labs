#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define NTHRS 10
sem_t semaphore;
pthread_mutex_t mutex;
int counter = 0;

void barrier_point(){
    pthread_mutex_lock(&mutex); // il tin blocat pana cand am "parcurs" NTHRS
    counter++;
    if (counter == NTHRS){
        sem_post(&semaphore);
    }
    pthread_mutex_unlock(&mutex); // apoi il deblochez si decrementez.

    sem_wait(&semaphore);
    sem_post(&semaphore);
}

void * tfun(void * v){
    int tid = *(int*)v;
    
    printf("%d reached the barrier\n", tid);
    barrier_point();
    printf("%d passed the barrier\n", tid);

    return NULL;
}

int main(){
    pthread_t threads[NTHRS];
    int arg[NTHRS];
    printf("NTHRS = %d\n", NTHRS);

    if(pthread_mutex_init(&mutex, NULL)){
        perror("Failed to create mutex");
        return errno;
    }

    if(sem_init(&semaphore, 0, 0)){// este ca un contor (si suporta doar incrementare si decrementare);
        perror("Failed to create a semaphore"); // si restrictioneaza numarul de threaduri care pot avea acces simultan la aceeasi sursa
        return errno;
    }//sem_post = ++
    //sem_wait = --

    for (int i = 0; i < NTHRS; i++){
        arg[i] = i;
        if(pthread_create(&threads[i], NULL, tfun, &arg[i])){
            perror("Failed to create a thread");
            return errno;
        }
    }

    for (int i = 0; i < NTHRS; i++){
        if(pthread_join(threads[i], NULL)){
            perror("Can't join a thread");
            return errno;
        }
    }

}