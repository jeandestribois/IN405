all: build

# Edition de lien

build: blackjack.o lire_ecrire.o fonctions_liste.o deck.o
	gcc -Wall blackjack.o lire_ecrire.o fonctions_liste.o deck.o -o blackjack

# Compilation

blackjack.o: blackjack.c lire_ecrire.h fonctions_liste.h mes_types.h deck.h
	gcc -c -Wall blackjack.c

lire_ecrire.o: lire_ecrire.c
	gcc -c -Wall lire_ecrire.c

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