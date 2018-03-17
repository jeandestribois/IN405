#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	pid_t fils;
	fils=fork();
	if(fils)
	{
		sleep(3);
		kill(fils, SIGSTOP);
		printf("Pere : met en pause son fils pendant 5 secondes\n");
		sleep(5);
		kill(fils, SIGCONT);
		wait(NULL);
	}
	else
	{
		printf("Fils : compte 5 secondes :\n");
		for(int i=1; i<6; i++)
		{
			printf("%d..\n",i);
			sleep(1);
		}
	}
	
	return 0;
}