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

struct infojoueurs		// Liste chaîné contenant les informations de toute la simulation
{
	char *cartesJoueur;
	int totalJoueur;
	char *cartesBanque;
	int totalBanque;
	int mise;
	int gain;
	int nbJetons;
	struct infojoueurs *suiv;
};
typedef struct infojoueurs* INFOJOUEURS;