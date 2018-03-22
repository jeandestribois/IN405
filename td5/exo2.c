#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

struct message
{
	int *t;
	int debut;
	int fin;
	int resultat;
};

void* routine_somme(void* args)
{
	struct message *mes=(struct message*)args;
	mes->resultat=0;
	for(int i=mes->debut; i<=mes->fin; i++)
	{
		mes->resultat+=mes->t[i];
	}
	pthread_exit(NULL);

}

void* routine_moyenne(void* args)
{
	
}

void* routine_max(void* args)
{
	
}

void* routine_min(void* args)
{
	
}

int main(int argc, char const *argv[])
{
	if (argc!=4)
	{
		fprintf(stderr,"Correct usage : %s <nbre threads> <taille tableau> <opcode>\nopcode : + ou / ou M ou m\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	int *t;
	int taille=atoi(argv[2]);
	int nbre_thread=atoi(argv[1]);

	if(taille<nbre_thread) nbre_thread=taille;

	t=malloc(sizeof(int)*taille);

	srand(time(NULL));
	for(int i=0; i<taille; i++)
	{
		t[i]=1+rand()%101;
		printf("Indice %d du tableau : %d\n",i,t[i]);
	}

	struct message *mes;
	mes=malloc(sizeof(struct message)*nbre_thread);

	pthread_t *thread;
	thread=malloc(sizeof(pthread_t)*(nbre_thread));

	mes[0].t=t;
	mes[0].debut=0;
	mes[0].fin=1+taille/nbre_thread;
	for(int i=1; i<nbre_thread-1; i++)
	{
		mes[i].t=t;
		mes[i].debut=mes[i-1].fin+1;
		mes[i].fin=mes[i].debut+(1+taille/nbre_thread);
	}
	mes[nbre_thread-1].t=t;
	mes[nbre_thread-1].debut=mes[nbre_thread-2].fin+1;
	mes[nbre_thread-1].fin=taille-1;

	if(argv[3][0]=='+')
	{
		int resultat_total=0;
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_create(&thread[i],NULL,routine_somme,(void*)&mes[i]);
		}
		for(int i=0; i<nbre_thread; i++)
		{
			resultat_total+=mes[i].resultat;
		}
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_join(thread[i],NULL);
		}
		printf("La somme total du tableau est : %d\n",resultat_total);
	}
	else if (argv[3][0]=='/')
	{
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_create(&thread[i],NULL,routine_moyenne,(void*)&mes[i]);
		}
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_join(thread[i],NULL);
		}	
	}
	else if (argv[3][0]=='M')
	{
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_create(&thread[i],NULL,routine_max,(void*)&mes[i]);
		}
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_join(thread[i],NULL);
		}
	}
	else if (argv[3][0]=='n')
	{
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_create(&thread[i],NULL,routine_min,(void*)&mes[i]);
		}
		for(int i=0; i<nbre_thread; i++)
		{
			pthread_join(thread[i],NULL);
		}
	}
	else
	{
		fprintf(stderr,"Wrong opcode argument : %s\n",argv[3]);
		exit(EXIT_FAILURE);
	}





	return 0;
}