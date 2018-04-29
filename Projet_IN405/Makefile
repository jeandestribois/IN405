all: clean build test

# Test avec un niveau

test:
	./blackjack lv1.blackjack

# Edition de lien

build: blackjack.o lire_ecrire.o simuler.o fonctions_liste.o deck.o
	gcc -Wall blackjack.o lire_ecrire.o simuler.o fonctions_liste.o deck.o -o blackjack

# Compilation

blackjack.o: blackjack.c lire_ecrire.h simuler.h fonctions_liste.h deck.h mes_types.h
	gcc -c -Wall blackjack.c

lire_ecrire.o: lire_ecrire.c
	gcc -c -Wall lire_ecrire.c

simuler.o: simuler.c
	gcc -c -Wall simuler.c

fonctions_liste.o: fonctions_liste.c
	gcc -c -Wall fonctions_liste.c

deck.o: deck.c
	gcc -c -Wall deck.c

# Clean

clean:
	rm -f blackjack
	rm -f *.o
	rm -f joueur_n*

# Edition

edit:
	subl Makefile *.c *.h &

resultat:
	subl joueur_n* &