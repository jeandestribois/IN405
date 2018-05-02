#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "deck.h"
#include "mes_types.h"
#include "fonctions_liste.h"


void gagneOuPerd(INFOJOUEURS *tmp, TABLE t, TUBE *tubeVersJoueur, TUBE *tubeVersBanque, int *nbJetons, int *quiJoue)
{
	int aGagner;
	for(int i=0; i<t.nbJoueurs; i++)
	{
		if(quiJoue[i])
		{
			if(tmp[i]->totalJoueur<17 || tmp[i]->totalJoueur>21)		// Si le joueur perd
			{
				tmp[i]->gain=0;
				aGagner=0;
			}
			else if(tmp[i]->blackjackJ)									// Si le joueur fait blackjack
			{
				tmp[i]->gain=3*tmp[i]->mise;
				aGagner=1;
			}
			else if(tmp[i]->totalBanque<17 || tmp[i]->totalBanque>21)	// Si la banque perd
			{
				if(!tmp[i]->blackjackJ) tmp[i]->gain=2*tmp[i]->mise;			// S'il n'a pas fait de blackjack
				else tmp[i]->gain=3*tmp[i]->mise;								// S'il a fait un blackjack
				aGagner=1;
			}
		
			else if(tmp[i]->totalBanque == 21)							// Si la banque fait un blackjack
			{
				tmp[i]->gain=0;
				aGagner=0;
			}
			else														// Si aucun des deux ne perd ou ne fait de blackjack
			{
				if(tmp[i]->totalBanque < tmp[i]->totalJoueur)					// Si le total joueur est plus grand que le total banque
				{
					tmp[i]->gain=2*tmp[i]->mise;
					aGagner=1;
				}
				else if(tmp[i]->totalBanque < tmp[i]->totalJoueur)				// Si l'inverse
				{
					tmp[i]->gain=0;
					aGagner=0;
				}
				else															// Si il y a égalité
				{
					tmp[i]->gain=tmp[i]->mise;
					aGagner=0;
				}
			}
			nbJetons[i]+=tmp[i]->gain;
			tmp[i]->nbJetons=nbJetons[i];
			write(tubeVersJoueur[i].tube[1],&aGagner,sizeof(int));				// On lui dit s'il a gagner
			write(tubeVersJoueur[i].tube[1],&nbJetons[i],sizeof(int));		// On lui dit le nombre de jetons qu'il a
		}
	}
}

void propositionCartes(INFOJOUEURS *tmp, TABLE t, TUBE *tubeVersJoueur, TUBE *tubeVersBanque, deck_t *deck, int *quiJoue)
{
	int pioche;			// Variable étant à 0 ou à 1 en fonction de si le joueur désire piocher ou non
	int carteID;
	int nbCarte;		// Variable stockant le nombre de cartes possédées
	int total;
	// La banque complete la main des joueurs
	for(int i=0; i<t.nbJoueurs; i++)		
	{
		if(quiJoue[i])
		{
			nbCarte=2;
			total=getCardValueFromChar(tmp[i]->cartesJoueur[0])+getCardValueFromChar(tmp[i]->cartesJoueur[1]);
			read(tubeVersBanque[i].tube[0],&pioche,sizeof(int));		// La banque attend de savoir si le joueur veut piocher ou pas
			while(pioche)
			{
				carteID=drawCard(deck);
				tmp[i]->cartesJoueur[nbCarte]=getCharFromCardValue(getValueFromCardID(carteID));
				total+=getValueFromCardID(carteID);
				write(tubeVersJoueur[i].tube[1],&carteID,sizeof(int));
				read(tubeVersBanque[i].tube[0],&pioche,sizeof(int));
				nbCarte++;
			}
			if(nbCarte==2 && total==21) tmp[i]->blackjackJ=1;
			tmp[i]->totalJoueur=total;
		}
	}
	// La banque complète sa main
	nbCarte=2;
	total=getCardValueFromChar(tmp[0]->cartesBanque[0])+getCardValueFromChar(tmp[0]->cartesBanque[1]);
	while(total<17)
	{
		carteID=drawCard(deck);
		total+=getValueFromCardID(carteID);
		for(int i=0; i<t.nbJoueurs; i++)
		{
			if(quiJoue[i])
			{
				tmp[i]->cartesBanque[nbCarte]=getCharFromCardValue(getValueFromCardID(carteID));
				tmp[i]->totalBanque=total;
			}		
		}
		nbCarte++;
	}
}

