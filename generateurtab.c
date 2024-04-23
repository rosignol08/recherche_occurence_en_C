#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_ALPHABET 4
#define TAILLE_TABLEAU 4000

void genere(char* tableau) {
    char alphabet[] = {'F', 'R', 'I', 'O'};
    int i;
    //pour avoir des valeurs aléatoires
    srand(time(NULL));
    //pour remplir le tableau avec des lettres aléatoires de l'alphabet
    for (i = 0; i < TAILLE_TABLEAU; i++) {
        tableau[i] = alphabet[rand() % TAILLE_ALPHABET];
    }
}
