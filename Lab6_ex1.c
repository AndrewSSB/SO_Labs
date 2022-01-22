#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void * inverted_string(void *v){
    char *first = (char*)v; // am copiat string-ul primit ca parametru (nu era necesar)
    int  length = strlen(first);
    char *second = (char*)malloc(length * sizeof(char) + 1);

    if (second == NULL){
        printf("Memory not allocated. \n");
        return errno;
    }

    int i = 0;

    for (i=0; i<strlen(first); i++){    //aici am inversat literele 
        second[i] = first[length-1];
        length--;
    }
    second[i+1] = '\0';
    return second;
}

int main(int argc, char * argv[]){

    pthread_t thread;
    if (pthread_create(&thread, NULL, inverted_string, argv[1])){
        perror(NULL);
        return errno;
    }

    char * result; // nu am mai incercat sa fac void * 
    if (pthread_join(thread, &result)){ // restul = inverted_string(); adica sirutl inversat
            perror(NULL);
            return errno;
        }

    printf("%s", result); //si sa convertesc aici la tip char (bataie de cap)
    free(result);
}