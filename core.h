#ifndef __CORE__
#define __CORE__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h> 
#include <limits.h>


#define TAILLE_MAX 1300
/*
#define LARGEUR 		seam.largeur
#define LARGEURSortie 	seam.largeurSortie
#define HAUTEUR 		seam.hauteur
#define HAUTEURSortie 	seam.hauteurSortie
*/
#define imageRVB_ 				seam->imageRVB
#define imageRVBtransposee_ 	seam->imageRVBtransposee
#define imageNB_				seam->imageNB
#define imageNBtransposee_		seam->imageNBtransposee
#define HAUTEUR_				seam->hauteur
#define LARGEUR_				seam->largeur
#define I_						seam->I
#define T_						seam->T

//16.50

typedef enum { FALSE, TRUE } bool;
typedef struct {
	int largeur;
	int hauteur;
	int largeurSortie;
	int hauteurSortie;
	bool affichage;
	int** I;
	int** T;
	unsigned char** imageNB;;;;;
	unsigned char** imageNBtransposee;;;
/*
	unsigned char** imageNBtransposee;
*/
	unsigned char***imageRVB;
	unsigned char***imageRVBtransposee;
	
	FILE* fInput;
	FILE* fOutput;
	
	
} seam_t;

//Permet de calculer T[x][y], pour un couple (x, y) donné. (récursivement)
int compteChemin(int** T, int** I, int x, int y, int LARGEUR);

//Retourne le plus petit père de tab[x]
int parcoursChemin(int tab[TAILLE_MAX], int x, int LARGEUR);

//Permet de trouver l'entier minimum dans un tableau, et renvoie son indice.
int min(int *tab, int taille);

//Permet de supprimer le x-ième élément d'un tableau.
void pop(void* tab, int x, int taille, size_t s);

//Permet de supprimer le x-ième élément d'un tableau. Mais cet élément est un tableau lui aussi.
void pop2D(void* tab, int x, int taille, size_t s);

//Cette fonction va dupliquer le x-ième élement du tableau.
void push(void* tab, int x, int taille, size_t s);

//Cette fonction va dupliquer la x-ième ligne du tableau.
void push2D(void* tab, int x, int taille, size_t s);

//Remplis le tableau I
void remplissageI(int** I, unsigned char** imageNB, int largeur, int hauteur);

//Remplis la tableau T
void remplissageT(int** I, int** T, int largeur, int hauteur);

//Ajoute des colonnes.
void pushColonnes(seam_t* seam, int repetition);

//Supprime des colonnes 
void popColonnes(seam_t* seam, int repetition);

//Remplace l'image originale par sa transposee, en RVB et NB. Inverse hauteur et largeur.
void transpose(seam_t* seam);

//Creer la transposee de l'imageRVB et de l'imageNB
void creerTransposee(seam_t* seam);

//Alloue un tableau en 3 dimensions
void* alloueTab3D(size_t s);

//Alloue un tableau en 2 dimensions
void* alloueTab2D(size_t s);

#endif



/*	*
 * 
 * 
 * Code qui permet d'afficher le tableau I
 * 
 * 
 * 

		int max = 0, min = 50000;
		for (i=1; i<=seam.hauteur; i+=1){ 
			for (j=1; j<=seam.largeur; j+=1){
				max = (seam.I[i][j] > max) ? seam.I[i][j] : max;
				min = (seam.I[i][j] < min) ? seam.I[i][j] : min;
			}	
		}
		
		for (i=1; i<=seam.hauteur; i++){ //On ecrit dans le fichier de sortie.
			for (j=1; j<=seam.largeur; j++){
				fputc((unsigned char) (((double)seam.I[i][j]/max)*255), seam.fOutput);
				//printf("%d ",(unsigned char) (((double)seam.I[i][j]/max)*255));

			}
		}

	
	*
	* 
	* 
	* Code qui permet d'afficher le tableau T
	* 
	* 
	* 
	* */
