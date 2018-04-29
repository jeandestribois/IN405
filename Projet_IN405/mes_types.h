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
	int nbMains;
	int nbJoueurs;
	int nbDecks;
} TABLE;

struct infojoueurs		// Liste chaîné contenant les informations de toute la simulation
{
	char cartesJoueur[21];		// 21 correspond au carte max qu'un joueur pourra piocher (dans le cas où il ne pioche que des as) 
	int totalJoueur;
	char cartesBanque[21];
	int totalBanque;
	int mise;
	int gain;
	int nbJetons;
	int blackjackJ;
	int blackjackB;
	struct infojoueurs *suiv;
};
typedef struct infojoueurs* INFOJOUEURS;

typedef struct tube			// Structure implémentant un pipe
{
	int tube[2];
} TUBE;