void distributionCartes(INFOJOUEURS *tmp, TABLE t, TUBE *tubeVersJoueur, TUBE *tubeVersBanque, deck_t *deck, int *quiJoue)
{
	int carteID;
	char carteCharJ;
	char carteCharB;

	shuffleDeck(deck);

	// Distribution de la premiere carte pour chaque joueurs
	for(int i=0; i<t.nbJoueurs; i++)
	{
		if(quiJoue[i])
		{
			carteID=drawCard(deck);												// On pioche une carte
			carteCharJ=getCharFromCardValue(getValueFromCardID(carteID));		// On prend le carractere correspondant
			tmp[i]->cartesJoueur[0]=carteCharJ;									// On inscrit la carte dans la sructure infojoueurs
		
			write(tubeVersJoueur[i].tube[1],&carteID,sizeof(int));
		}
	}
	// On pioche une derniere carte pour la banque (la banque pioche en dernier)
	carteID=drawCard(deck);
	carteCharB=getCharFromCardValue(getValueFromCardID(carteID));
	// Distribution d'une deuxieme carte pour chaque joueurs
	for(int i=0; i<t.nbJoueurs; i++)
	{
		if(quiJoue[i])
		{
			carteID=drawCard(deck);
			carteCharJ=getCharFromCardValue(getValueFromCardID(carteID));
			tmp[i]->cartesJoueur[1]=carteCharJ;
			tmp[i]->cartesBanque[0]=carteCharB; 					// On en profite pour inscrire la premiere carte piocher par le pere

			write(tubeVersJoueur[i].tube[1],&carteID,sizeof(int));
		}
	}
	// On pioche la deuxieme carte de la banque
	carteID=drawCard(deck);
	carteCharB=getCharFromCardValue(getValueFromCardID(carteID));
	for(int i=0; i<t.nbJoueurs; i++)
	{
		if(quiJoue[i]) tmp[i]->cartesBanque[1]=carteCharB;
	}

}

void obtenirMise(INFOJOUEURS *tmp, TABLE t, TUBE *tubeVersBanque, int *nbJetons, int *quiJoue)
{
	for(int i=0; i<t.nbJoueurs; i++)
	{
		if(quiJoue[i])
		{
			read(tubeVersBanque[i].tube[0],&tmp[i]->mise,sizeof(int));
			nbJetons[i]-=tmp[i]->mise;
		}
	}
}

int jouerB(TABLE t, TUBE *tubeVersJoueur, TUBE *tubeVersBanque, int numMain, int *quiJoue)
{
	int compteur=0;
	int joue=1;
	if(numMain<t.nbMains)					// Si on a pas encore atteint le nombre de main max
	{
		for(int i=0; i<t.nbJoueurs; i++)
		{
			if(quiJoue[i])
			{
				write(tubeVersJoueur[i].tube[1],&joue,sizeof(int));
				read(tubeVersBanque[i].tube[0],&quiJoue[i],sizeof(int));
				if(quiJoue[i]) compteur++;
			}
		}
		if(compteur) return 1;				// Si un joueur au moins joue on continue
		else return 0;						// Sinon on arrete
	}
	else
	{
		joue=0;
		for(int i=0; i<t.nbJoueurs; i++)
		{
			if(quiJoue[i])
			{
				write(tubeVersJoueur[i].tube[1],&joue,sizeof(int));
			}
		}
		return 0;
	}
}

