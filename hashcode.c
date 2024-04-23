/* ***************************************************************
*
*  Hashcoding
*
*  F. Balmas 2021
*
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "freel-liste.h"
#include "hashcode.h"

//#define NODEBUG
//#define SIZE BUFSIZ

LINKED tabhash[N];
struct Occurrence *tabhash_occ[N];

void init_tabhash(int taille) {
    int i;
    for (i = 0; i < taille; i++) {
        tabhash_occ[i] = NULL; // Initialiser chaque pointeur à NULL testtttttttttttttttttttt
    }
}

int compute_hash (char *word) {
    int i, nb;
    nb = 0;
    for (i=0 ; word[i] != '\0' ; i++)
       nb += word[i];
    return (nb % N);
}

// retourne pointeur sur chaînon si trouve
// NULL sinon
struct Occurrence* search_nom (char *nom) {
    int hash;

	hash = compute_hash (nom);
	return (member(nom, tabhash_occ[hash]));
}

void add_begin_occ(struct Occurrence **t, struct Occurrence *l) {
    l->next = *t; //ici on ajoute l'élément l au début de la liste t
    *t = l; //là l devient le premier élément de la liste
}

void add_nom(char *nom) {
    int hash;
    struct Occurrence *new;
	hash = compute_hash (nom);
	new = ajoute_mot_noeud(nom, tabhash_occ[hash]);
	//printf("debug: ajout de %s dans la table de hachage à la position : %d \n", nom, hash);
	tabhash_occ[hash] = new;
}


void print_tabhash(int taille) {
    int i;
    //printf("debug: affichage de la table de hachage\n");
    for (i = 1; i < taille; i++) {
        if (tabhash_occ[i] != NULL) {  //on verifie si on peut ajouter quelque chose
            printf("%d : ", i);
            print_list(tabhash_occ[i]);
        }
    }
}
