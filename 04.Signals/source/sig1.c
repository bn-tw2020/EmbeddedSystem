#include <stdio.h>
#include <signal.h>

void SigIntHandler(int signo) {
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");
	exit(0);
}

int main() {
	signal(SIGINT, SigIntHandler);
	printf("Press ^C to equit\n");

	for(; ; )
		pause();
}
