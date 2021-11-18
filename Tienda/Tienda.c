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

//******variables globales******
//tuberia
int fd = 0;
//hilo
pthread_t thread;
//semaforo
sem_t semaforo;
//variables funcionales
char conexion[5];     //contra a validar
int Usuarioadm = 0;   //usuario a validar
int usuario_provedor; //indicador que habilita opciones para cliente o para proveedor
//usuario admin
char contra[] = "12345";
int usuario = 1;
int id;

//void *opcionesProvedorServer(void *arg)
//{
//}
int validar_usuario(int usuarioadm, int usuario)
{
    int validacion = 0;
    printf("Validando Datos....\n");
    if (usuario == usuarioadm)
    {
        printf("ok usuario\n");
    }
    else
    {
        printf("no ok usuario\n");
        validacion = 1;
    }
    return validacion;
}

int validarContrasena(char conexion[], char contra[])
{
    printf("Validando Datos....\n");
    int validacion = 0;
    validacion = strcmp(conexion, contra);

    if (validacion == 0)
    {
        printf("ok\n");
    }
    else
    {
        printf("no ok\n");
    }
    return validacion;
}

void PIPE_CONEXION(int fd)
{
    //Recepción de información del usuario
    //lectura de usuario
    fd = open("/tmp/mi_fifo", O_RDONLY);

    read(fd, &Usuarioadm, sizeof(Usuarioadm));

    //lectura de contra
   
    read(fd, &conexion, sizeof(conexion));

    close(fd);

    //Procesamiento y respuesta de la conexion
    printf("\nValdando usuario\n");
    int identificadorConexionUsuario = validar_usuario(Usuarioadm, usuario);
    printf("\nValdando contra\n");
    int identificadorConexion = validarContrasena(conexion, contra);


    if (identificadorConexion == 0 && identificadorConexionUsuario == 0)
    {
        //Le confirmamos al usuario que su contraseña es correcta
        
        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        write(fd, &identificadorConexion, sizeof(int));

        close(fd);

        //Recibimos su id
        fd = open("/tmp/mi_fifo", O_RDONLY);

        read(fd, &id, sizeof(int));
        read(fd, &usuario_provedor, sizeof(int));

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