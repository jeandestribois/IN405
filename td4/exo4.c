#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/wait.h>

// Création d'un processus fils qui effectue 
// une boucle et utilsation de times() par le père.

int main(int argc, char const *argv[])
{
	struct tms buf;
	times(&buf);
	int status;
	if(fork())
	{
		wait(&status);
		printf("\nTemps d'execution de mon fils : %ld\n",buf.tms_cutime);
	}
	else
	{
		for(int i=0; i<100; i++) printf("."); 
	}
	return 0;
}