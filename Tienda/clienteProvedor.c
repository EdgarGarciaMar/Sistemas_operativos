//Generales y fifo
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
//Variables globales
int fd;
char contrasena[5];
int respuesta = 0;
int id = 0;

//Tuberia de conexion, con esta funcion pedimos acceso al servidor
int PIPE_CONEXION(int conexion)
{
    printf("Escribe la contrasena de 5 carateres:\n");
    scanf("%s", contrasena);

    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, &contrasena, sizeof(contrasena));

    close(fd);

    //Recepción de la confirmación de la contraseña

    fd = open("/tmp/mi_fifo", O_RDONLY);

    read(fd, &respuesta, sizeof(int));

    close(fd);

    if (respuesta == 0)
    {
        //Envio de ID, porque la contraseña es correcta
        srand(time(NULL));
        id = rand() % 1100;

        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        write(fd, &id, sizeof(int));

        close(fd);

        printf("ID de esta sesion: %d\n", id);
    }
    else{
        respuesta=-1;
    }
    return respuesta;
}

//login del proverdor, verifica su contraseña y su id y si son incorrectas niega el acceso
int inicioProveedor()
{
    printf("Conectando con el servidor....\n");
    int con = PIPE_CONEXION(fd);

    return con;
}

//login del cliente, verifica su contraseña y su id y si son incorrectas niega el acceso
int inicioCliente()
{
    printf("Conectando con el servidor....\n");
    int con = PIPE_CONEXION(fd);

    return con;
}

//Función de creacion de cuenta para cliente y proveedor
void crearCuenta()
{
    printf("Hola soy crear cuenta\n");
}

//Acciones de Agregar artículo, Agregar existencia, Búsqueda del artículo
void opcionesProvedor()
{
    printf("Hola soy opciones de provedor\n");
}

//Acciones de solicitar carrito, agregar carrito
void opcionesCliente()
{
    printf("Hola soy opciones de cliente\n");
}

void inicioSesion()
{
    //Variables
    int login, proveedor = 0, cliente = 0, opcErrorContrasena;

    //Impresion en consola
    printf("***************************************\n");
    printf("\t**Mi Tienda**\n");
    printf("\t*Login*\n");
    printf("\t1: Inicio Proveedor\n");
    printf("\t2: Inicio Cliente\n");
    printf("\t3: Salir de la app\n");
    printf("***************************************\n");
    printf("Tu seleccion es: ");
    scanf("%d", &login);

    //Proceso de verificacion de la opcion seleccionada
    switch (login)
    {
    case 1:
        proveedor = inicioProveedor();
        if (proveedor == -1)
        {

            //Informe del error
            printf("**ID o contrasena incorrecta**\n");
            printf("Seleccione una de las siguientes opciones:\n");
            printf("1: Volver a digitar ID y contrasena desde el menu principal\n");
            printf("2: crear una cuenta\n");
            scanf("%d", &opcErrorContrasena);
            switch (opcErrorContrasena)
            {
            case 1:
                inicioSesion();
                break;
            case 2:
                crearCuenta();
            default:
                printf("opcion invalida, volviendo al menu\n");
                inicioSesion();
                break;
            }
        }
        else
        {
            opcionesProvedor();
        }

        break;
    case 2:
        cliente = inicioCliente();
        if (cliente == -1)
        {

            //Informe del error
            printf("**ID o contrasena incorrecta**\n");
            printf("Seleccione una de las siguientes opciones:\n");
            printf("1: Volver a digitar ID y contrasena desde el menu principal\n");
            printf("2: crear una cuenta\n");
            scanf("%d", &opcErrorContrasena);
            switch (opcErrorContrasena)
            {
            case 1:
                inicioSesion();
                break;
            case 2:
                crearCuenta();
            default:
                printf("opcion invalida, volviendo al menu\n");
                inicioSesion();
                break;
            }
        }

        else
        {
            opcionesCliente();
        }
        break;
    case 3:
        system("clear");
        printf("\tGracias por su visita!!.\n");
        exit(0);
        break;
    default:
        printf("Selecciona una opcion valida\n\n\n");
        inicioSesion();
        break;
    }
}

int main(void)
{
    inicioSesion();
    return 0;
}