void banque(TABLE t, INFOJOUEURS *infoJoueurs, TUBE *tubeVersJoueur, TUBE *tubeVersBanque)
{
	printf("Début banque\n");
	deck_t *deck;
	card_t * tmpCard;
	INFOJOUEURS *tmp;					// infoJoueurs pointe vers le premier élément et tmp vers le dernier
	tmp=malloc(sizeof(INFOJOUEURS)*t.nbJoueurs);

	int *nbJetons;
	int *quiJoue;								// quiJoue est à 1 lorsque le joueur joue et à 0 lorsqu'il ne joue pas
	int numMain=0;								// Variable stockant le nombre de tours joués
	nbJetons=malloc(sizeof(int)*t.nbJoueurs);
	quiJoue=malloc(sizeof(int)*t.nbJoueurs);
	for(int i=0; i<t.nbJoueurs; i++)
	{
		quiJoue[i]=1;							// On initialise quiJoue à 1 pour tout le monde
		nbJetons[i]=t.joueurs[i].nbJetons;		// On initialise le nombre de jetons possédé à la valeur initiale de chacun
	}

	initDeckLib();
	deck=initDeck(P52,t.nbDecks);

	while(jouerB(t,tubeVersJoueur,tubeVersBanque,numMain,quiJoue))
	{
		printf("\nDébut tour %d\n", numMain);
		for(int i=0; i<t.nbJoueurs; i++)
		{
			if(quiJoue[i])
			{
				tmp[i]=malloc(sizeof(struct infojoueurs));	// On initialise tmp;
				tmp[i]->suiv=NULL;
				for(int j=0; j<21; j++)					// On met à vide les tableaux de caractere
				{
					tmp[i]->cartesJoueur[j]='\0';
					tmp[i]->cartesBanque[j]='\0';
				}
			}		
		}	

		while (deck->handCards != NULL){
			tmpCard = deck->handCards;
			deck->handCards = tmpCard->next;
			tmpCard->next = deck->discardPile;
			deck->discardPile = tmpCard;
		}

		printf("Banque : mélange deck\n");
		shuffleDeck(deck);
		printf("Banque : obtention mises\n");
		obtenirMise(tmp,t,tubeVersBanque,nbJetons,quiJoue);
		printf("Banque : distribution des cartes\n");
		distributionCartes(tmp,t,tubeVersJoueur,tubeVersBanque,deck,quiJoue);		// Distribue 2 cartes à chanque joueurs et à la banque
		printf("Banque : proposition des cartes\n");
		propositionCartes(tmp,t,tubeVersJoueur,tubeVersBanque,deck,quiJoue);		// La banque demande aux joueurs si ils veulent compléter leur main
		printf("Banque : distribution des gains\n");
		gagneOuPerd(tmp,t,tubeVersJoueur,tubeVersBanque,nbJetons,quiJoue);
		numMain++;
		for(int i=0; i<t.nbJoueurs; i++)
		{
			if(quiJoue[i]){
				infoJoueurs[i]=ajoute_element_fin(infoJoueurs[i],tmp[i]);
			}
		}
	}
	printf("\nFin Banque\n");
	removeDeck(deck);
	free(quiJoue);
	free(nbJetons);
	free(tmp);
}

int gereMise(TABLE t, int mise, int aGagner, int numJoueur, int nbJetons)
{
	if(t.joueurs[numJoueur].typeMise=='\0')				// Si le type de la mise est constant
	{
		mise=t.joueurs[numJoueur].mise;
	}
	else if(t.joueurs[numJoueur].typeMise=='+')			// Si le type de la mise est croissant
	{
		if(aGagner) mise=t.joueurs[numJoueur].mise;
		else mise=2*mise;
	}
	else												// Si le type de sa mise est descendante
	{
		if(aGagner) mise=t.joueurs[numJoueur].mise;
		else mise=mise/2;
	}
	if(mise>nbJetons) mise=nbJetons;					// Dans le cas où le joueur désire miser plus que ce qu'il possède
	return mise;
}

