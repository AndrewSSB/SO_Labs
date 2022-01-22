#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_oucurrences(const char* text, const char* word){
    // Am presupus ca textul este scris corect.
    //char aux[strlen(text)+1], *p;
    char *p;
    char *aux = (char*)malloc((strlen(text)+1)*sizeof(char));
    strcpy(aux, text);
    int counter = 0;
    p = strtok(aux, "., :;");
    while(p != NULL){
        if (strcmp(p, word) == 0)
            ++counter;
        p = strtok(NULL, "., :;");
    }
    free(aux);
    return counter;
}

char *swap_words(const char * givenWord, const char* toChangeWord, const char * text){ //diferenta in modul dintre cuvantul_nou si cuvantul_vechi
    int appear = count_oucurrences(text, toChangeWord);
    char * text_copy = (char*)malloc((strlen(text)+(appear*(strlen(givenWord)-strlen(toChangeWord))))*sizeof(char));
    int oldWordL = strlen(toChangeWord);
    int newWordL = strlen(givenWord);
    int i = 0;
    while (*text){
        if (strstr(text, toChangeWord) == text){
            strcpy(&text_copy[i], givenWord);
            i += newWordL;
            text += oldWordL;
        }
        else
            text_copy[i++] = *text++;
    }
    return text_copy;
    free(text_copy);
}

int main(){

    /*char text[] = "Ana are mere. Ana mananca mere.";
    char cuvant[] = "mere";
    char cuvant_nou[] = "Maria";
    */
    char *text = (char*)malloc(sizeof(char)*200);
    printf("Textul: \n");
    fgets(text, 200, stdin);
    char *cuvant = (char*)malloc(sizeof(char)*30);
    printf("Cuvantul: \n");
    scanf("%s", cuvant);
    //ex2
    printf("%d", count_oucurrences(text, cuvant));
    //ex3
    char *cuvant_nou = (char*)malloc(sizeof(char)*30);
    printf("\nCuvantul nou: \n");
    scanf("%s", cuvant_nou);
    printf("%s", swap_words(cuvant_nou, cuvant, text));
    return 0;
}