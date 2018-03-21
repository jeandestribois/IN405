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
	int* alea=(int*)args;
	printf("Entier aleatoire généré : %d\n",*alea);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	pthread_t thread1, thread2, thread3, thread4, thread5;
	int alea;
	srand(time(NULL));
	alea=rand();

	pthread_create(&thread1,NULL,routine_thread1,NULL);
	pthread_create(&thread2,NULL,routine_thread2,(void*)&alea);


	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);



	return 0;
}