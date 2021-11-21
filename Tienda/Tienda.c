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
//recuperacion de datos del archivo
#include <time.h>

//******variables globales******
//tuberia
int fd = 0;
//hilo
pthread_t thread1, thread2;
//semaforo
sem_t semaforo, semaforo2;
//variables funcionales
char conexion[5];     //contra a validar
int Usuarioadm = 0;   //usuario a validar
int usuario_provedor; //indicador que habilita opciones para cliente o para proveedor
//usuario admin
char *contra[] = {"12345", "56789", "01234"};
int tam = sizeof(contra) / sizeof(char *);
int usuario[3] = {1, 2, 3};
int id;
//Operaciones de provedor
int ID_producto = 0; //Precio del producto
char Descripcion[100];
char nombre_producto[100];
int swichopc = 0;
char rsp[] = "EL producto no esta registrado.\n";
char rsp2[] = "El producto esta en almacen.\n";
//Archivos compartidos
FILE *producto_p;
FILE *carrito;
char text[20];
//archivo de carritos
char nombrecarro[30];

void *opcionesUsuario()
{
    printf("********************HILO Usuario***************************\n");
    sem_wait(&semaforo2);
    //printf("opc=%d\n",swichopc);
    switch (swichopc)
    {
    case 1:
        carrito = fopen(nombrecarro, "w");
        if (carrito == NULL)
        {
            printf("Error al abir o crear el archivo del carro del cliente.\n");
            exit(0);
        }
        //printf("***Carro creado******\n");

        sprintf(text, "%d", ID_producto);

        fputs(nombre_producto, carrito);
        fputs("\n", carrito);
        fputs(text, carrito);
        fputs("\n", carrito);
        fputs(Descripcion, carrito);
        fputs("\n", carrito);
        fputs("Pago-directo.\n", carrito);

        fclose(carrito);
        break;
    case 2:
        carrito = fopen(nombrecarro, "w");
        if (carrito == NULL)
        {
            printf("Error al abir el archivo del carro del cliente.\n");
            exit(0);
        }
        //printf("***Carro creado******\n");

        sprintf(text, "%d", ID_producto);

        fputs(nombre_producto, carrito);
        fputs("\n", carrito);
        fputs(text, carrito);
        fputs("\n", carrito);
        fputs(Descripcion, carrito);
        fputs("\n", carrito);
        fputs("Pago-archivo.\n", carrito);

        fclose(carrito);
        break;

    default:
        break;
    }
    sem_post(&semaforo2);
    pthread_exit(NULL);
}

