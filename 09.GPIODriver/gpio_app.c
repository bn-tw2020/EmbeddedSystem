#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

int dev, value;
void SIGINT_handler();

int main() {
    signal(SIGINT, SIGINT_handler);

    dev = open("/dev/gpio_dev", O_RDWR);
    if(dev > 0) printf("open successful! \n");
    else {
        perror("open()\n");
        return -1;
    }

    while(1) {
        read(dev, &value, sizeof(value));
        printf("[gpio_app] button data : %d\n", value);
        sleep(5);
    }
    return 0;
}

void SIGINT_handler() {
    close(dev);
    exit(0);
}