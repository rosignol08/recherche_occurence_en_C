/* ***************************************************************
*
*  Hashcoding
*
*  F. Balmas 2021
*
******************************************************************/
// valeur standard
// #define N 53
// valeur de demo
#define N 600
#include "freel-liste.h"



//cree un noeud
struct Occurrence* cree_noeud(int position, int points, char *motif, int numero_de_motif);
//ajoute un noeud avec un motif
struct Occurrence* ajoute_mot_noeud(char *motif, struct Occurrence *occurrences);

extern LINKED tabhash[N];
extern struct Occurrence *tabhash_occ[N];
//cherche un nom dans la table de hachage
struct Occurrence* search_nom(char *nom);
//ajoute un nom dans la table de hachage
void add_nom(char *s);
//initialise la table de hachage
void init_tabhash(int taille);
//affiche la table de hachage
void print_tabhash(int taille);
//calcule le hash d'un mot
int compute_hash(char *word);
//nouvelle vertion de add_begin pour ma version de liste chainée
void add_begin_occ(struct Occurrence **t, struct Occurrence *l);

