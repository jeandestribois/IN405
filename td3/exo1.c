#include <stdlib.h>
#include <stdio.h>

#include <sys/stat.h>
#include <time.h>




int main(int argc, char *argv[])
{
	struct stat sb;
	if(argc != 2)
	{
		fprintf(stderr,"le nombre d'argument saisi est incorrecte\n");
		exit(EXIT_FAILURE);
	}
	if(stat(argv[1], &sb) == -1)
	{
		fprintf(stderr,"la lecture des elements du fichier a échoué\n");
		exit(EXIT_FAILURE);
	}
	printf("File type:                ");
	
	switch (sb.st_mode & S_IFMT)
	{
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}
	
	char rwx[9];
	rwx[8]=(sb.st_mode & 1) ? 'x' : '-';
	rwx[7]=(sb.st_mode & 2) ? 'w' : '-';
	rwx[6]=(sb.st_mode & 4) ? 'r' : '-';
	rwx[5]=(sb.st_mode & 8) ? 'x' : '-';
	rwx[4]=(sb.st_mode & 16) ? 'w' : '-';
	rwx[3]=(sb.st_mode & 32) ? 'r' : '-';
	rwx[2]=(sb.st_mode & 64) ? 'x' : '-';
	rwx[1]=(sb.st_mode & 128) ? 'w' : '-';
	rwx[0]=(sb.st_mode & 256) ? 'r' : '-';
	printf("Access permissions:	  %s \n",rwx);
	printf("User ID of owner:	  %ld \ns",(long) sb.st_uid);
	printf("File size:		  %lld bytes \n",(long long) sb.st_size);
	
	
	
	
	exit(EXIT_SUCCESS);
}
