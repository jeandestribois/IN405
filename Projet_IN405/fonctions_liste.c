#include <stdlib.h>
#include <stdio.h>

#include "deck.h"
#include "mes_types.h"



INFOJOUEURS ajoute_element_fin(INFOJOUEURS info, char *cartesJoueur, int totalJoueur, char *cartesBanque, int totalBanque, int mise, int gain, int nbJetons)
{
    INFOJOUEURS tmp, info2;

    tmp=malloc(sizeof(struct infojoueurs));
    tmp->cartesJoueur=cartesJoueur;
    tmp->totalJoueur=totalJoueur;
    tmp->cartesBanque=cartesBanque;
    tmp->totalBanque=totalBanque;
    tmp->mise=mise;
    tmp->gain=gain;
    tmp->nbJetons=nbJetons;
   	tmp->suiv=NULL;

    if(info==NULL)
    {
        return tmp;
    }
    info2=info;
    while(info->suiv!=NULL)
    {
        info=info->suiv;
    }
    info->suiv=tmp;
    return info2;
}


INFOJOUEURS libere_memoire(INFOJOUEURS info)
{
	if(info!=NULL)
	{
		info->suiv=libere_memoire(info->suiv);
		free(info);
	}
	return NULL;
}