void *opcionesProvedorServer()
{
    printf("********************HILO Proveedor***************************\n");
    sem_wait(&semaforo);
    int opc = swichopc;
    struct stat sb;

    switch (opc)
    {
    case 1: //Busqueda de articulo

        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        if (stat(nombre_producto, &sb) == -1)
        {

            perror("El archivo no existe reiniciando server\n");
            write(fd, &rsp, sizeof(rsp));
            exit(EXIT_FAILURE);
        }

        printf("Inode number: %lu\n", sb.st_ino);
        printf("User ID of owner: %u\n", sb.st_uid);
        printf("Group ID of owner: %u\n", sb.st_gid);
        printf("Total file size: %lu bytes\n", sb.st_size);
        printf("Last status change:       %s", ctime(&sb.st_ctime));
        printf("Last file access:         %s", ctime(&sb.st_atime));
        printf("Last file modification:   %s", ctime(&sb.st_mtime));

        write(fd, &rsp2, sizeof(rsp2));
        close(fd);
        //printf("Consulta finalizada con exito...\n");
        //exit(0);
        break;
    case 2: //Agregar Articulo

        producto_p = fopen(nombre_producto, "w");
        if (producto_p == NULL)
        {
            printf("Error al abir o crear el archivo del producto.\n");
            exit(0);
        }

        sprintf(text, "%d", ID_producto);

        fputs(nombre_producto, producto_p);
        fputs("\n", producto_p);
        fputs(text, producto_p);
        fputs("\n", producto_p);
        fputs(Descripcion, producto_p);
        fputs("\n", producto_p);

        fclose(producto_p);

        break;
    case 3: //Agregar existencia
        /* code */
        producto_p = fopen(nombre_producto, "r");
        if (producto_p == NULL)
        {
            printf("Error al abir el archivo del producto.\n");
            exit(0);
        }
        if (stat(nombre_producto, &sb) == -1)
        {
            perror("Error al abir el archivo del producto.\n");
            exit(EXIT_FAILURE);
        }

        char file_contents[1000];
        fread(file_contents, sb.st_size, 1, producto_p);

        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        write(fd, &file_contents, sizeof(file_contents));

        close(fd);
        //printf("Actualizacion realizada con exito, nuevos datos:\n");
        printf("%s\n", file_contents);

        fclose(producto_p);

        //exit(1);
        break;
    }
    //printf("+++Salio..+++");
    sem_post(&semaforo);
    pthread_exit(NULL);
    //printf("+++No Salio..+++");
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

            if (0 != pthread_create(&thread1, NULL, opcionesProvedorServer, NULL))
            {
                printf("Error en hilo de consulta\n");
                exit(0);
            }
            printf("\nUn cliente esta siendo atendido.\n");
            break;
        case 2:

            if (0 != pthread_create(&thread1, NULL, opcionesProvedorServer, NULL))
            {
                printf("Error en hilo de agregar producto\n");
                exit(0);
            }
            printf("\nUn cliente esta siendo atendido.\n");

            break;
        case 3:
            if (0 != pthread_create(&thread1, NULL, opcionesProvedorServer, NULL))
            {
                printf("Error en hilo de Actualizar producto\n");
                exit(0);
            }
            printf("\nUn cliente esta siendo atendido.\n");
            break;
        case 4:
            printf("El cliente salio de la app.\n");
            break;
        }
    }
    else
    { //opciones del usuario
        /*
      1: Solicitar Carrito
      2: Guaradr carrito
    */
        //printf("\nHOLA DESDE LAS OPC DE usuario SERVER");

        fd = open("/tmp/mi_fifo", O_RDONLY);
        read(fd, &swichopc, sizeof(int));
        read(fd, &nombrecarro, sizeof(nombrecarro));
        read(fd, &nombre_producto, sizeof(nombre_producto));
        read(fd, &ID_producto, sizeof(ID_producto));
        read(fd, &Descripcion, sizeof(Descripcion));
        close(fd);

        printf("%d---%s---%s---%d---%s", swichopc, nombrecarro, nombre_producto, ID_producto, Descripcion);
        switch (swichopc)
        {
        case 1:
            if (0 != pthread_create(&thread1, NULL, opcionesUsuario, NULL))
            {
                printf("Error en hilo de solicitud de carrito\n");
                exit(0);
            }
            printf("\nUn cliente esta comprando.\n");
            break;
        case 2:
            if (0 != pthread_create(&thread1, NULL, opcionesUsuario, NULL))
            {
                printf("Error en hilo de Guardado de carrito\n");
                exit(0);
            }
            printf("\nUn cliente esta pagando y guardando su carro.\n");
            break;
        default:
            printf("El cliente salio de la app.\n");
            break;
        }
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
    int identificadorConexionUsuario;
    int identificadorConexion;
    //Recepción de información del usuario
    //lectura de usuario
    fd = open("/tmp/mi_fifo", O_RDONLY);

    read(fd, &Usuarioadm, sizeof(Usuarioadm));

    //lectura de contra

    read(fd, &conexion, sizeof(conexion));

    close(fd);

    //Procesamiento y respuesta de la conexion
    for (int i = 0; i < tam; i++)
    {
        printf("\nValdando usuario: %d\n", i);
        identificadorConexionUsuario = validar_usuario(Usuarioadm, usuario[i]);
        printf("\nValdando contra: %d\n", i);
        identificadorConexion = validarContrasena(conexion, contra[i]);
        printf("%d----%s\n", usuario[i], contra[i]);

        if (identificadorConexion == 0 && identificadorConexionUsuario == 0)
        {
            break;
        }
    }

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
        //printf("Salio de opc de proveedorrrrrrr");
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
    sem_init(&semaforo2, 0, 1);
    while (1)
    {

        printf("\t\t***Proceso de servidor Tienda***\n");
        printf("\tEsperando una conexion....\n");
        PIPE_CONEXION(fd);
    }
    return 0;
}