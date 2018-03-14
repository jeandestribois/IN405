#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	
	srand(time(NULL));
	int fils = 0;
	pid_t cpid;
	for(int i=0; i<10; i++) {
		cpid = fork();
		if(!cpid) {
			fils = 1;
			break;
		}
	}
	if(fils){
		int waitingTime = 1+rand()%10;
		sleep(waitingTime);
		pid_t pid = getpid();
		printf("My PID : %d\twaited : %d\n", pid, waitingTime);
		exit(EXIT_SUCCESS);
	}
	else{
		for(int i=0; i<10; i++){
			pid_t pid = wait(NULL);
			printf("%d terminated.\n", pid);
		}
	}

	return EXIT_SUCCESS;
}
