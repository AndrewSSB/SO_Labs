#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>  
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int arg, char * argv[]){
    size_t bytes_HELLO;
    bytes_HELLO = write(STDOUT_FILENO, "Hello, ", strlen("Hello, "));
    if (bytes_HELLO == -1){
        perror("Eroare la hello");
        return errno;
    }

    size_t bytes_write;
    /*for (int offset = 0; offset < strlen(argv[1]); offset += bytes_write){ // al 2lea parametru va fi sirul pe care-l vom afisa
        bytes_write = write(STDOUT_FILENO, argv[1] + offset, strlen(argv[1]) - offset);
        if (bytes_write == -1){
            perror("Eroare de scriere");
            return errno;
        }
    } */

    bytes_write = write(STDOUT_FILENO, argv[1], strlen(argv[1]));
    if (bytes_write == -1){
        perror("Eroare la citirea argumentului");
        return errno;
    }

    return 0;
}