int jouerJ(TABLE t, TUBE *tubeVersJoueur, TUBE *tubeVersBanque, int nbJetons, int numJoueur)
{
	int caJoue;
	int jeJoue;
	read(tubeVersJoueur[numJoueur].tube[0],&caJoue,sizeof(int));
	if(caJoue)																	// Si la banque lui dit qu'on peut jouer
	{
		if(nbJetons>0 && nbJetons<t.joueurs[numJoueur].objJetons)				// Si le joueur peut ou veut toujours jouer
		{
			jeJoue=1;
			write(tubeVersBanque[numJoueur].tube[1],&jeJoue,sizeof(int));
			return 1;
		}
		else
		{
			jeJoue=0;
			write(tubeVersBanque[numJoueur].tube[1],&jeJoue,sizeof(int));
			return 0;
		}
	}
	else return 0;
}

void joueurs(TABLE t, TUBE *tubeVersJoueur, TUBE *tubeVersBanque, int numJoueur)
{
	printf("Début joueur %d\n", numJoueur);
	int carteID[21];							// 21 correspond au nombre maximum de carte qu'un joueur peut piocher
	int total;
	int pioche;
	int nbCarte;
	int mise=t.joueurs[numJoueur].mise;			// Variable permettant de stocker la mise voulu pour chaque joueur initialisé à la mise de base pour le premier tour
	int aGagner;								// Variable permettant de savoir si le joueur à perdu ou a gagner
	int nbJetons=t.joueurs[numJoueur].nbJetons;	// Variable permettant au joueur de connaitre le nombre de jetons qu'il possède
	if(mise>nbJetons) mise=nbJetons;			// On verifie que la mise initial n'est pas superieur au nombre de jetons
	int numMain=0;

	while(jouerJ(t,tubeVersJoueur,tubeVersBanque,nbJetons,numJoueur))
	{
		pioche=1;
		nbCarte=2;
		write(tubeVersBanque[numJoueur].tube[1],&mise,sizeof(int));					// Communication de la mise
		read(tubeVersJoueur[numJoueur].tube[0],&carteID[0],sizeof(int));			// Réception des deux premiere cartes
		read(tubeVersJoueur[numJoueur].tube[0],&carteID[1],sizeof(int));

		total=getValueFromCardID(carteID[0])+getValueFromCardID(carteID[1]);
		printf("Joueur %d : début complète main\n", numJoueur);
		while(total<t.joueurs[numJoueur].valStop)									// Distribution du reste des cartes
		{
			write(tubeVersBanque[numJoueur].tube[1],&pioche,sizeof(int));			// Demande de carte
			read(tubeVersJoueur[numJoueur].tube[0],&carteID[nbCarte],sizeof(int));	// Réception carte
			total+=getValueFromCardID(carteID[nbCarte]);
			nbCarte++;
		}
		printf("Joueur %d : fin complete main\n", numJoueur);
		pioche=0;
		write(tubeVersBanque[numJoueur].tube[1],&pioche,sizeof(int));				// Demande d'arret des cartes

		read(tubeVersJoueur[numJoueur].tube[0],&aGagner,sizeof(int));				// La banque lui dit si il a gagner ou pas
		read(tubeVersJoueur[numJoueur].tube[0],&nbJetons,sizeof(int));				// La banque lui dit le nombre de jetons qu'il possède
		mise=gereMise(t,mise,aGagner,numJoueur,nbJetons);							// Il calcul la prochaine mise
		numMain++;
	}
	printf("\nFin joueur %d\n", numJoueur);
}



