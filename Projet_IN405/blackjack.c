#include <stdlib.h>
#include <stdio.h>

#include "deck.h"
#include "mes_types.h"
#include "fonctions_liste.h"
#include "lire_ecrire.h"
#include "simuler.h"

int main(int argc, const char *argv[])
{
	if(argc!=2)
	{
		fprintf(stderr,"Mauvais usage de l'executable.\nRappel : %s <unfichierniveau.blackjack>.\n",argv[0]);
		exit(0);
	}
	TABLE t=lire_fichier(argv[1]);
	INFOJOUEURS *info=simuler(t);
	
	ecrire_fichier(info,t.nbJoueurs);

	exit(0);
}