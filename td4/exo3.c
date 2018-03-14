#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void q1(int m, int n){

	//Le père crée m fois n processus
	int fils = 0;
	int count = 0;

	for(int mi = 0; mi < m; mi++){

		for(int ni = 0; ni < n; ni++){

			if(fork()){
				//pere
				count++;
			}
			else{
				//fils
				fils = 1;
				break;
			}

		}

		if(fils) break;
		else printf("Le père vient de crée %d processus fils.\n", n);

	}

	if(!fils) printf("Au total le père a crée %d processus fils.\n", count);

}

void q2(int m, int n){

	//Le père crée m processus fils
	//Chaque processus fils crée n processus petit-fils

	int fils = 0, pfils = 0;
	int count = 0;

	for(int mi = 0; mi < m; mi++){
		if(fork()){
			//père
			count += n+1;
		}
		else{
			//fils
			fils = 1;
			for(int ni = 0; ni < n; ni++){
				if(fork()){
					//fils
				}
				else
				{
					//petit-fils
					pfils = 1;
					break;
				}
			}
			if(!pfils) printf("Le processus fils %d vient de crée %d processus petit-fils.\n", mi, n);
			break;
		}

	}

	if(!fils & !pfils) {
		printf("Le processus père vient de crée %d processus fils.\n", m);
		printf("Au total %d processus on été crées.\n", count);
	}

}

void q3(int m, int n){

	//Le père crée n processus fils,
	//puis chaque processus fils crée n processus petit-fils,
	//ainsi de suite m fois.

	int gen = 0;

	for(int ni = 0; ni < n; ni++){
		if(fork()){}
		else{
			gen++;
			if(gen<m) ni = -1;
			else break;
		}
	}
	if(gen<m) printf("Generation %d : %d processus crée.\n", gen, n);

}

int main(int argc, char** argv){

	switch(atoi(argv[3])){
		case 1: q1(atoi(argv[1]), atoi(argv[2])); break;
		case 2: q2(atoi(argv[1]), atoi(argv[2])); break;
		case 3: q3(atoi(argv[1]), atoi(argv[2])); break;
		default: printf("Correct Usage : %s m n question\n", argv[0]); return EXIT_FAILURE;
	}	

	return EXIT_SUCCESS;
}
