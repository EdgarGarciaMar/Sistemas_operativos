//Generales y fifo
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
//Hilos
#include <pthread.h>
//Semaforo
#include <semaphore.h>

//Variables globales
int fd = -1;
int asientos[] = {1, 2, 3, 4, 5};
int asientos2[] = {1, 2, 3, 4, 5};
int asientos3[] = {1, 2, 3, 4, 5};
char confirm[] = "Vuelo y asientos reservados correctamente";
int bufc, N_vuelo = 0;
int buf[10];
pthread_t thread;
sem_t semaforo;

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

    fd = open("/tmp/mi_fifo", O_RDONLY);

    read(fd, &buf[0], sizeof(int));

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

void pipera(int fd, int asientos[]) //Lectura de la reserva de asientos el 5 es el tamaño de los asientos
{
    int reserva[5];
    fd = open("/tmp/mi_fifo", O_RDONLY);
    for (int i = 0; i < 5; i++)
    {
        read(fd, &buf[i], sizeof(int));
        //printf("Asiento del Cliente: %d\n", buf[i]);
        reserva[i] = binarySearch(asientos, 0, 5, buf[i]);
        if (reserva[i] != -1)
        {
            printf("Asiento del Cliente: %d\n", reserva[i]);
        }
    }
    
    for (int i = 0; i < 5; i++)
    {
        printf("\nAsiento disponible %d\n", asientos[i]);
    }

    close(fd);
}

void pipewc(int fd) //Pipe de confirmación que indica el estatus de la reserva
{
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, confirm, sizeof(confirm));

    //printf("%s\n", confirm);

    close(fd);
}

void pipew(int fd, int buf[]) //Pipe de envío de asientos
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

void *Codigo_Hilo1()
{
    while (1)
    {
        piper(fd); //tubería de lectura de vuelo
        if (N_vuelo == 1)
        {
            pipew(fd, asientos); //tubería de envío de asientos disponibles
            printf("\n1N_vuelo\n");
        }
        if (N_vuelo == 2)
        {
            pipew(fd, asientos2); //tubería de envío de asientos disponibles
            printf("\n2N_vuelo\n");
        }
        if (N_vuelo == 3)
        {
            pipew(fd, asientos3); //tubería de envío de asientos disponibles
            printf("\n3N_vuelo\n");
        }

        pipemenur(fd); //Tubería de confirmación de compra de vuelos
        if (bufc == 1)
        {
            //pipera(fd); //tubería de reserva de asientos
            if (N_vuelo == 1)
            {
                sem_wait(&semaforo);  //Semaforo que da orden a las reservas de multiples clientes
                pipera(fd, asientos); //tubería de reserva de asientos
                break;
            }
            if (N_vuelo == 2)
            {
                sem_wait(&semaforo);   //Semaforo que da orden a las reservas de multiples clientes
                pipera(fd, asientos2); //tubería de reserva de asientos
                break;
            }
            if (N_vuelo == 3)
            {
                sem_wait(&semaforo);   //Semaforo que da orden a las reservas de multiples clientes
                pipera(fd, asientos3); //tubería de reserva de asientos
                break;
            }
            //pipewc(fd); //Tubería de confirmación
        }
    }
    pipewc(fd);          //Tubería de confirmación
    sem_post(&semaforo); //Cuando se termine la operacion de reserva estara disponible para otro hilo
    pthread_exit(NULL);
}
void PIPE_CONEXION(int fd)
{
    int conexion;
    fd = open("/tmp/mi_fifo", O_RDONLY);

    read(fd, &conexion, sizeof(int));

    close(fd);
}
int main(void)
{
    //gcc Programa2_Servidor_hilos.c -o servidorhilos -lpthread compilar
    sem_init(&semaforo, 0, 1); //Semaforo se inicializa en 1
    while (1)
    {
        //Tubería de conexión
        printf("\nEsperando conexion.... \n");
        PIPE_CONEXION(fd);
        int estado = pthread_create(&thread, NULL, Codigo_Hilo1, NULL);
        if (estado != 0)
        {
            perror("\nEl thread no pudo crearse\n");
            exit(-1);
        }
        else
        {
            printf("\nHilo creado, Conexion establecida \n");
        }
        sleep(10);
    }

    return 0;
}