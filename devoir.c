#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "generateurtab.c"
#include "hashcode.h"
//#include "freel-liste.h"
#define TAILLE_ALPHABET 4
#define TAILLE_TABLEAU 4000

enum types { F, I, O, R };

void afficher_occurrences(struct Occurrence *occurrences) {
    if (occurrences == NULL){
        printf("Aucun motif trouvé.\n");
            return;
    }
    printf("Occurrences trouvées :\n");
    occurrences = occurrences->next;
    while (occurrences->next != NULL){
        printf("Position : %d, Points : %d, Motif : %s, Motif numéro : %d\n", occurrences->position, occurrences->points, occurrences->motif, occurrences->numero_de_motif);
        occurrences = occurrences->next;
    }
}

void effacer_duplicats(struct Occurrence *head) {
    struct Occurrence *current = head;
    struct Occurrence *prev = NULL;
    struct Occurrence *temp = NULL;

    while (current != NULL) {
        temp = current->next;
        prev = current;

        //on parcours les éléments suivants pour vérifier les doublons
        while (temp != NULL) {
            if (strcmp(current->motif, temp->motif) == 0) {
                // Supprimer le doublon
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            } else {
                //on avance dans la liste
                prev = temp;
                temp = temp->next;
            }
        }

        //ça passe au prochain élément de la liste principale
        current = current->next;
    }
}

