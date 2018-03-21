#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>



int signal()
{

}

int main(int argc, char const *argv[])
{
	pid_t fils;
	sigaction(SIGUSER1,SA_ONSTACK)
	if((fils=fork()))
	{

	}
	else
	{
		printf("Fils : compte 12 secondes :\n");
		for(int i=1; i<13; i++)
		{
			printf("%d..\n",i);
			sleep(1);
		}
	}


	return 0;
}