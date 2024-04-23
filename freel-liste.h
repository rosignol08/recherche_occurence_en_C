/************************************************************
*
*   Ce fichier contient les declarations necessaires pour
*    utiliser la librairie list
*
*   F. Balmas 2023
*
*************************************************************/

struct linked {
    char nom[21]; //le nom, max 20 char
    struct linked *cell; //pointeur sur le chainon suivant
};

//ma liste chainée 
struct Occurrence {
    int position;
    int points;
    char motif[1000];
    int numero_de_motif;
    struct Occurrence *next; 
};

// creation du type LINKED pour simplifier les declarations
typedef struct linked *LINKED;

//creation du type Occurrence pour simplifier les declarations
struct Occurrence* cree_noeud(int position, int points, char *motif, int numero_de_motif);
//ajoute un noeud avec un motif
struct Occurrence* ajoute_mot_noeud(char *motif, struct Occurrence *occurrences);

// alloue un chainon
//LINKED alloc_list(void);
//nouvelle version de alloc_list
struct Occurrence* alloc_list(char *motif, struct Occurrence *occurrences);
// initialise la liste
void init_list(struct Occurrence* p);
// initialise la freeliste
void init_freeliste(void);
// remplir la freelistevoid remplir_freeliste(void);
void remplir_freeliste(void);

// ajoute un chainon au debut de la liste
void add_begin (struct Occurrence* t, struct Occurrence* l);
// ajoute a la fin
void add_end (struct Occurrence* t, struct Occurrence* l);
// enleve au debut
struct Occurrence* remove_begin (struct Occurrence* t);
// enleve a la fin
struct Occurrence* remove_end (struct Occurrence* t);

// affiche le contenu d'une liste
int print_list (struct Occurrence* l);
// affiche un chainon
void print_nom(struct Occurrence* p);

// teste si un elt appartient a une liste
struct Occurrence* member (char *x, struct Occurrence* l);

// calcule la longueur de la liste
int length(struct Occurrence* l);
