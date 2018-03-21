#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

// Création d'un processus fils qui effectue 
// une boucle et utilsation de times() par le père.

// PERMET D'UTILISER LA FONCTION rdtsc() POUR AVOIR LE NOMBRE DE CYCLE D'UN PROCESSUS

#ifdef __i386
extern __inline__ uint64_t rdtsc(void) {
  uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}
#elif defined __amd64
extern __inline__ uint64_t rdtsc(void) {
  uint64_t a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif

// Finalement times() c'est de la merde du coup on utilise rdtsc()

int main(int argc, char const *argv[])
{
	uint64_t ticks=0;
	int pipefd[2];
	pipe(pipefd);
	uint64_t N=atoll(argv[1]);
	//struct tms buf;
	double *t, ret;
	t=malloc(sizeof(double)*N);
	int status;


	if(fork())
	{
		wait(&status);
		//times(&buf);
		//printf("\nTemps d'execution de mon fils : %llu\n",(long long unsigned)buf.tms_cutime);


		if(WIFEXITED(status)) ret =WEXITSTATUS(status);
		printf("\nLa valeur de r est : %lf\n",ret);

		close(pipefd[1]);
		read(pipefd[0],&ticks,4);
		close(pipefd[0]);
		printf("\nLe nombre de cycle fait par le fils est : %llu\n",ticks);
	}
	else
	{
		uint64_t start_ticks=0;
		uint64_t stop_ticks=0;

		start_ticks=rdtsc();

		uint64_t r=0;
		memset(t,1,sizeof(double)*N);

		for(uint64_t i=0; i<N; i++) r+=t[i];

		stop_ticks=rdtsc();
		ticks=stop_ticks-start_ticks;
		close(pipefd[0]);
		write(pipefd[1],&ticks,4);
		close(pipefd[1]);

		exit(r);

	}
	return 0;
}