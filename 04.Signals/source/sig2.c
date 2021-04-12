#include <stdio.h>
#include <signal.h>

void SigUserHandler(int signo) {
	if(signo == SIGUSR1) {
		printf("Received a SIGUSR1 signal\n");
	}
	else if(signo == SIGUSR2) {
		printf("Received a SIGUSR2 signal\n");
	}
	else {
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

int main () {
	if(signal(SIGUSR1, SigUserHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}
	if(signal(SIGUSR2, SigUserHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}
	for(; ; )
		pause();
}

