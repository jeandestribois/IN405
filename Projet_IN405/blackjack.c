#include <stdlib.h>
#include <stdio.h>

#include "deck.h"
#include "mes_types.h"
#include "fonctions_liste.h"
#include "lire_ecrire.h"

int main(int argc, const char *argv[])
{
	if(argc!=2)
	{
		fprintf(stderr,"Mauvais usage de l'executable.\nRappel : %s <unfichierniveau.blackjack>.\n",argv[0]);
		exit(0);
	}
	TABLE t=lire_fichier(argv[1]);
	INFOJOUEURS infoJoueurs[1];
	infoJoueurs[0]=NULL;
	infoJoueurs[0]=ajoute_element_fin(infoJoueurs[0],"J68",24,"98",17,50,0,950);
	ecrire_fichier(infoJoueurs,1);

	exit(0);
}