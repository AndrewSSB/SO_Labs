#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>  
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int arg, char * argv[]){
    char * fisier_sursa = argv[1];
    char * fisier_destinatie = argv[2];

    struct stat stat_buf;
    if (stat(fisier_sursa, &stat_buf) == -1){
        perror("failed at src");
        return errno;
    }

    int fd_src = open(fisier_sursa, O_RDONLY);
    if (fd_src == -1){
        perror("Nu pot deschide fisierul");
        return errno;
    }

    char *buf = (char*)malloc(stat_buf.st_size * sizeof(char) + 1);
    if (buf == NULL){
        perror("Nu pot aloca memorie");
        return errno;
    } 

    size_t bytes_read;

    for (int offset = 0; offset < stat_buf.st_size; offset += bytes_read){
        bytes_read = read(fd_src, buf + offset, stat_buf.st_size - offset);
        if (bytes_read == -1){
            perror("NU pot citi din fisier");
            return errno;
        }
    }

    buf[stat_buf.st_size] = '\0';

    int fd_dst = open(fisier_destinatie, O_WRONLY | O_CREAT, S_IRWXU);
    size_t bytes_write;
    for (int offset = 0; offset < stat_buf.st_size; offset += bytes_write){
        bytes_write = read(fd_dst, buf + offset, stat_buf.st_size - offset);
        if (bytes_write == -1){
            perror("Nu pot scrie in fisier");
            return errno;
        }
    }
    free(buf);
    close(fd_src);
    close(fd_dst);
    return 0;
}