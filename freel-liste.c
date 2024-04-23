#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "freel-liste.h"

#define NL 20
// Tête de la freeliste
static struct Occurrence TETE;
static struct Occurrence *T;

//fonction primaires pour definir ma liste chainée
struct Occurrence* cree_noeud(int position, int points, char *motif, int numero_de_motif) {
    struct Occurrence* newOcc = (struct Occurrence*) malloc(sizeof(struct Occurrence));
    if(newOcc == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(0);
    }
    newOcc->position = position;
    newOcc->points = points;
    strcpy(newOcc->motif, motif);
    newOcc->next = NULL;
    newOcc->numero_de_motif = numero_de_motif;
    return newOcc;
}

struct Occurrence* ajoute_mot_noeud(char *motif, struct Occurrence *occurrences) {
    struct Occurrence *newOcc = (struct Occurrence*) malloc(sizeof(struct Occurrence));
    if(newOcc == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(0);
    }
    strcpy(newOcc->motif, motif);
    newOcc->next = occurrences;
    return newOcc;
}


//initialise le pointeur T sur TETE (T est la tête de la freeliste)
void init_freeliste(void)
{
    T = &TETE;
}

//ça alloue un nouveau noeud et l'ajoute à la tête de la freeliste
struct Occurrence* alloc_list(char *motif, struct Occurrence *occurrences) {
    //créer un nouveau noeud avec le motif
    struct Occurrence *newNode = ajoute_mot_noeud(motif, occurrences);
    //initialiser le noeud
    init_list(newNode);
    //ajoute le noeud à la tête de la freeliste
    add_begin(T, newNode);
    return newNode;
}

//initialise aussi le noeud
void init_list(struct Occurrence *node)
{
    node->next = NULL;
}

//ajouter un noeud au début de la liste
void add_begin(struct Occurrence *list, struct Occurrence *node)
{
    node->next = list->next;
    list->next = node;
}

//ajouter un noeud à la fin de la liste
void add_end(struct Occurrence *list, struct Occurrence *node)
{
    struct Occurrence *temp = list;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = node;
}

//ça supprime le premier noeud de la liste et le renvoye
struct Occurrence* remove_begin(struct Occurrence *list)
{
    if (list->next == NULL)
    {
        fprintf(stderr, "Liste vide, suppression impossible\n");
        return NULL;
    }
    struct Occurrence *removedNode = list->next;
    list->next = removedNode->next;
    removedNode->next = NULL;
    return removedNode;
}

//ça supprime le dernier noeud de la liste et le renvoye
struct Occurrence* remove_end(struct Occurrence *list)
{
    if (list->next == NULL)
    {
        fprintf(stderr, "Liste vide, suppression impossible\n");
        return NULL;
    }
    struct Occurrence *temp = list;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    struct Occurrence *removedNode = temp->next;
    temp->next = NULL;
    return removedNode;
}

// Afficher le contenu de la liste (pas besoin de cette fonction)
/*
void print_list(struct Occurrence *list){
    struct Occurrence *temp = list->next;
    while (temp != NULL){
        print_nom(temp);
        temp = temp->next;
    }
    printf("\n");
}
*/

int print_list(struct Occurrence *list) {
    struct Occurrence *temp = list;
    while (temp != NULL) {
        print_nom(temp);
        temp = temp->next;
    }
    printf("\n");
    return 1;
}


//affiche le contenu d'un noeud
void print_nom(struct Occurrence *node)
{
    printf("%s, ", node->motif);
}

//vérifie si un motif est présent dans la liste
struct Occurrence* member(char *motif, struct Occurrence *list)
{
    struct Occurrence *temp = list->next;
    while (temp != NULL)
    {
        if (strcmp(temp->motif, motif) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

//calcule la longueur de la liste
int length(struct Occurrence *list)
{
    int len = 0;
    struct Occurrence *temp = list->next;
    while (temp != NULL)
    {
        len++;
        temp = temp->next;
    }
    return len;
}

//remplit la freeliste avec des noeuds
void remplir_freeliste(void){
    printf("debug: ça marche ici\n");
    struct Occurrence *m, *pm, *pf;
    #ifdef DEBUG
        printf("Remplissage de la freeliste ...\n");
    #endif
    m = (struct Occurrence*)malloc(NL * sizeof(struct Occurrence));
    if (m == NULL)
    {
        printf("Plus de memoire disponible ...\n");
        exit(1);
    }
    pf = T;
    for (pm = m; pm < m + NL; pm++)
    {
        pf->next = pm;
        pf = pf->next;
    }
    pf->next = NULL;
    #ifdef DEBUG
        printf("Longueur de la FREELIST : %d\n", length(T));
    #endif
}
