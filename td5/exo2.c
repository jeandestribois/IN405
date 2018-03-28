#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Enumération des différents opérateurs possibles
typedef enum {
	OCD_SOMME,
	OCD_MOYENNE,
	OCD_MAX,
	OCD_MIN,
	OCD_TOTAL
} opcode_t;

// Structure contenant les arguments du programme
typedef struct {
	int nbThreads;			//< Nombre de threads à utiliser
	int tailleTableau;		//< Taille du tableau utilisé pour la réduction
	opcode_t code;			//< Opération à réaliser
} arg_t;

// Structure contenant les informations nécessaires au thread
typedef struct {
	int *t;
	int debut;
	int fin;
	int resultat;
} message_t;

// Alias de pointeurs de fonction
typedef int (* ptrVerif) (int *, int, int);

// Fonction thread -- calcule la somme locale d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * sommeTableau (void * arg)
{
	message_t *mes = (message_t*)arg;
	mes->resultat=0;
	for(int i=mes->debut; i<mes->fin; i++)
	{
		mes->resultat+=mes->t[i];
	}
	pthread_exit(NULL);
}

// Fin de la réduction -- calcule la somme globale
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducSomme (message_t * mes, int nbThreads)
{
	int resultat=0;
	for(int i=0; i<nbThreads; i++)
	{
		resultat+=mes[i].resultat;
	}
	return resultat;
}

// Fonction thread -- calcule la moyenne locale d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * moyenneTableau (void * arg)
{
	message_t *mes=(message_t*)arg;
	mes->resultat=0;
	int compteur=0;
	for(int i=mes->debut; i<mes->fin; i++)
	{
		mes->resultat+=mes->t[i];
	}
	pthread_exit(NULL);
}

// Fin de la réduction -- calcule la moyenne globale
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMoyenne (message_t * mes, int nbThreads,arg_t arg)
{
	int resultat=0;
	for(int i=0; i<nbThreads; i++)
	{
		resultat+=mes[i].resultat;
	}
	resultat=resultat/arg.tailleTableau;
	return resultat;
}

// Fonction thread -- calcule le maximum local d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * maxTableau (void * arg)
{
	message_t *mes=(message_t*)arg;
	mes->resultat=0;
	for(int i=mes->debut; i<mes->fin; i++)
	{
		if(mes->resultat<mes->t[i])	mes->resultat=mes->t[i];
	}
	pthread_exit(NULL);
}

// Fin de la réduction -- calcule le maximum global
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMax (message_t * mes, int nbThreads)
{
	int resultat=0;
	for(int i=0; i<nbThreads; i++)
	{
		if(resultat<mes[i].resultat)	resultat=mes[i].resultat;
	}
	return resultat;
}

// Fonction thread -- calcule le minimum local d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * minTableau (void * arg)
{
	message_t *mes=(message_t*)arg;
	mes->resultat=0;
	for(int i=mes->debut; i<mes->fin; i++)
	{
		if(mes->resultat>mes->t[i])	mes->resultat=mes->t[i];
	}
	pthread_exit(NULL);
}

// Fin de la réduction -- calcule le minimum global
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMin (message_t * mes, int nbThreads)
{
	int resultat=0;
	for(int i=0; i<nbThreads; i++)
	{
		if(resultat>mes[i].resultat) resultat=mes[i].resultat;
	}
	return resultat;
}

// NE PAS TOUCHER
// Fonction de vérification -- somme des éléments du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifSomme (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == tmp;
}

// NE PAS TOUCHER
// Fonction de vérification -- moyenne des éléments du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMoyenne (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	if (tailleTableau == 0)
		return resultat == 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == (tmp / tailleTableau);
}

