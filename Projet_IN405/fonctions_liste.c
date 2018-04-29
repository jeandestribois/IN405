#include <stdlib.h>
#include <stdio.h>

#include "deck.h"
#include "mes_types.h"



INFOJOUEURS ajoute_element_fin(INFOJOUEURS info, INFOJOUEURS tmp)        // Lorsqu'on utilise cette fonction, on se trouve déjà à la fin de la liste
{
    if(info==NULL)
    {
        return tmp;
    }
    else
    {
        INFOJOUEURS tmpBis=info;
        while(info->suiv!=NULL)
        {
            info=info->suiv;   
        }
        info->suiv=tmp;
        return tmpBis;
    }
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