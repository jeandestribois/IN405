all: main #main-opt

main: main.o se_fichier.o
	gcc -Wall main.o se_fichier.o -o main
	
main.o: main.c se_fichier.h
	gcc -c -Wall main.c
	
main-opt: main-opt.o se_fichier.o
	gcc -Wall main.o se_fichier.o -o main-opt
	
main-opt.o: main-opt.c se_fichier.h
	gcc -c -Wall main-opt.c
	
se_fichier.o: se_fichier.c
	gcc -c -Wall se_fichier.c
	
clean:
	rm *.o
	rm main
	rm main-opt
	
