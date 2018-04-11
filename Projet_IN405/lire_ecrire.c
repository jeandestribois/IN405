#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "deck.h"
#include "mes_types.h"
#include "fonctions_liste.h"
#include "lire_ecrire.h"

void gere_erreur(int erreur)
{
	switch(erreur)
	{
		case 1: fprintf(stderr,"Erreur lors de l'ouverture du fichier\n");
		case 2:	fprintf(stderr,"Impossible de lire les informations données dans le fichier\n");
		case 3: fprintf(stderr,"Imposible de creer une simulation car le fichier ne possede pas d'information(s) sur le(s) joueur(s)\n");
		case 4: fprintf(stderr,"Erreur lors de la lecture du fichier\n");
		case 5:	fprintf(stderr,"Pas assez d'information pour les joueurs\n");
		case 6: fprintf(stderr,"Erreur lors de la fermeuture d'un fichier\n");
		case 7: fprintf(stderr,"Erreur lors de la creation d'un fichier\n");
		case 8: fprintf(stderr,"Erreur lors de l'ecriture d'un fichier\n");
	}
	exit(0);
}


TABLE lire_fichier(const char *nom)
{
	int erreur=0;		// Variable à 0 si il n'y a pas d'erreur ou 1 si il y en a
	int fd;
	fd=open(nom,O_RDONLY);

	if(fd==-1)
	{
		gere_erreur(1);
	}

	char c;
	TABLE t;
	ssize_t ret;		// Variable récupérant la valeur de retour de la fonction read
	t.nbJoueurs=0;;
	t.nbDecks=0;
	t.nbMains=0;

	/*	
		Lecture de la première ligne
	*/

	ret=read(fd,&c,sizeof(char));
	while(c!=';' && ret>0 && erreur==0)		// Boucle pour le nombre de joueurs
	{
		if(!isdigit(c)) erreur=2;			// Vérifie qu'on lit bien un chiffre
		t.nbJoueurs=t.nbJoueurs*10+atoi(&c);		// Ajoute le chiffre lu au nombre final
		ret=read(fd,&c,sizeof(char));			// Lecture du prochain caractere
	}
	ret=read(fd,&c,sizeof(char));
	while(c!=';' && ret>0 && erreur==0)		// Boucle pour le nombre de decks
	{
		if(!isdigit(c)) erreur=2;
		t.nbDecks=t.nbDecks*10+atoi(&c);
		ret=read(fd,&c,sizeof(char));	
	}
	ret=read(fd,&c,sizeof(char));
	while(c!='\n' && ret>0 && erreur==0)		// Boucle pour le nombre max de mains
	{
		if(!isdigit(c)) erreur=2;
		t.nbMains=t.nbMains*10+atoi(&c);
		ret=read(fd,&c,sizeof(char));	
	}

	if(ret==0) erreur=3;		// Si on arrive au bout du fichier
	if(ret==-1) erreur=4;			// Si la fonction read a rencontré un problème
	if(erreur!=0) gere_erreur(erreur);

	printf("Nombre de joueurs : %d\n",t.nbJoueurs);
	printf("Nombre de decks : %d\n",t.nbDecks);
	printf("Nombre de main : %d\n",t.nbMains);


	t.joueurs=malloc(sizeof(JOUEUR)*t.nbJoueurs);
	t.decks=malloc(sizeof(deck_t)*t.nbDecks);

	/*
		Lecture des autres lignes
	*/

	for(int i=0; i<t.nbJoueurs; i++)
	{
		t.joueurs[i].nbJetons=0;
		t.joueurs[i].mise=0;
		t.joueurs[i].typeMise='c';	// Lorsque rien est indiqué, la mise est de type constant
		t.joueurs[i].valStop=0;
		t.joueurs[i].objJetons=0;

		ret=read(fd,&c,sizeof(char));
		while(c!=';' && ret>0 && erreur==0)		// Boucle pour le nombre de jetons
		{
			if(!isdigit(c)) erreur=2;
			t.joueurs[i].nbJetons=t.joueurs[i].nbJetons*10+atoi(&c);
			ret=read(fd,&c,sizeof(char));
		}
		ret=read(fd,&c,sizeof(char));
		while(c!=';' && ret>0 && erreur==0)		// Boucle pour la mise
		{
			if(!isdigit(c))
			{
				if(c=='+' || c=='-') t.joueurs[i].typeMise=c;		// Obtention du type de la mise
				else erreur=2;
			}
			else t.joueurs[i].mise=t.joueurs[i].mise*10+atoi(&c);
			ret=read(fd,&c,sizeof(char));	
		}
		ret=read(fd,&c,sizeof(char));
		while(c!=';' && ret>0 && erreur==0)		// Boucle pour la valeur d'arret
		{
			if(!isdigit(c)) erreur=2;
			t.joueurs[i].valStop=t.joueurs[i].valStop*10+atoi(&c);
			ret=read(fd,&c,sizeof(char));
		}
		ret=read(fd,&c,sizeof(char));
		while(c!='\n' && ret>0 && erreur==0)		// Boucle pour l'objectif du nombre de jetons
		{
			if(!isdigit(c)) erreur=2;
			t.joueurs[i].objJetons=t.joueurs[i].objJetons*10+atoi(&c);
			ret=read(fd,&c,sizeof(char));	
		}

		if(ret==0 && i<t.nbJoueurs-1) erreur=5;	// Si on arrive à la fin du fichier mais n'ayant les informations de tout les joueurs
		if(ret<0) erreur=4;
		if(erreur!=0)
		{
			free(t.joueurs);
			free(t.decks);
			gere_erreur(erreur);
		}
		printf("Joueur %d : nombre de jetons : %d\n",i+1,t.joueurs[i].nbJetons);
		printf("Joueur %d : type de la mise : %d%c\n",i+1,t.joueurs[i].mise,t.joueurs[i].typeMise);
		printf("Joueur %d : valeur stop : %d\n",i+1,t.joueurs[i].valStop);
		printf("Joueur %d : objectif jetons : %d\n",i+1,t.joueurs[i].objJetons);
	}
	ret=close(fd);
	if(ret==-1)
	{
		free(t.joueurs);
		free(t.decks);
		gere_erreur(6);
	}
	return t;
}

