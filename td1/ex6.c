#include <stdlib.h>
#include <stdio.h>

int main(){
	FILE* fichier = NULL;
	fichier = fopen("ex3.sh","r");
	if(!fichier) exit(-1);
	char cmd[100];
	system("cd");
	fgets(cmd, 100, fichier);
	fgets(cmd, 100, fichier);
	while(fgets(cmd, 100, fichier)) system(cmd);
	fclose(fichier);
	exit(0);
}