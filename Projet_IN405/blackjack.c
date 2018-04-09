#include <stdlib.h>
#include <stdio.h>

#include "deck.h"
#include "mes_types.h"
#include "lire_ecrire.h"

int main(int argc, const char *argv[])
{
	TABLE t=lire_fichier(argv[1]);
	INFOJOUEURS infoJoueurs[1];
	infoJoueurs[0]=malloc(sizeof(struct infojoueurs));
	infoJoueurs[0]->suiv=NULL;
	infoJoueurs[0]->cartesJoueur="J68";
	infoJoueurs[0]->totalJoueur=24;
	infoJoueurs[0]->cartesBanque="98";
	infoJoueurs[0]->totalBanque=17;
	infoJoueurs[0]->mise=50;
	infoJoueurs[0]->gain=0;
	infoJoueurs[0]->nbJetons=950;
	ecrire_fichier(infoJoueurs,1);

}