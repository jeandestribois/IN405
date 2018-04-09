typedef struct joueur			// Structure contenant les informations initiale pour un joueur
{
	int nbJetons;
	int mise;
	char typeMise;
	int valStop;
	int objJetons;
} JOUEUR;

typedef struct table			// Structure contenant les informations initiale pour une table
{
	JOUEUR *joueurs;
	deck_t *decks;
	int nbMains;
	int nbJoueurs;
	int nbDecks;
} TABLE;

typedef struct infotour		// Structure contenant les informations d'un tour joué pour un joueur	
{
	char cartesJoueur[10];
	int totalJoueur;
	char cartesBanque[10];
	int totalBanque;
	int mise;
	int gain;
	int nbJetons;
} INFOTOUR;

typedef struct infojoueurs		// Liste chaîné contenant les informations de toute la simulation
{
	INFOTOUR infoTour;
	struct infojoueurs *suiv;
} INFOJOUEURS;