INFOJOUEURS* simuler(TABLE t)
{
	INFOJOUEURS *infoJoueurs;
	infoJoueurs=malloc(sizeof(INFOJOUEURS)*t.nbJoueurs);		// On cree un tableau de la taille du nombre de joueur pour stocker les infos de la partie
	for(int i=0; i<t.nbJoueurs; i++) infoJoueurs[i]=NULL;		// On cree un élément à la liste

	int pere=1;				// Entier permettant de diffrencier le père des fils
	int numJoueur;			// Entier permettant d'identifier chaque fils
	TUBE *tubeVersJoueur;
	TUBE *tubeVersBanque;

	tubeVersJoueur=malloc(sizeof(struct tube)*t.nbJoueurs); 		// On cree un tableau de t.nbJoueurs tubes
	tubeVersBanque=malloc(sizeof(struct tube)*t.nbJoueurs);

	for(int i=0; i<t.nbJoueurs; i++)			// Initialisation des tubes
	{
		if(pipe(tubeVersJoueur[i].tube)==-1)
		{
			fprintf(stderr,"Erreur lors de l'utilisation de la fonction pipe()\n");
			free(tubeVersBanque);
			free(tubeVersJoueur);
			for(int i=0; i<t.nbJoueurs; i++)
			{
				libere_memoire(infoJoueurs[i]);
			}
			free(infoJoueurs);
			exit(0);
		}
		if(pipe(tubeVersBanque[i].tube)==-1)
		{
			fprintf(stderr,"Erreur lors de l'utilisation de la fonction pipe()\n");
			free(tubeVersBanque);
			free(tubeVersJoueur);
			for(int i=0; i<t.nbJoueurs; i++)
			{
				libere_memoire(infoJoueurs[i]);
			}
			free(infoJoueurs);
			exit(0);
		}
	}

	for(int i=0; i<t.nbJoueurs && pere; i++) 		// Création des processus et gestion des tubes
	{
		printf("Création du processus numero %d\n",i);
		pere=fork();
		if(pere==-1)
		{
			fprintf(stderr,"Erreur lors de l'utilisation de la fonction fork()\n");
			free(tubeVersBanque);
			free(tubeVersJoueur);
			for(int i=0; i<t.nbJoueurs; i++)
			{
				libere_memoire(infoJoueurs[i]);
			}
			free(infoJoueurs);
			exit(0);
		}
		else if(!pere)
		{
			numJoueur=i;									// Chaque fils aura un numéro different de 0 à nbJoueurs
			close(tubeVersJoueur[numJoueur].tube[1]);		// Chaque fils s'occupent de son pipe
			close(tubeVersBanque[numJoueur].tube[0]);
		}
		else
		{
			close(tubeVersJoueur[i].tube[0]);
			close(tubeVersBanque[i].tube[1]);
		}
	}

	if(pere) banque(t,infoJoueurs,tubeVersJoueur,tubeVersBanque);
	else joueurs(t,tubeVersJoueur,tubeVersBanque,numJoueur);

	// Fermeture des tubes
	if(pere)
	{
		printf("La banque ferme ses pipes\n");
		for(int i=0; i<t.nbJoueurs; i++)
		{
			close(tubeVersJoueur[i].tube[1]);
			close(tubeVersBanque[i].tube[0]);
		}
		printf("La banque attend ses procces fils\n");
		for(int i=0; i<t.nbJoueurs; i++)
		{
			wait(NULL);
		}
		printf("La banque a fini d'attendre\n");
		free(tubeVersJoueur);
		free(tubeVersBanque);
		printf("La banque a fini de free les pipes\n");
	}
	else
	{
		printf("Le joueur %d ferme ses pipes\n", numJoueur);
		close(tubeVersJoueur[numJoueur].tube[0]);
		close(tubeVersBanque[numJoueur].tube[1]);
		printf("Le joueur %d fait son exit(0)\n", numJoueur);
		exit(0);
	}

	printf("Sortie de simuler\n");
	return infoJoueurs;

}