void insert_tri(struct Occurrence** head_ref, struct Occurrence* new_node) {
    struct Occurrence* current;
    if (*head_ref == NULL || (*head_ref)->numero_de_motif >= new_node->numero_de_motif) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    } else {
        current = *head_ref;
        while (current->next != NULL && current->next->numero_de_motif < new_node->numero_de_motif) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void trier_occurrences(struct Occurrence **head_ref) {
    struct Occurrence *sorted = NULL;
    struct Occurrence *current = *head_ref;

    while (current != NULL) {
        struct Occurrence *next = current->next;
        current->next = NULL; // Disconnect the node from the original list
        insert_tri(&sorted, current);
        current = next;
    }

    *head_ref = sorted; // Update the head pointer of the original list
}




int len(struct Occurrence *head) {
    int count = 0;
    struct Occurrence *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

#define NBETATS 5
#define NBETATS2 7
#define NBTYPES 4
#define ETATENTREE 0
#define ETATSORTIE 7
#define ETATERREUR -1
int etat_erreur = -1;

// Motif 1 : FI+OI*R+
int ttransit1[NBETATS][TAILLE_ALPHABET] = {
    {1, -1, -1, -1}, // 0
    {-1, 2, -1, -1}, // 1
    {-1, 2, 3, -1}, // 2
    {-1, 3, -1, 4}, // 3
    {7, 7, 7, 4}  // 4
};

// Motif 2 : F+IO+I?O*R+
//  F   I   O   R
int ttransit2[NBETATS2][TAILLE_ALPHABET] = {
    {1,-1,-1,-1}, // 0
    {1,2,-1,-1}, // 1
    {-1,-1,3,-1}, // 2
    {-1,4,3,6}, // 3
    {-1,-1,5,6}, // 4
    {-1,-1,5,6}, // 5
    {7,7,7,6} // 6
};

enum types get_type(char l) {
    switch (l) {
        case 'F': return F;
        case 'I': return I;
        case 'O': return O;
        case 'R': return R;
        default: return -1; // Caractère non reconnu
    }
}
//F = 10, R = 20, I = 3, O = 4
int calculer_points(char c) {
    switch (c) {
        case 'F': return 10;
        case 'R': return 20;
        case 'I': return 3;
        case 'O': return 4;
        default: return 0;
    }
}

//  F I O R
void affiche_ttransit(int tab[NBETATS][NBTYPES]) {
	int i, j;
	printf("ETAT | F | I | O | R\n");
	for (i=0 ; i<NBETATS ; i++) {
		printf("  %2d |  ", i);
		for (j=0 ; j<NBTYPES ; j++) {
			printf("%2d |  ", tab[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void automate(char *tab, int (*ttransit)[TAILLE_ALPHABET], struct Occurrence *occurrences, int numero_de_motif) {
    int nb_occurrences = 0;
    int etat_courant = ETATENTREE;
    int motif_index = 0; //indice pour ajouter des lettres au motif trouvé
    char motif_trouve[1000]; //pour stocker le motif trouvé

    for (int i = 0; tab[i] != '\0'; i++) {
        enum types t = get_type(tab[i]);
        etat_courant = ttransit[etat_courant][t];

        if (etat_courant == ETATERREUR) { //si l'état courant est l'état d'erreur
            etat_courant = ETATENTREE; //on réinitialise l'état courant
            motif_index = 0; //et on efface le contenu du motif trouvé
            memset(motif_trouve, 0, sizeof(motif_trouve)); //ça efface le tableau motif_trouve
        } else if (etat_courant == ETATSORTIE) { //et si l'état courant est l'état final
            //on calcul des points pour l'occurrence trouvée
            int points = 0;
            //for (int k = i - motif_index + 1; k <= i; k++) {
            for (int k = 0; motif_trouve[k] != '\0'; k++) {
                points += calculer_points(motif_trouve[k]);
            }
            //on crée une nouvelle occurrence
            struct Occurrence* newOccurrence = cree_noeud(i - motif_index, points, motif_trouve, numero_de_motif);
            if (occurrences == NULL) { //si la liste des occurrences est vide
                occurrences = newOccurrence; //la nouvelle occurrence devient la première de la liste
            } else {
                struct Occurrence* temp = occurrences;
                while (temp->next != NULL) { //ça trouve le dernier élément de la liste
                    temp = temp->next;
                }
                temp->next = newOccurrence; //puis on ajoute la nouvelle occurrence à la fin de la liste
            }
            nb_occurrences++; //on augmente le nombre d'occurrences trouvées
            etat_courant = ETATENTREE; //on réinitialise l'état courant
            motif_index = 0; //on efface le contenu du motif trouvé
            memset(motif_trouve, 0, sizeof(motif_trouve)); //et on efface le tableau motif_trouve(qui stoque le motif trouvé)
        } else {
            //on ajoute la lettre courante au motif trouvé
            motif_trouve[motif_index++] = tab[i];
        }
        //printf("i = %d\n", i);
    }
    return;
}

int max_hash_value(struct Occurrence *occurrences) {
    int max_hash = -1;
    //pour trouver le hash maximum
    while (occurrences != NULL) {
        int hash = compute_hash(occurrences->motif);
        if (hash > max_hash) {
            max_hash = hash;
        }
        occurrences = occurrences->next;
    }
    return max_hash;
}

void hashage(struct Occurrence *occurrences_premier, struct Occurrence *occurrences){
    //déjà on trie les occurrences avant de les ajouter à la table de hachage
    trier_occurrences(&occurrences);
    //printf("debug: Occurrences triées\n");
    struct Occurrence *current = occurrences_premier; //ça c'est un pointeur de parcours initialisé au premier élémentrs le premier élément
    int hash = max_hash_value(current);
    //printf("debug: Valeur max hash ///////////////////////////////////////////// : %d\n", hash);


    //afficher_occurrences(occurrences);
    init_tabhash(hash);
    //printf("debug: Table de hachage initialisée valeur max hash : %d \n", hash);
    current = occurrences_premier;
    //printf("debug: Affichage des occurrences\n");
    
    while (current != NULL) {
        add_nom(current->motif); // Ajouter l'occurrence à la table de hachage
        current = current->next;
    }
    print_tabhash(hash+1);
    //etape 7
    float moyenne = 0;
    int count = 0;
    current = occurrences_premier; //repart du debut de la liste d'occurence
    while (current != NULL) {
        moyenne += current->points;
        count++;
        current = current->next;
    }
    moyenne = moyenne / count-1;
    printf("Moyenne des points : %.2f\n", moyenne);
}

//les algorithmes
//Algorithme pour remplir un ensemble de points différents
void algo1(struct Occurrence *occurrences) {
    bool *points_utilises = (bool *)calloc(100, sizeof(bool)); //allouer dynamiquement le tableau
    if (points_utilises == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(0);
    }

    int nb_total_points = 0;
    int nb_comparaisons = 0; //on ajoute un compteur de comparaisons

    occurrences = occurrences->next;
    struct Occurrence *current = occurrences;
    printf("Ensemble de points différents trouvés :\n");
    while (current != NULL) {
        int points = 0;
        for (int i = 0; current->motif[i] != '\0'; i++) {
            points += calculer_points(current->motif[i]);
        }
        if (!points_utilises[points]) {
            points_utilises[points] = true;
            nb_total_points++;
            printf("%d ", points); // Afficher les points pour l'algorithme 1
        }
        current = current->next;
        nb_comparaisons++; //et on incrémente le compteur de comparaisons
    }
    printf("\n");
    printf("Algorithme 1 :\n");
    printf("Nombre total de nombres de points différents trouvés : %d\n", nb_total_points);
    printf("Nombre de comparaisons effectuées : %d\n", nb_comparaisons);

    free(points_utilises); //libère la mémoire allouée dynamiquement
}

//structure pour l'algorithme 2
//ça c'est une structure simple pour les éléments de la liste chaînée
struct Node {
    int points;
    struct Node *next;
};

//Algorithme pour compter le nombre total de nombres de points différents
void algo2(struct Occurrence *occurrences) {
    struct Node *head = NULL; //on initialise : la liste chainée
    int nb_total_points = 0; //le compteur de nombres de points différents
    int nb_comparaisons = 0; //le compteur de comparaisons
    int somme_points = 0; //et la somme totale des points

    //ça parcours des occurrences
    occurrences = occurrences->next;
    struct Occurrence *current = occurrences;
    while (current != NULL) {
        int points = 0;
        //et calcul le nombre total de points pour chaque occurrence
        for (int i = 0; current->motif[i] != '\0'; i++) {
            points += calculer_points(current->motif[i]);
        }

        //puis on fait une vérification si le nombre de points est déjà dans la liste
        struct Node *temp = head;
        bool trouve = false;
        while (temp != NULL) {
            nb_comparaisons++; //on incrémente le compteur de comparaisons
            if (temp->points == points) {
                trouve = true;
                break;
            }
            temp = temp->next;
        }

        //si le nombre de points n'est pas dans la liste, l'ajouter
        if (!trouve) {
            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            if (newNode == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire\n");
                exit(EXIT_FAILURE);
            }
            newNode->points = points;
            newNode->next = head;
            head = newNode;
            nb_total_points++;
            somme_points += points; //ça joute le nombre de points à la somme totale
        }

        current = current->next; //et on passe à l'occurrence suivante
    }
    
    //fait la somme des points et calcule la moyenne
    float moyenne_points = (float)somme_points / nb_total_points-1;

    //affichage du nombre total de nombres de points différents trouvés
    printf("Algorithme 2 :\n");
    printf("Nombre total de nombres de points différents trouvés : %d\n", nb_total_points);
    printf("Nombre de comparaisons effectuées : %d\n", nb_comparaisons);
    printf("Nombre de points moyen : %.2f\n", moyenne_points); // Afficher le nombre de points moyen

    //on fini en libérant la mémoire allouée pour la liste chaînée
    struct Node *temp = head;
    while (temp != NULL) {
        struct Node *tempNext = temp->next;
        free(temp);
        temp = tempNext;
    }
}



int main() {
    char tableau[TAILLE_TABLEAU];
    //struct Occurrence occurrences[TAILLE_TABLEAU];
    struct Occurrence *occurrences = (struct Occurrence*) malloc(sizeof(struct Occurrence));
    if(occurrences == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(0);
    }
    struct Occurrence *occurrences_premier = occurrences;
    //ça parcours des occurrences
    struct Occurrence *affiche = occurrences;
    printf("Alphabet : F, I, O, R\n");
    printf("Motif 1 : FI+OI*R+\n");
    printf("Motif 2 : F+IO+I?O*R+\n");
    // Remplir le tableau avec des lettres aléatoires
    genere(tableau);
    printf("Tableau généré : %s\n", tableau); //pour afficher le tableau généré
    //appele la fonction automate pour le motif 1
    automate(tableau, ttransit1, occurrences, 1);
    //appele la fonction automate pour le motif 2
    automate(tableau, ttransit2, occurrences, 2);
    int taille = len(occurrences);
    //printf("Nombre total d'occurrences trouvées avant suppression des doublons : %d\n", taille);
    //afficher_occurrences(occurrences);
    effacer_duplicats(occurrences);  
    taille = len(occurrences);
    printf("Nombre total d'occurrences trouvées après suppression des doublons : %d\n", taille);
    afficher_occurrences(affiche);

    hashage(occurrences_premier, occurrences); //pour remplire la table de hash
    //appel des algorithmes
    algo1(occurrences);
    algo2(occurrences);
    return 0;
}
