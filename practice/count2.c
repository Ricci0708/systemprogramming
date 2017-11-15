#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

FILE* fp;
FILE* ch;

int n;
pid_t arr[3];

char path[256];

char path_0[256];
char path_1[256];
char path_2[256];

char buf[10];
// char r[256];

void sig_fn();

char get_status(char str[256]);

int main(int argc, char* argv[]){
	int isExist;
	int a;

	if(signal(SIGALRM, sig_fn) == SIG_ERR){
		perror("siganl error!\n");
	}

	if(argc<= 1){
		printf("need more arguments!\n");
		exit(0);
	}

	n = atoi(argv[1]); //assign the number
	strcpy(path, argv[2]);

	if(n <= 0){
		printf("error! The nubmer should be larger than 0!\n");
		exit(0);
	}
	isExist = access(argv[2], F_OK);

	//the file exist
	if(isExist == 0){
		fp = fopen(argv[2],"r+");
		int check;
		fscanf(fp,"%d",&check);

		if(check != 0){
			freopen(argv[2], "wb+", fp);
			fprintf(fp, "%d", 0);
		}
	}
	//file Not exist
	else{
		fp = fopen(argv[2], "w+");
		fprintf(fp,"%d", 0);
	}
	arr[0] = getpid();


	if((arr[1] = fork()) == 0){
		//it is child process.
		arr[1] = getpid();
		if((arr[2] = fork()) == 0){
			arr[2] = getpid();
		}
	}

	sprintf(path_0,"ps h -eo s,pid | grep %d",arr[0]);
	sprintf(path_1,"ps h -eo s,pid | grep %d",arr[1]);
	sprintf(path_2,"ps h -eo s,pid | grep %d",arr[2]);

	// for(int i =0; i< 3; ++i){
	// 	printf("pid%d ; %d\n",i,arr[i]);
	// }

	if(getpid() == arr[0]){
		alarm(1);
	}

	while(1){
		rewind(fp);
		fscanf(fp,"%d",&a);
		printf("a ; %d\n", a);
		if(a >= n){
			break;
		}
		else{
			if(getpid() == arr[0]){
				// printf("p1\n");

				pause();
				while(1){
					printf("p1\n");
					if((ch = popen(path_1,"r") != NULL)){
						kill(arr[1],SIGALRM);
						break;
					}
				}
				kill(arr[1],SIGALRM);
			}
			else if(getpid() == arr[1]){
				// printf("p2\n");

				pause();
				while(1){
					printf("p2\n");

					if((ch = popen(path_2,"r") != NULL)){
						kill(arr[2],SIGALRM);
						break;
					}
				}
				kill(arr[2],SIGALRM);
			}
			else{
				// printf("p3\n");
				pause();
				while(1){
					printf("p3\n");

					if((ch = popen(path_0,"r") != NULL)){
						kill(arr[0],SIGALRM);
						break;
					}
				}
				kill(arr[0],SIGALRM);
			}
		}
	}
	return 0;
}

void sig_fn(){
	FILE* fp2;
	printf("sig_fn in\n");

	int i;
	fp2 = fopen(path, "rw+");
	fscanf(fp,"%d", &i);
	printf("num in; %d\n",i);

	if(i < n){
		// freopen(path,"ab",fp2);
		rewind(fp2);
		sprintf(buf, "%d", ++i);
		fwrite(buf,strlen(buf),1,fp2);
		// fprintf(fp2,"%d",++i);
	}
	fsync(fp2);
	fclose(fp2);
	printf("num out; %d\n",i);
	printf("sig_fn out\n");
}
// char get_status(char str[256]){
// 	ch = popen(str, "r");
// 	for(int i=0;i <3; ++i){
// 		fgets(r,256,ch);
// 	}
// 	return(r[7]);
// }