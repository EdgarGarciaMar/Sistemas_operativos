//Generales y fifo
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//procesos hijos
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

void piper(int fd)
{
    char buf[10000];
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
    int fd=-1;
    pid_t pid;
    //Recepcion de pipes
    while (fd == -1)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("\nError al crear el proceso\n");
            exit(-1);
        }
        if (pid == 0)
        {
            printf("\nHola soy el proceso hijo\n");
            printf("\nMi identificador es: %d\n", getpid());
            printf("\nMi proceso padre es: %d\n", getppid());
            piper(fd);
            sleep(10);
            //exit(0);
        }
        else
        {
            wait(NULL);
            printf("\n\t\t\tHola soy el proceso padre\n");
            printf("\n\t\t\tMi identificador es: %d\n", getpid());
            printf("\n\t\t\tMi proceso padre es: %d\n", getppid());
            //piper(fd);
        }
        printf("\n\t\t\tHola soy el proceso padre fuera del if\n");
        printf("\n\t\t\tMi identificador es: %d\n", getpid());
        printf("\n\t\t\tMi proceso padre es: %d\n", getppid());
    }
        printf("\nFin\n");
        return 0;
}