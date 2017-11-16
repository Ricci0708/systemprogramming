#include <stdio.h>

int main(){
	FILE* fp;

	fp = fopen("./sample.txt","w");
	fprintf(fp,"%d",7);
	return 0;
}