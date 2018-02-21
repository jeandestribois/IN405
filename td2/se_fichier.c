#include "se_fichier.h"
#include <fcntl.h>
#include <unistd.h>

mode_t newFileMode = 0x644;
int charSize = sizeof(char);

SE_FICHIER SE_ouverture (
	const char * chemin,
	int acces){
	SE_FICHIER fichier;
	fichier.descripteur = open(chemin, acces|O_CREAT);
	fichier.chemin = chemin;
	fichier.acces = acces;
	return fichier;
}

int SE_fermeture (
	SE_FICHIER fichier){
	return close(fichier.descripteur);
}

int SE_suppression (
	const char * chemin){
	return remove(chemin);
}

ssize_t SE_lectureCaractere (
	SE_FICHIER fichier,
	char * caractere){
	return read(fichier.descripteur, caractere, charSize);
}

ssize_t SE_ecritureCaractere (
	SE_FICHIER fichier,
	const char caractere){
	return write(fichier.descripteur, &caractere, charSize);
}

/*
int SE_lectureChaine (
	SE_FICHIER fichier,
	char * chaine,
	int tailleMax);

int SE_ecritureChaine (
	SE_FICHIER fichier,
	const char * chaine,
	int taille);

int SE_lectureEntier (
	SE_FICHIER fichier,
	int * entier);

int SE_ecritureEntier (
	SE_FICHIER fichier,
	const int entier);
*/