#include "core.h"


#define VERTICAL 0
#define HORIZONTAL 1




int main(int argc, char **argv){
	seam_t seam;

	int i, j, k;
	char* nom;
	
	//On récupere les arguments pris en entrée par le programme.
	nom = argv[1];
	seam.largeurSortie = (int)strtol(argv[2], NULL, 10);
	seam.hauteurSortie = (int)strtol(argv[3], NULL, 10);
	seam.affichage = FALSE; //On désactive l'affichage, il ne marche pas bien. (int)strtol(argv[4], NULL, 10);  
	
	 //Permet de calculer le temps d'execution entre les deux phases du programme.
    clock_t t1, t2;
    t1 = clock();
    
	//On ouvre le fichier.
	seam.fInput = fopen(nom,"r");
	if (seam.fInput == NULL) return EXIT_FAILURE;
	seam.fOutput = fopen("input_resize.ppm","w");
	fscanf(seam.fInput, "P6\n");
	fscanf(seam.fInput, "%d %d\n", &seam.largeur, &seam.hauteur);
	fscanf(seam.fInput, "255\n");
	fprintf(seam.fOutput, "P6\n");
	fprintf(seam.fOutput, "%d %d\n", seam.affichage ? seam.largeur : seam.largeurSortie,
									 seam.affichage ? seam.hauteur : seam.hauteurSortie);
	fprintf(seam.fOutput, "255\n");
	
	seam.I = alloueTab2D(sizeof(int));
	seam.T = alloueTab2D(sizeof(int));
	seam.imageNB = alloueTab2D(sizeof(char));  //Va contenir l'image en N&B.
	seam.imageRVBtransposee	= alloueTab3D(sizeof(char));  //Transposée de l'image en couleur.
	seam.imageNBtransposee	= alloueTab2D(sizeof(char));  //Transposée de l'image en N&B.
	seam.imageRVB = alloueTab3D(sizeof(char));	//Tableau 3D
	
	//On copie l'image dans le tableau imageRVB, et on copie la version Noire et Blanche dans le tableau imageNB.
	for (i=1; i<=seam.hauteur; i+=1){ 
		for (j=1; j<=seam.largeur; j+=1){
			for (k=0; k < 3; k++)
				seam.imageRVB[i][j][k] = fgetc(seam.fInput);
			seam.imageNB[i][j] = (2*seam.imageRVB[i][j][0] + 7*seam.imageRVB[i][j][1] + seam.imageRVB[i][j][2])/10;
		}
	}
	//Fin de copie
	
	
	
	//Affichage d'informations diverses.
	printf("Nom du fichier : %s\n(Largeur, Hauteur) : (%d, %d)\n(LargeurSortie, HauteurSortie) : (%d, %d)\nAffichage ? %s\n" , nom, seam.largeur, seam.hauteur, seam.largeurSortie, seam.hauteurSortie, seam.affichage ? "Oui" : "Non");
	t2 = clock();
	printf("Image Chargée en %.1lf s.\nTraitement de l'image en cours ...\n", (float)(t2-t1)/CLOCKS_PER_SEC);
	
	
	if (seam.affichage){
		char s1[2];
		int s2;
		int** tabSelect;
		printf("Voulez voir I ou T ? "); scanf("%1s", s1);
		printf("Combien de chemins voulez vous voir ? "); scanf("%d", &s2);
		if(s1[0]=='T')
			tabSelect=seam.T;
		else
			tabSelect=seam.I;
			
			
		remplissageI(seam.I, seam.imageNB, seam.largeur, seam.hauteur);
		remplissageT(seam.I, seam.T, seam.largeur, seam.hauteur);
		
		int max = 0, min = 50000;
		for (i=1; i<=seam.hauteur; i+=1){ 
			for (j=1; j<=seam.largeur; j+=1){
				max = (tabSelect[i][j] > max) ? tabSelect[i][j] : max;
				min = (tabSelect[i][j] < min) ? tabSelect[i][j] : min;
			}	
		}
		
		for (i=1; i<=seam.hauteur; i++){ //On ecrit dans le fichier de sortie.
			for (j=1; j<=seam.largeur; j++){
				for (k=0; k<3; k++)
					seam.imageRVB[i][j][k] = (unsigned char) (((double)tabSelect[i][j]/max)*255);
			}
		}	
		popColonnes(&seam, s2);
		creerTransposee(&seam);
		transpose(&seam);
		
		
		
		goto ecriture;
		
		
	}
	
	
	
	
	int differenceLargeur = seam.largeur - seam.largeurSortie;
	int differenceHauteur = seam.hauteur - seam.hauteurSortie;
	
		
	//On supprime des colonnes ou on en ajoute.
	if (differenceLargeur > 0)
		popColonnes(&seam, differenceLargeur);
	else if (differenceLargeur < 0)
		pushColonnes(&seam, (-1)*differenceLargeur);


	creerTransposee(&seam);
	transpose(&seam);
	

	//On supprime des lignes ou on en ajoute.
	if (differenceHauteur > 0)
		popColonnes(&seam, differenceHauteur);
	else if (differenceHauteur < 0)
		pushColonnes(&seam, (-1)*differenceHauteur);
	
		
	
		
	
ecriture:
	printf("Traitement effectué en %.1lf s.\n%d:%d   %d:%d\n", (float)(clock()-t2)/CLOCKS_PER_SEC, seam.largeur, seam.hauteur, seam.largeurSortie, seam.hauteurSortie);
	for (i=1; i<=seam.largeur; i++){ //On ecrit dans le fichier de sortie.
		for (j=1; j<=seam.hauteur; j++){
			for (k=0; k < 3; k++)
				fputc(seam.imageRVB[j][i][k], seam.fOutput);
		}
	}
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


void creerTransposee(seam_t* seam){
	int i, j;
	for (i=0 ; i<LARGEUR_ ; i++){
		for (j=0 ; j<HAUTEUR_ ; j++){
			imageRVBtransposee_[i][j]	= imageRVB_[j][i];
			imageNBtransposee_[i][j]	= imageNB_[j][i];
		}
	}
}
void transpose(seam_t* seam){
	int tmp;
	void* tmp_p;
	
	//Inversion de la dimension de l'image
	tmp  = LARGEUR_;
	LARGEUR_ = HAUTEUR_;
	HAUTEUR_ = tmp;
	
	//Inversion de l'originale et de la transposee en RVB
	tmp_p = imageRVB_;
	imageRVB_ = imageRVBtransposee_;
	imageRVBtransposee_ = tmp_p;
	
	//Inversion de l'originale et de la transposee en NB
	tmp_p = imageNB_;
	imageNB_ = imageNBtransposee_;
	imageNBtransposee_ = tmp_p;
	
}


void popColonnes(seam_t* seam, int repetition){
		//OPTIMISATION
	//imageNB_[i] correspond a *(seam+imageNB+i)   tandis que
	//imageNB[i] correspond a *(imageNB+i)   où imageNB et ma variable locale
/*
	unsigned char** imageNB  = imageNB_;
	unsigned char*** imageRVB = imageRVB_;
	int** I = I_;
	int** T = T_;
	int* H = &HAUTEUR_;
	int* L = &LARGEUR_;
	bool affichage = seam->affichage;
*/
	//FIN OPTIMISATION
	int x;
	int y;
	int i;
	int largeurImage = LARGEUR_;
	
	clock_t t1, t2, t3; //Va calculer le temps de remplissage de I de T et des deux.
	for(i=0; i<repetition; i++){ //On va supprimer plusieurs colonnes, selon repetition.
		
		//Premiere etape : on calcule I.
		//t1 = clock();
		
		remplissageI(I_, imageNB_, largeurImage, HAUTEUR_);
		
		//t2 = clock();
		//Remplis le tableau T
		remplissageT(I_, T_, largeurImage, HAUTEUR_);
/*
		t3 = clock();
		printf("Temps de remplissage   I : %.2d ms  |   T : %.2d ms  |   total : %.2d ms\n",
			(int)((float)(t2-t1)/CLOCKS_PER_SEC*1000), (int)((float)(t3-t2)/CLOCKS_PER_SEC*1000), (int)((float)(t3-t1)/					CLOCKS_PER_SEC*1000));
*/

		y = HAUTEUR_-2;
		x = min(T_[y], LARGEUR_);
		while(y>0){
			if(seam->affichage){
				imageNB_[y][x] = 255; T_[y][x] = 5000000; 
				imageRVB_[y][x][0] = 255;//Permet d'afficher les colonnes que l'on supprime, en rouge.
				imageRVB_[y][x][1] = 0;
				imageRVB_[y][x][2] = 0;
			}
			else{
				pop2D(imageRVB_[y], x, LARGEUR_, sizeof(char)); //On supprime la colonne dans l'image original.
			}
			pop(imageNB_[y], x, LARGEUR_, sizeof(char));
			y--;
			x = parcoursChemin(T_[y], x, LARGEUR_);
			
		}
		
		seam->affichage ? 0 : (LARGEUR_)--;	
		(largeurImage)--;
		
	}
}

void pushColonnes(seam_t* seam, int repetition){
	int x;
	int y;
	int i;
	int largeurImage = LARGEUR_;
	
	remplissageI(I_, imageNB_, largeurImage, HAUTEUR_);

   for(i=0; i<repetition; i++){ //On va dupliquer plusieurs colonnes, selon repetition.
	   remplissageT(I_, T_, largeurImage, HAUTEUR_);
	   y = HAUTEUR_-2;
	   x = min(T_[y], LARGEUR_);

	   while(y>0){
		  push2D(imageRVB_[y], x, LARGEUR_, sizeof(char)); //On duplique la colonne dans l'image original.
		  I_[y][x] += 5000;
		  push(I_[y], x, LARGEUR_, sizeof(int));
		  y--;
		  x = parcoursChemin(T_[y], x, LARGEUR_);
	   }
		seam->affichage ? 0 : (LARGEUR_)++;
		(largeurImage)++;
	}
}


void remplissageI(int** I, unsigned char** imageNB, int largeur, int hauteur){
	
	int i, j, Gx, Gy;
	for (i=1; i<hauteur-1; i++){ //On ignore les bords de l'image.
		for (j=1; j<largeur-1; j++){  //Algorithme de Sobel.
/*
  SOBEL
*/
			
			Gx = ((2*(imageNB[i][j-1]-imageNB[i][j+1]))+(imageNB[i-1][j-1]-imageNB[i-1][j+1])+(imageNB[i+1][j-1]-imageNB[i+1][j+1]));
			Gy = ((2*(imageNB[i+1][j]-imageNB[i-1][j]))+(imageNB[i-1][j-1]-imageNB[i+1][j-1])+(imageNB[i-1][j+1]-imageNB[i+1][j+1]));
/*
			Gx = imageNB[i][j-1]-imageNB[i][j+1];
			Gy = imageNB[i][j-1]-imageNB[i][j+1];
*/
			I[i][j] = sqrt(
						pow(Gx, 2) //On met au carré.
						+
						pow(Gy, 2) //On met au carré.
						);
		}

	}
	
	
	
}
void remplissageT(int** I, int** T, int largeur, int hauteur){
	
	int i, j;

	for (j=1; j<largeur-1; j++) //On remplit la premiere ligne
		T[1][j] = I[1][j];
	for (i=2; i<hauteur-1; i++) //On remplit la premiere colonne
		T[i][1] = I[i][1] + fmin(T[i-1][1], T[i-1][2]);


	for (i=2; i<hauteur-1; i++){ 
		for (j=2; j<largeur-2; j++)
			T[i][j] = I[i][j] + fmin(T[i-1][j], fmin(T[i-1][j-1], T[i-1][j+1]));
		T[i][largeur-2] = I[i][largeur-2] + fmin(T[i-1][largeur-2], T[i-1][largeur-3]);
	}
}

void pop(void* tab, int x, int taille, size_t s){
	//Cette fonction va supprimer le x-ième élément du tableau
	int i;
	if(s==sizeof(int)){
		int* t = tab;
		for(i=x; i<taille; i++)
			t[i]=t[i+1];
	}
	else{
		char* t = tab;
		for(i=x; i<taille; i++)
			t[i]=t[i+1];
	}
	
}


void pop2D(void* tab, int x, int taille, size_t s){
	//Cette fonction va supprimer la x-ième ligne du tableau
	int i;
	if(s==sizeof(int)){
		int** t = tab;
		for(i=x; i<taille; i++)
			t[i]=t[i+1];
	}
	else{
		char** t = tab;
		for(i=x; i<taille; i++)
			t[i]=t[i+1];
	}

}
void push(void* tab, int x, int taille, size_t s){
	//Cette fonction va dupliquer le x-ième élement du tableau
	int i;
	if(s==sizeof(int)){
		int* t = tab;
		for(i=taille-1; i>=x; i--)
			t[i+1]=t[i];	
	}
	else{
		char* t = tab;
		for(i=taille-1; i>=x; i--)
			t[i+1]=t[i];
	}
}


void push2D(void* tab, int x, int taille, size_t s){
	//Cette fonction va dupliquer la x-ième ligne du tableau
	int i;
	if(s==sizeof(int)){
		int** t = tab;
		for(i=taille-1; i>=x; i--)
			t[i+1]=t[i];	
	}
	else{
		char** t = tab;
		for(i=taille-1; i>=x; i--)
			t[i+1]=t[i];
	}
}


int parcoursChemin(int tab[TAILLE_MAX], int x, int LARGEUR){
	
	if(x<=1)
		return (tab[x] > tab[x+1]) ? x+1 : x;
	if(x>=LARGEUR-2)
		return (tab[x] > tab[x-1]) ? x-1 : x;
	
	if ((tab[x] <= tab[x+1]) && (tab[x] <= tab[x-1]))
		return x;
	else if (tab[x+1] <= tab[x-1])
		return x+1;
	else 
		return x-1;

}



int min(int *tab, int taille){
	int i, indiceMin = 1;

	int min=tab[1];
	for(i=2; i<taille-1; i++){
		 if (tab[i]<min){
				min=tab[i];
				indiceMin = i;
		 }
	}


	return indiceMin;
}

void* alloueTab2D(size_t s){
	int i;
	if(s==sizeof(int)){
		int** t = malloc(sizeof(int*) * TAILLE_MAX);
		for (i=0; i<TAILLE_MAX; i++)
			t[i] = malloc(s * TAILLE_MAX);
		return t;
	}
	else{
		char** t = malloc(sizeof(char*) * TAILLE_MAX);
		for (i=0; i<TAILLE_MAX; i++)
			t[i] = malloc(s * TAILLE_MAX);
		return t;
	}
	
	
}
void* alloueTab3D(size_t s){
	int i, j;
	unsigned char*** tab = malloc(sizeof(char**)*TAILLE_MAX);
	for (i=0; i<TAILLE_MAX; i++){
		tab[i] = malloc(sizeof(char*)*TAILLE_MAX);
		for (j=0; j<TAILLE_MAX; j++)
			tab[i][j] = malloc(sizeof(char)*3);	//*3 car chaque pixel est un triplet RVB.
	}
	return tab;
}



