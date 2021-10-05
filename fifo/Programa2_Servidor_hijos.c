//Generales y fifo
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//procesos hijos

void piper(int fd)
{
    char buf[512];
    int n;
    fd = open("/tmp/mi_fifo", O_RDONLY);

    n = read(fd, buf, sizeof(buf));

    printf("Bites : %d\n", n);
    printf("mensaje: %s\n", buf);

    close(fd);
}

int main(void)
{
    //Variables del main
    int fd;
    piper(fd);

    return 0;
}