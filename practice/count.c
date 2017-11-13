#include <stdio.h>
#include <signal.h>
#include <unistd.h>


int main(int argc, char* argv[]){
	FILE * fp;

	int isExist;
	int num;

	if (argc <= 1){
		printf("need arguments!\n");
	}
	else if (argv[1] <= 0){
		printf("error! The number should be larger than 0!\n");
	}
	isExist = access(argv[2],F_OK);

	if(isExist == 0){
		fp = fopen(argv[2],"rw");
		fscanf(fp,"%d", &num);
		printf("num; %d\n", num);
		if(num != 0){
			freopen(argv[2], "wb", fp);
			fprintf(fp, "%d", 0);
		}
	}
	else{
		fp = fopen(argv[2],"w+");
		fprintf(fp, "%d", 0);
	}

	return 0;
}

