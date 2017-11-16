#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int n;
int fd;
pid_t arr[3];
pid_t next_pid;

clock_t begin_time, end_time;

char path[256];

void sig_fn();

char get_status(char str[256]);

void my_exiter() { exit(0); }

int main(int argc, char *argv[])
{
    int isExist;
    int a;

    if (signal(SIGALRM, sig_fn) == SIG_ERR)
    {
        perror("signal error!\n");
    }
    if (signal(SIGQUIT, my_exiter) == SIG_ERR) {
        perror("signal error!\n");
    }

    if (argc <= 1)
    {
        printf("need more arguments!\n");
        exit(0);
    }

    n = atoi(argv[1]); //assign the number
    strcpy(path, argv[2]);

    if (n <= 0)
    {
        printf("error! The nubmer should be larger than 0!\n");
        exit(0);
    }

    //the file exist
    char check[256];
    if((fd = open(argv[2], O_RDWR)) < 0){
        fd = open(argv[2], O_RDWR | O_CREAT, 0644);
        pwrite(fd, (void*)"0                  ", 20, 0);

    }
    else{
        pread(fd,check,20,0);
        int check_ = atoi(check);
        // printf("%d\n",check_);
        if(check_ != 0){
            close(fd);
            fd = open(argv[2], O_RDWR | O_TRUNC, 0644);
            pwrite(fd, (void*)"0                  ", 20, 0);

        }

    }

    pwrite(fd, (void*)"0                  ", 20, 0);

    arr[0] = getpid();
    if ((arr[1] = fork()) == 0) {
        if ((arr[2] = fork()) == 0) {
            next_pid = arr[0];
            printf("process: (%d) -> (%d)\n", getpid(), next_pid);
            begin_time = clock();
            kill(next_pid, SIGALRM);
            for(;;) pause();
        } else {
            next_pid = arr[2];
            printf("process: (%d) -> (%d)\n", getpid(), next_pid);
            for(;;) pause();
        }
    } else { // Parent
        next_pid = arr[1];
        printf("process: (%d) -> (%d)\n", getpid(), next_pid);
        for(;;) pause();
    }

    return 0;
}

void sig_fn()
{
    char buffer[256];
    pread(fd, buffer, 20, 0);
    int now = atoi(buffer);

    if(now >= n) {
        end_time = clock();
        printf("time: %f\n", (double)(end_time - begin_time) / CLOCKS_PER_SEC);
        kill(arr[0], SIGQUIT);
        exit(1);
    }

    sprintf(buffer, "%d             ", now + 1);
    pwrite(fd, buffer, 20, 0);

    kill(next_pid, SIGALRM);
}
