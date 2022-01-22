#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define NTHREAD 5
#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;

//am cautat despre pthread_cond pe stackoverflow: stackoverflow.com/questions/34512/what-is-a-deadlock/34520
                                            //  linux.die.net/man/3/pthread_cond_signal
pthread_mutex_t mutex; //de obicei se declara ca variabile globale sau intr-o structura (pentru a fi usor accesibila de oriunde)
pthread_cond_t cond;    

int decrease_count(int count){
    pthread_mutex_lock(&mutex);
    
    if (available_resources < count){
        pthread_cond_wait(&cond, &mutex); // Pun o conditie pe mutex in caz ca se va ajunge la un deadlock
    }

    available_resources -= count;
    printf("Got %d resources %d remaining\n", count, available_resources);    

    pthread_mutex_unlock(&mutex);
    return 0;
}

int increase_count(int count){
    pthread_mutex_lock(&mutex);
    available_resources += count;
    printf("Released %d resources %d remainig\n", count, available_resources);
    
    pthread_cond_signal(&cond); // Ii dau unlock cel putin unui thread cu variabila cond.
                                // Cand apelezi lock de mai multe ori se poate ajunge la un deadlock.
    pthread_mutex_unlock(&mutex);
    return 0;
}

void * thread_routine(void * arg){
    int index = *(int*)arg;
    int random = rand() % MAX_RESOURCES + 1;
    
    decrease_count(random); //Aici are loc un deadlock. Din cauza ca se intampla 2 LOCK-uri in acelasi timp 
    increase_count(random); //prin functiile drecrease si increase si sunt "oarecum" apelate in ordine diferita. -- Nu pra am inteles conceptul                             

    return NULL;
}

int main(){

    pthread_t threads[NTHREAD];
    int arg[NTHREAD];
    printf("MAX_RESOURCES = %d\n", MAX_RESOURCES);

    if (pthread_mutex_init(&mutex, NULL)){
        perror("Failed at mutex");
        return errno;
    }

    if (pthread_cond_init(&cond, NULL)){
        perror("Failed at condition");
        return errno;
    }

    srand(time(0));
    for (int i = 0; i < MAX_RESOURCES; i++){
        arg[i] = i;
        if(pthread_create(&threads[i], NULL, thread_routine, &arg[i])){//e cam irelevant, nu-l folosesc la nimic
            perror("Failed to create a thread");
            return errno;
        }
    }

    for (int i = 0; i < MAX_RESOURCES; i++){
        if(pthread_join(threads[i], NULL)){
            perror("Can't join a thread");
            return errno;
        }
    }

    pthread_mutex_destroy(&mutex); //cand nu mai avem nevoie de ele le distrugem.
    pthread_cond_destroy(&cond);
}