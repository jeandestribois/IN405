#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int compteur;

void mon_signal(int signal)
{
	printf("debug: %d\n",compteur);
}

int main(int argc, char const *argv[])
{
	struct sigaction sig;
	sig.sa_handler=&mon_signal;
	sig.sa_flags=SA_ONSTACK;
	pid_t pid_fils;
	sigaction(SIGUSR1,&sig,NULL);
	pid_fils=fork();
	if(pid_fils)
	{
		sleep(3);
		kill(pid_fils,SIGUSR1);
		sleep(2);
		kill(pid_fils,SIGUSR1);
		sleep(3);
		kill(pid_fils,SIGUSR1);
	}
	else
	{
		printf("Fils : compte 12 secondes :\n");
		for(compteur=1; compteur<13; compteur++)
		{
			printf("%d..\n",compteur);
			sleep(1);
		}
	}


	return 0;
}