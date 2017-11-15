#include <stdio.h>

int main(){
	FILE *fp;
	int status;
	char path[256];

	fp = popen("cat /proc/4698/status","r");
	if(fp==NULL){
		perror("ssibla");
	}
	// while(fgets(path,256,fp)!= NULL){
	// 	printf("%s", path);
	// }
	for(int i=0;i <3; ++i){
		fgets(path,256,fp);
	}
	printf("%s\n", path);
	printf("%c\n",path[7]);
	if(path[7] =='S') printf("ssibal\n");
	else if (path[7] == 's') printf("gaesaekki\n");
	status = pclose(fp);

	return 0;
}