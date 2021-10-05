//Generales y fifo
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//procesos hijos
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

//Variables globales
int fd = -1;
pid_t pid;
int asientos[] = {1, 2, 3, 4, 5};
char bufres[] = "1,2,3,4,5";
char confirm[] = "Vuelo y asientos reservados correctamente";
char asientooc[] = "El asiento esta ocupado";
char vuelolleno[] = "El vuelo esta lleno";
int bufc, N_vuelo = 0;

int binarySearch(int arr[], int l, int r, int x)
{
    while (l <= r)
    {
        int m = l + (r - l) / 2;
        int dato = 0;
        // Check if x is present at mid
        if (arr[m] == x)
        { //Se verifica si el valor buscado esta en la m-esima posicion y regresa el dato
            dato = x;
            arr[m] = 0;
            return dato;
        }
        // If x greater, ignore left half

        if (arr[m] < x) //Se verifica si el valor buscado es mayor al valor en la m-esima posicion
            l = m + 1;  //Se delimita la parte izquierda del nuevo arreglo a analizar, para descartar los valores en donde no se encuentra el valor buscado
        // If x is smaller, ignore right half
        else           //Se verifica si el valor buscado es menor al valor en la m-esima posicion
            r = m - 1; //Se delimita la parte derecha del nuevo arreglo a analizar, para descartar los valores en donde no se encuentra el valor buscado
    }
    // if we reach here, then element was
    // not present
    return -1;
}

void piper(int fd) //lectura de vuelo
{
    int buf[10000];
    int n;
    fd = open("/tmp/mi_fifo", O_RDONLY);

    n = read(fd, &buf[0], sizeof(int));

    //printf("Bites : %d\n", n);
    printf("Cliente: %d\n", buf[0]);
    switch (buf[0])
    {
    case 1:
        printf("\n1: Vuelo a Cancun, viaje redondo\n");
        N_vuelo = 1;
        break;
    case 2:
        printf("\n2: Vuelo a Yucatan, viaje redondo\n");
        N_vuelo = 2;
        break;
    case 3:
        printf("\n3: Vuelo a Sinaloa, viaje redondo\n");
        N_vuelo = 3;
        break;
    default:
        printf("\nError en vuelo\n");
        break;
    }

    close(fd);
}

void pipera(int fd) //Lectura de la reserva de asientos el 5 es el tamaño de los asientos
{
    int buf[10000];
    int reserva[10000];
    int n;
    fd = open("/tmp/mi_fifo", O_RDONLY);
    for (int i = 0; i < 5; i++)
    {
        n = read(fd, &buf[i], sizeof(int));
        //printf("Asiento del Cliente: %d\n", buf[i]);
        reserva[i] = binarySearch(asientos, 0, 5, buf[i]);
        if (reserva[i] != -1)
        {
            printf("Asiento del Cliente: %d\n", reserva[i]);
        }
    }
    //printf("Bites : %d\n", n);
    for (int i = 0; i < 5; i++)
    {
        printf("\nAsiento disponible %d\n", asientos[i]);
    }

    close(fd);
}

void pipewc(int fd) //Pipe de confirmacion
{
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, confirm, sizeof(confirm));

    //printf("%s\n", confirm);

    close(fd);
}

void pipew(int fd, int buf[]) //Pipe de envio de asientos
{
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    for (int i = 0; i < 5; i++)
    {
        write(fd, &buf[i], sizeof(int));
    }

    close(fd);
}

void pipemenur(int fd)
{
    int n;
    fd = open("/tmp/mi_fifo", O_RDONLY);

    n = read(fd, &bufc, sizeof(int));

    //printf("Bites : %d\n", n);
    printf("Confirmacion: %d\n", bufc);
}

int main(void)
{

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

            piper(fd);           //tuberia de lectura de vuelo
            pipew(fd, asientos); //tuberia de envio de asientos disponibles
            pipemenur(fd);       //Tuberia de confirmacion de compra de vuelos
            if (bufc == 1)
            {
                pipera(fd); //tuberia de reserva de asientos
                pipewc(fd); //Tuberia de confirmacion
            }
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