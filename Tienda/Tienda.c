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
pthread_t thread1, thread2;
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
//Operaciones de provedor
int ID_producto = 0;
char Descripcion[100];
char nombre_producto[100];
int swichopc = 0;
//Archivos compartidos
FILE *producto_p;

void *opcionesProvedorServer()
{
    printf("HILO***************************");
    sem_wait(&semaforo);
    int opc = swichopc;

    switch (opc)
    {
    case 1: //Busqueda de articulo

        break;
    case 2: //Agregar Articulo

        producto_p = fopen(nombre_producto, "w");
        if (producto_p == NULL)
        {
            printf("Error al abir o crear el archivo del producto.\n");
            exit(0);
        }

        char text[20];
        sprintf(text, "%d", ID_producto);

        fputs(nombre_producto, producto_p);
        fputs("\n", producto_p);
        fputs(text, producto_p);
        fputs("\n", producto_p);
        fputs(Descripcion, producto_p);
        fputs("\n", producto_p);

        fclose(producto_p);

        //leer lo que se escribio

        /*char buffer[1000];

        producto_p = fopen(nombre_producto, "r");

        fscanf(producto_p, "%s", buffer);
        printf("%s", buffer);

        fclose(producto_p);*/

        break;
    case 3: //Agregar existencia
        /* code */
        break;
    }

    sem_post(&semaforo);
    pthread_exit(NULL);
}

void menu_de_opciones(int usuario_provedor)
{
    
    if (usuario_provedor == 1) //opc de proveedor
    {
        /*
    1: Busqueda de articulo
    2: Agregar articulo
    3: Agregar Existencia
        */
        printf("\nHOLA DESDE LAS OPC DE PROVEDOR SERVER\n");
        fd = open("/tmp/mi_fifo", O_RDONLY);
        read(fd, &swichopc, sizeof(int));
        read(fd, &nombre_producto, sizeof(nombre_producto));
        read(fd, &ID_producto, sizeof(ID_producto));
        read(fd, &Descripcion, sizeof(Descripcion));
        close(fd);
        switch (swichopc)
        {
        case 1:
            /* code */
            break;
        case 2:
            
            if (0 != pthread_create(&thread1, NULL, opcionesProvedorServer,NULL))
            {
                printf("Error en hilo\n");
                exit(0);
            }
            printf("\nUn cliente esta siendo atendido.\n");

            break;
        case 3:
            /* code */
            break;
        }
    }
    else
    { //opciones del usuario
        /*
      1: Solicitar Carrito
      2: Guaradr carrito
    */
        printf("\nHOLA DESDE LAS OPC DE usuario SERVER");
    }
}
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
        printf("ok contra\n");
    }
    else
    {
        printf("no ok contra\n");
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

        menu_de_opciones(usuario_provedor); //habilitamos las acciones del server
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
    sem_init(&semaforo, 0, 1);
    while (1)
    {

        printf("\t\t***Proceso de servidor Tienda***\n");
        printf("\tEsperando una conexion....\n");
        PIPE_CONEXION(fd);
    }
    return 0;
}