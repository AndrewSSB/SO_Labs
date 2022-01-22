#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

#define SHM_SIZE 4096   // astea sunt macrouri
#define max_string 1000

int main(int argc, char * argv[]){

    const char * shm_name = "SharedFile";   // Numele fisierului de memorie partajata
    
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); // il creez cu ajutorul functiei shm_open (care intoarce un int, daca e < 0 inseamna eroare)
                                                                          // pot scrie in fisier (O_RDWR)
    if (shm_fd < 0){
        return errno;
    }

    int s = ftruncate(shm_fd, SHM_SIZE);    // odata creat fisierul, ii definim dimensiunea cu ajutorul lui ftruncate 
    if (s < 0){                             // la fel, intoarce un int, daca e < 0 inseamna eroare
        return errno;
    }

    char * shm_ptr_rdonly = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);       // Memoria partajata se incarca in spatiul procesului cu ajutorul functiei mmap
                                                                                       // cand se executa cu succes, rezultatul este un pointer catre adresa spatiului la care a fost incarcat obiectul
    if (shm_ptr_rdonly == MAP_FAILED){
        return errno;
    }


    pid_t pid;      
    int ofset = 50;
    for (int i=1; i<argc; i++){ // pt fiecare argument creez un proces
        pid = fork();

        if (pid == 0){
            char * shm_ptr_child = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);    //fac acelasi lucru ca mai sus (parintele va avea acces la ce scrie copilul si invers)
            if (shm_ptr_child == MAP_FAILED){
                return errno;
            }

            int n = strtol(argv[i], NULL, 10);
            char *p = (char*)malloc(max_string * sizeof(char)+1);
            if (p == NULL){
                perror("Nu am putut aloca memorie");
                return errno;
            }

            int offset = sprintf(p, "%d: %d", n, n);       //sprintf scrie in fisierul de memorie partajata a copilului
            while(n > 1) {
                if (n%2)
                    n = 3*n+1;
                else 
                    n = n/2;    
                offset += sprintf(&p[offset], " %d", n);
            }
            
            sprintf(shm_ptr_child + ofset * i, "%s", p);
            munmap(shm_ptr_child, SHM_SIZE);    // munmap dezaloca memoria incarcata in fisier

            printf("Done Parent %d Me %d\n", getppid(), getpid());  // pana aici e cam la fel
            break;
        }

    }

    if (pid > 0){
        printf("Starting parent %d \n", getppid());
        for (int i = 1; i < argc; i++)
            wait(NULL); //astept ca fiecare copil sa termine procesul

        for (int i = 1; i < argc; i++)
            printf("%s\n", shm_ptr_rdonly + i*ofset); // si aici accesez zona de memorie in care a scris fiecare copil si afisez vectorul

        printf("Done parent %d Me %d\n", getppid(), getpid());
    }

    

    munmap(shm_ptr_rdonly, SHM_SIZE);
    shm_unlink(shm_name);
}