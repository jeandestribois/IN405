#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>


void* routine_thread1()
{
	printf("Hello world\n");
	pthread_exit(NULL);
}

void* routine_thread2(void* args)
{
	int alea=(int)args;
	printf("Entier aleatoire généré par le processus principal : %d\n",alea);
	pthread_exit(NULL);
}

void* routine_thread3(void* args)
{
	int* alea=(int*)args;
	*alea=rand();
	pthread_exit(NULL);
}

void* routine_thread4(void* args)
{
	int* talea=(int*)args;
	for(int i=0; i<5; i++)
	{
		printf("Indice %d du tableau 1 : %d\n",i,talea[i]);
	}
	pthread_exit(NULL);
}

void* routine_thread5(void* args)
{
	int* talea=(int*)args;
	for(int i=1; i<talea[0]; i++)
	{
		printf("Indice %d du tableau 2 : %d\n",i,talea[i]);
	}
	pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
	if(argc!=2)
	{
		fprintf(stderr,"Correct usage : %s <un nombre>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int n=atoi(argv[1]);

	pthread_t thread1, thread2, thread3, thread4, thread5;
	int alea1, alea2, talea1[5], *talea2;
	srand(time(NULL));
	alea1=rand();
	alea2=0;

	for(int i=0; i<5; i++)
	{
		talea1[i]=rand();
	}

	talea2=malloc((sizeof(int))*(n+1));
	talea2[0]=n;	// Je stock la taille de tableau dans la case d'indice 0 pour la retrouver dans le thread
	for(int i=1; i<n; i++)
	{
		talea2[i]=rand();
	}

	pthread_create(&thread1,NULL,routine_thread1,NULL);
	pthread_create(&thread2,NULL,routine_thread2,(void*)alea1);
	pthread_create(&thread3,NULL,routine_thread3,(void*)&alea2);
	pthread_create(&thread4,NULL,routine_thread4,(void*)talea1);
	pthread_create(&thread5,NULL,routine_thread5,(void*)talea2);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);

	printf("Entier aleatoire genere par le thread 3 : %d\n",alea2);

	free(talea2);

	return 0;
}