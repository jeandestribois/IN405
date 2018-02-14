#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "se_fichier.h"


SE_FICHIER SE_ouverture (const char *chemin, int acces)
{
	SE_FICHIER fichier_sortie;
	fichier_sortie.descripteur=open(chemin, acces);
	fichier_sortie.chemin=chemin;
	fichier_sortie.acces=acces;
	return fichier_sortie;
}

int SE_fermeture (SE_FICHIER fichier)
{
	
}

int SE_suppression (const char *chemin)
{
	
	
}

int SE_lectureCaractere (SE_FICHIER fichier, char * caractere)
{
	
}

int SE_ecritureCaractere (SE_FICHIER fichier, const char caractere)
{
	
}
