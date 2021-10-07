#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//Variables globales
int fd;
int buf[10000];
char bufc[10000];
int personas, Confirmacion;

void piper(int fd) //Recepción de asientos
{
    //int buf[10000];
    int n;
    fd = open("/tmp/mi_fifo", O_RDONLY);

    for (int i = 0; i < 5; i++)
    {
        n = read(fd, &buf[i], sizeof(int));
        printf("Asiento: %d\n", buf[i]);
    };

    //printf("Bites : %d\n", n);

    close(fd);
}

void pipew(int fd, int buf[]) //Tubería de asignación de asientos
{
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    for (int i = 0; i < personas; i++)
    {
        write(fd, &buf[i], sizeof(int));
    }

    close(fd);
}

void pipewv(int fd, int buf[]) //vuelos
{
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, &buf[0], sizeof(int));

    close(fd);
}

void piperc(int fd) //Pipe de confirmación
{

    fd = open("/tmp/mi_fifo", O_RDONLY);

    read(fd, bufc, sizeof(bufc));

    printf("\nEstatus: %s\n", bufc);

    close(fd);
}

void pipemenu(int Confirmacion)
{
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, &Confirmacion, sizeof(int));

    close(fd);
}

void PIPE_CONEXION(int conexion)
{
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, &conexion, sizeof(int));

    close(fd);
}

void menu()
{
    //Tubería de conexión con server
    PIPE_CONEXION(1);
    //Impresión en consola
    printf("\n\t***Aeropuerto Internacional de la Ciudad de Mexico***\n");
    printf("\n\t***Reservas de vuelos***\n");
    printf("\nOpciones:");
    printf("\n1: Vuelo a Cancun, viaje redondo");
    printf("\n2: Vuelo a Yucatan, viaje redondo");
    printf("\n3: Vuelo a Sinaloa, viaje redondo");
    printf("\n4: Salir");
    printf("\nSeleccione su reservacion al introducir el numero:\n");
    printf("\n Ejemplo: 1\n");
    printf("Seleccion:\n");
    scanf("%d", &buf[0]);
    if (buf[0] == 4)
    {
        exit(0);
    }
    //Tubería de selección de vuelo
    pipewv(fd, buf);

    //Tubería de recepción de asientos
    printf("\nLos asientos disponibles son:\n");
    piper(fd);
    printf("\n");

    //Confirmación de vuelos
    printf("\nPor favor confirme si desea adquirir los boletos (1) para si (2) para no\n");
    scanf("%d", &Confirmacion);

    //pipe de menu de confirmación
    pipemenu(Confirmacion);

    if (Confirmacion == 1)
    {

        //Tubería de reserva de asientos
        printf("Indique el numero de personas para el vuelo:\n");
        scanf("%d", &personas);
        for (int i = 0; i < personas; i++)
        {
            printf("Asiento del Pasajero %d:\n", i);
            scanf("%d", &buf[i]);
        }
        pipew(fd, buf);

        //Tubería de confirmación de estatus de la reserva
        piperc(fd);
    }
    else
    {
        menu();
    }
}

int main(void)
{
    menu();
    return 0;
}
