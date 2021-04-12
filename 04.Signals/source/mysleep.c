#include <stdio.h>
#include <signal.h>

void SigAlarmHandler(int signo) {
	/* nothing to do, just return to wake up the pause */
	return;
}

unsigned int mysleep(unsigned int nsecs) {
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return nsecs;
	}

	alarm(nsecs);
	pause();
	return alarm(0);
}

int main() {
	printf("Wait for 5 seconds...\n");
	mysleep(5);
}
