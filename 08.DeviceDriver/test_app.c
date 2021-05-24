#include <stdio.h> // for perror()
#include <fcntl.h> // for file
#include <unistd.h> // for open(), read(), write(), close()
#include <sys/ioctl.h> // for ioctl()

int main() {
    int fd;
    fd = open("/dev/testdd", O_RDWR);
    if(fd < 3) {
        perror("/dev/testdd");
        return -1;
    }
    read(fd, NULL, 0);
    write(fd, NULL, 0);
    ioctl(fd, 0, 0);
    close(fd);

    return 0;
}