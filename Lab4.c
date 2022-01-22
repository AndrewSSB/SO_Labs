#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    //ex1
    // pid_t pid = fork();         // creez un proces nou

    // if (pid < 0)
    //     return errno;
    // else if (pid == 0){
    //     const char * path = "/usr/bin/ls";   // ca sa folosesti functia execve ai nevoie de calea absoluta
                                                // o poti afla cu which pwd/ls/etc (ce comenzi vrei sa execute)
                                                // pentru ex1 am scris which ls
         
    //    char * args[] = {"ls", NULL};         // aici ii dai "functia" pe care sa o execute, NULL se pune in caz de eroare, sa nu faca nimic

    //    execve(path, args, NULL);             // suprascrie complet procesul apelant cu un nou proces (de aceea de regula se foloseste cu fork()), conform programului gasit la calea indicata in path.
    // }                                        // dupa rularea lui execve nu se mai revine in programul initial decat daca returneaza o eroare
    // else{
    //     printf("My PID=%d, Child PID=%d\n", getppid(), getpid());    // mai intai afisez ce e in parinte (parintele se executa primul)
    //     wait(NULL);        // aici ii dau wait ca sa afiseze si ce e in copil imediat dupa parinte, altfel il afiseaza aiurea
    // }

    //ex2

    // pid_t pid = fork();      // am creat un proces nou

    // char *p;                 // argv este un vector de char
    // errno = 0;
    // int n = strtol(argv[1], &p, 10); // asa ca am convertit argumentul din char in int ca sa ma pot folosi de el
                                        // argv[0] -> numele programului, argv[1] -> primul argument, argv[2] -> al doilea si tot asa
                                        // de ex ./Lab4 10 (Lab4 e primul argument si 10 e al doilea)
    // if (pid < 0)
    //     return errno;
    // else if (pid == 0){              
    //     while(n > 1){
    //         printf("%d ", n);
    //         if (n%2)
    //             n = 3*n+1;
    //         else
    //             n = n/2;
    //     }
    //     printf("\nChild %d finished\n", getpid());
    // }
    // else{
    //     printf("%d:", n);            // parinte apoi copil asta e ordinea
    // }

    pid_t pid;

    for(int i=1; i< argc; i++){         // argc reprezinta numarul de argumente (e calculat automat)
                                        // am pus incrementarea de la 1 pentru ca este luat in considerare si argv[0] (numele programul)
        pid = fork();      // creez un proces nou pt fiecare argument dat

        if (pid == 0){     // in copil calculez ipoteza lui Collantz
            char * p;
            int n = strtol(argv[i], &p, 10);    // exact ca in exercitiul de mai devreme
            printf("%d: ", n);  // Dupa ce astepti, se afiseaza tot ce e aici pana la break;

            while(n != 1) {
                printf("%d ", n);
                if (n%2)
                    n = 3*n+1;
                else 
                    n = n/2;    
            }
            printf("\nDone Parent %d Me %d\n", getppid(), getpid()); 
            break;
        }
    }
    if (pid > 0){
        printf("Starting parent %d\n", getppid());  // Afisez mai intai linia asta
        for (int i=1; i<argc; i++)                  // Apoi astept ca fiecare copil sa termine de calculat
            wait(NULL);
    }
}