void ecrire_fichier(INFOJOUEURS infoJoueurs[], int nbJoueurs)
{
	int fd;
	ssize_t ret;
	char nom[100];					// Chaine de caractere stockant le nom des fichiers qu'on cree
	char entier[10];				// Chaine de caractere stockant les chiffres d'un entier
	char c;							// Caractere stockant le ';' ou le retour à la ligne
	INFOJOUEURS tmp;
	for(int i=0; i<nbJoueurs; i++)
	{
		tmp=infoJoueurs[i];
		while(tmp!=NULL && ret!=-1)
		{
			sprintf(nom,"joueur_n%d.blackjack",i+1);
			fd=open(nom,O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
			if(fd==-1)
			{
				gere_erreur(7);
			}
			c=';';

			ret=write(fd,tmp->cartesJoueur,strlen(tmp->cartesJoueur)*sizeof(*tmp->cartesJoueur));

			ret=write(fd,&c,sizeof(char));

			sprintf(entier,"%d",tmp->totalJoueur);			// Je stock l'entier dans une chaine de caractere grace à sprintf()
			ret=write(fd,&entier,strlen(entier)*sizeof(char));

			ret=write(fd,&c,sizeof(char));

			ret=write(fd,tmp->cartesBanque,strlen(tmp->cartesBanque)*sizeof(*tmp->cartesBanque));
			
			ret=write(fd,&c,sizeof(char));

			sprintf(entier,"%d",tmp->totalBanque);
			ret=write(fd,&entier,strlen(entier)*sizeof(char));

			ret=write(fd,&c,sizeof(char));

			sprintf(entier,"%d",tmp->mise);
			ret=write(fd,&entier,strlen(entier)*sizeof(char));

			ret=write(fd,&c,sizeof(char));
			
			sprintf(entier,"%d",tmp->gain);
			ret=write(fd,&entier,strlen(entier)*sizeof(char));

			ret=write(fd,&c,sizeof(char));
			
			sprintf(entier,"%d",tmp->nbJetons);
			ret=write(fd,&entier,strlen(entier)*sizeof(char));

			c='\n';
			ret=write(fd,&c,sizeof(char));

			if(ret==-1)
			{
				for(int i=0; i<nbJoueurs; i++) libere_memoire(infoJoueurs[i]);
				gere_erreur(8);
			}

			tmp=tmp->suiv;
		}
		ret=close(fd);
		if(ret==-1)
		{
			for(int i=0; i<nbJoueurs; i++) libere_memoire(infoJoueurs[i]);
			gere_erreur(6);
		}
		libere_memoire(infoJoueurs[i]);
	}
}