// NE PAS TOUCHER
// Fonction de vérification -- maximum du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMax (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp < tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// NE PAS TOUCHER
// Fonction de vérification -- minimum du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMin (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 101;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp > tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// Analyse des arguments du programme. Quitte si le nombre d'arguments est
// différent de 4. Remplit la structure de données des arguments.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Structure de données des arguments
arg_t analyseArguments (int argc, char ** argv)
{
	arg_t arg;
	if (argc!=4)
	{
		fprintf(stderr,"Usage correcte : %s <nbre threads> <taille tableau> <opcode>\nopcode : + ou / ou M ou m\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	arg.nbThreads=atoi(argv[1]);
	arg.tailleTableau=atoi(argv[2]);
	switch (argv[3][0])
	{
		case '+': arg.code=OCD_SOMME;
		case '/': arg.code=OCD_MOYENNE;
		case 'M': arg.code=OCD_MAX;
		case 'm': arg.code=OCD_MIN;
		default : fprintf(stderr,"Opcode non reconnu\nopcode : + ou / ou M ou m");
	}
	return arg;
}

// NE PAS TOUCHER
// Récupération de la fonction de vérification à partir de l'opcode
// \param	opcode 			Code de l'opération
// \return					Pointeur de la fonction de vérification
ptrVerif decodeOpcodeVerif (const opcode_t o) {
	switch (o) {
		case OCD_SOMME:		return verifSomme;
		case OCD_MOYENNE:	return verifMoyenne;
		case OCD_MAX:		return verifMax;
		case OCD_MIN:		return verifMin;
		default:
			printf ("L'opération n'a pas été reconnue\n");
			exit (-1);
	}
}

// Génération du tableau avec des entiers compris entre 1 et 100.
// \param	tailleTableau	Taille du tableau d'entiers
// \return					Tableau d'entiers
int * genereTableau (int tailleTableau) {  }

// Fonction chargée de la réduction multi-threadé, elle va initialiser les
// différentes variables utilisées par les threads (tableau d'entier, messages,
// etc.) puis créer les threads. Une fois l'exécution des threads terminée et
// la réduction opérée, la vérification du résultat est faite.
// \param	arg 			Arguments du programme décodés
void programmePrincipal (const arg_t arg) {
	// Déclaration des variables
	int * tab, res;
	pthread_t *thread;
	message *mes;

	// Allocation de la mémoire
	tab=malloc(sizeof(int)*arg.tailleTableau);
	thread=malloc(sizeof(pthread_t)*arg.nbThreads);
	mes=malloc(sizeof(message)*arg.nbThreads);

	// Initialisation des variables et création des threads
	for(int i=0; i<arg.tailleTableau; i++)
	{
		tab[i]=1+rand()%101%;
	}
	mes[0].t=t;
	mes[0].debut=0;
	mes[0].fin=arg.tailleTableau/arg.nbThreads;
	for(int i=1; i<arg.tailleTableau-1; i++)
	{
		mes[i].t=t;
		mes[i].debut=mes[i-1].fin+1;
		mes[i].fin=mes[i].debut+(1+taille/nbre_thread);
	}
	mes[nbre_thread-1].t=t;
	mes[nbre_thread-1].debut=mes[nbre_thread-2].fin+1;
	mes[nbre_thread-1].fin=taille-1;


	// Jointure

	// Réduction et affichage du résultat

	// NE PAS TOUCHER
	if ( (* (decodeOpcodeVerif (arg.code) ) ) (tab, arg.tailleTableau, res) )
		printf ("Le resultat est juste.\n");
	else printf ("Le resultat est faux.\n");
	// FIN

	// Libération de la mémoire
	free(tab);
	free(thread);
	malloc(mes);
}

// NE PAS TOUCHER
// Fonction main -- analyse les arguments donnés au
// programme, puis exécute le programme principal, chargé de la réduction
// multi-threadée.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Code de retour du programme (0 en cas de réussite)
int main (int argc, char ** argv) {
	arg_t a;

	srand (time (NULL) );

	a = analyseArguments (argc, argv);
	programmePrincipal (a);
	
	return 0;
}
