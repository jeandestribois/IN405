#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char** argv){

	srand(time(NULL));

	int pipefd[2];
	char buf;
	int status, num;
	pipe(pipefd);
	pid_t childPID = fork();


	if(!childPID) {
		printf("Fils :\n");
		printf("Mon PID est : %d\n", getpid());
		printf("Le PID de mon pere est : %d\n", getppid());

		num = 1+rand()%50;
		printf("num = %d\n", num);

		//
		close(pipefd[0]);
		write(pipefd[1], &num, 1);
		close(pipefd[1]);
		//
	}
	else {
		wait(&status);

		printf("\nPere :\n");
		printf("Mon PID est : %d\n", getpid());
		printf("Le PID de mon fils est : %d\n", childPID);

		//
		close(pipefd[1]);
		read(pipefd[0], &buf, 1);
		close(pipefd[0]);
		//

		printf("num = %d\n", buf);
	}
	return EXIT_SUCCESS;
}
