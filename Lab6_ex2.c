#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int matrice1[100][100]; //Definesc matricele
int matrice2[100][100];
int result[100][100];
int n1, m1; //Definesc indicii pentru matricea 1
int n2, m2; //pt matricea 2

typedef struct parameters { //definesc o structura care primeste ca parametru indicii unei matrice
    int x,y;
}args;

void * multiplication(void * arg){ // functia de multiplicare

    args *p = arg;

    for (int i=0; i<m1; i++){
        result[p->x][p->y] += matrice1[p->x][i] * matrice2[i][p->y];
    }
}

int main(){
    printf("Nr de lini si coloane pt prima matrice:\n"); //citirea primei matrice
    scanf(" %d", &n1);
    scanf(" %d", &m1);

    printf("Introduceti elem pt prima matrice: \n");
    for (int i=0; i<n1; i++){
        for (int j=0; j < m1; j++){
            printf("matrice1[%d][%d]: ", i, j);
            scanf(" %d", &matrice1[i][j]);
        }
    }

    printf("Nr de lini si coloane pt a 2-a matrice:\n"); //citirea celei de a 2-a matrice
    scanf(" %d %d", &n2, &m2);

    printf("Introduceti elem pt a 2-a matrice: \n");
    for (int i=0; i<n2; i++){
        for (int j=0; j < m2; j++){
            printf("matrice2[%d][%d]: ", i, j);
            scanf(" %d", &matrice2[i][j]);
        }
    }
    
    if (m1 == n2){ //conditia de inmultire a 2 matrice

        pthread_t threads[m1*n2]; //m1*n2 elemente -> exact atatea threaduri

        int thread_nr = 0;

        args p[m1*n2]; //fac un vector de argumente

        for (int i=0; i<m1;i++){
            for (int j = 0; j <n2; j++){
                p[thread_nr].x = i;
                p[thread_nr].y = j;

                int status;

                status = pthread_create(&threads[thread_nr], NULL, multiplication, (void*)&p[thread_nr]); //aplic functia pt fiecare elem al matricei
                if (status != 0){
                    printf("Eroare");
                    return errno;
                }
            
            thread_nr++;
            }
        }

        for (int i=0; i<(m1, n2); i++)
            pthread_join(threads[i], NULL); //fac join la functie, dar nu vreau sa atribui nicio valoare

        printf("\n");
        for (int i=0; i<m1; i++){ // afisez rezultatul inmultirii celor 2 matrice
            for (int j =0; j<n2; j++){
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("Nu se pot inmultii cele 2 matrice\n");
    }
}