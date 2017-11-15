#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

FILE* fp;
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
		fp = fopen(argv[2],"r+");
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
	if ((arr[1] = fork())==0){
		pause();
		kill(arr[0],SIGALRM);

	}


	if(getpid() == arr[0]){
		if((arr[2] = fork()) ==0){
			pause();
			kill(arr[1],SIGALRM);
		}
	}

	if(getpid() == arr[0]){
		alarm(1);
	}

	while((*pt) < n){
		if(getpid() == arr[0]){
			printf("p1\n");
			pause();
			kill(arr[2], SIGALRM);
		}

		else if(getppid() == arr[0] && arr[1] == 0){
			printf("p2\n");

			pause();
			kill(arr[0],SIGALRM);
		}

		else{
			printf("p3\n");
			pause();
			kill(arr[1],SIGALRM);
		}

	}

	return 0;
}


void sig_fn(){
	int i;
	fseek(fp,0,SEEK_SET);
	fscanf(fp,"%d",&i);
	if (i < n){
		fseek(fp,0,SEEK_SET);
		i++;
		fprintf(fp,"%d", i);
	}

}