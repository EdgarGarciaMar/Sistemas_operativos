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

//variables globales
int fd = 0;
pthread_t thread;
sem_t semaforo;
char conexion[5];
char contra[] = "12345";
int id;

int validarContrasena(char conexion[], char contra[])
{
    printf("Validando contrasena....\n");
    int validacion = 0;
    validacion = strcmp(conexion, contra);

    if (validacion == 0)
    {
        printf("Contrasena valida.\n");
    }
    else
    {
        printf("Contrasena incorrecta\n");
    }
    return validacion;
}

void PIPE_CONEXION(int fd)
{
    //Recepción de información del usuario
    fd = open("/tmp/mi_fifo", O_RDONLY);

    read(fd, &conexion, sizeof(conexion));

    close(fd);

    //Procesamiento y respuesta de la conexion
    int identificadorConexion = validarContrasena(conexion, contra);

    if (identificadorConexion == 0)
    {
        //Le confirmamos al usuario que su contraseña es correcta
        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        write(fd, &identificadorConexion, sizeof(int));

        close(fd);

        //Recibimos su id
        fd = open("/tmp/mi_fifo", O_RDONLY);

        read(fd, &id, sizeof(int));

        close(fd);

        printf("El id de la conexion es: %d\n", id);
    }
    else
    {
        //le confirmamos que la contraseña esta mal
        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        write(fd, &identificadorConexion, sizeof(int));

        close(fd);
    }
}

int main(void)
{
    while (1)
    {

        printf("\t\t***Proceso de servidor Tienda***\n");
        printf("\tEsperando una conexion....\n");
        PIPE_CONEXION(fd);
    }
    return 0;
}