#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

FILE* fp;
int* pt;
pid_t arr[3];
int n;

void sig_fn();

int main(int argc, char* argv[]){
	int num;
	pt = &num;

	int isExist;
	n =  atoi(argv[1]);

	if(signal(SIGALRM, sig_fn)== SIG_ERR){
		perror("sssibla\n");
	}

	if (argc <= 1){
		printf("need arguments!\n");
		exit(0);
	}
	else if (n <= 0){
		printf("error! The number should be larger than 0!\n");
		exit(0);
	}
	isExist = access(argv[2],F_OK);

	if(isExist == 0){
		fp = fopen(argv[2],"rw");
		int check;
		fread(&check,sizeof(int),1,fp);

		fscanf(fp,"%d",&check);
		if(check != 0){
			freopen(argv[2], "wb+", fp);
			fprintf(fp, "%d", 0);
		}
	}
	else{
		fp = fopen(argv[2],"w+");
		fprintf(fp, "%d", 0);
	}

	arr[0] = getpid();
	arr[1] = fork();

	if(getpid() == arr[0]){
		arr[2] = fork();
	}

	if(getpid() == arr[0]){
		alarm(1);
	}

	while((*pt) < n){
		if(getpid() == arr[0]){
			pause();
//			printf("p1: %d\n",num);
			kill(arr[2], SIGALRM);
		}
		if(getppid() == arr[0] && arr[1] == 0){
			pause();
//			printf("p2: %d\n",num);

			kill(getppid(),SIGALRM);
		}
		if(getppid() == arr[0] && arr[1] != 0){
			pause();
			// printf("p3: %d\n",num);

			kill(arr[1],SIGALRM);
		}

	}


	return 0;
}


void sig_fn(){
	fseek(fp,0,SEEK_SET);
	fscanf(fp,"%d",&(*pt));
	if ((*pt) < n){
		fseek(fp,0,SEEK_SET);
		(*pt)++;
		fprintf(fp,"%d", (*pt));
	}

}