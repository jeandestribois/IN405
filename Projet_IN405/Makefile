all: build

# Edition de lien

build: blackjack.o lire_ecrire.o deck.o
	gcc -Wall blackjack.o lire_ecrire.o deck.o -o blackjack

# Compilation

blackjack.o: blackjack.c lire_ecrire.h mes_types.h deck.h
	gcc -c -Wall blackjack.c

lire_ecrire.o: lire_ecrire.c
	gcc -c -Wall lire_ecrire.c

deck.o: deck.c
	gcc -c -Wall deck.c

# Clean

clean: 
	rm -f blackjack
	rm -f *.o

# Edition

edit:
	subl Makefile *.c *.h &