#include <stdio.h>
#include <signal.h>

void sig_fn();

void HandleSignal(int sig, siginfo_t *si, void * context);

int main(int argc, char* argv[]){
	struct sigaction sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = HandleSignal;

	sigaction(2,&sa,NULL);


	for(;;)
		pause();
}

void sig_fn(){
	printf("\nCtrl-C is pressed. Try Again\n");
}

void HandleSignal(int sig, siginfo_t *si, void* context){
	sig_fn();	
}