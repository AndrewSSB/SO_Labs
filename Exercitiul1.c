 int main(){

    //ex1
    int n;
    printf("Dimensiunea matricei patratice: ");
    scanf(" %d", &n);
    
    int ** matrice = (int**)malloc(n*sizeof(int*));
    for (int i=0; i<n; i++){
        matrice[i] = (int*)malloc(n*sizeof(int));
    }

    for (int i = 0; i<n; i++)
        for (int j=0; j<n; j++)
            scanf(" %d", &matrice[i][j]);

    if (n%2)
        printf("Intersectia diagonalelor: %d", matrice[n/2][n/2]);
    else
        printf("\nNu se poate afisa intersectia diagonalelor"); 

    for (int i=0; i<n; i++)
            printf("\nDiag princip: %d", matrice[i][i]);
    for (int i=0; i<n; i++)
            printf("\nDiag secund: %d", matrice[i][n-i-1]);

    for (int i=0; i<n; i++)
        free(matrice[i]);
    free(matrice);
    return 0;
}