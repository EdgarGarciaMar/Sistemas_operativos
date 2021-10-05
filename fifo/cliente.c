#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    //Variables del main
    int fd;
    char buf[] = "Pipe";

    //Impresion en consola
    printf("\n\t***Aeropuerto Internacional de la Ciudad de Mexico***\n");
    printf("\n\t***Reservas de vuelos***\n");
    printf("\nOpciones:");
    printf("\n1: Vuelo a Cancun, viaje redondo");
    printf("\n2: Vuelo a Yucatan, viaje redondo");
    printf("\n3: Vuelo a Sinaloa, viaje redondo");
    printf("\nSeleccione su reservacion al introducir el numero:\n");
    printf("\n Ejemplo: 1\n");
    printf("Seleccion:\n");
    scanf("%s", &buf);

    //Tuberia
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, buf, sizeof(buf));

    close(fd);
    return 0;
}
