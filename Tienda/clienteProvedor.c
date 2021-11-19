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
int usuario = 0;
int respuesta = 0;
int id = 0;
//opc
int swichopc;
int IDproducto;
char Descripcion[100];
char nombre_producto[100];
char rsp[100];

//prototipos
int PIPE_CONEXION(int conexion, int identificador);
int inicioProveedor();
int inicioCliente();
void crearCuenta();
void opcionesProvedor();
void opcionesCliente();
void inicioSesion();

int main(void)
{
    inicioSesion();
    return 0;
}

//Tuberia de conexion, con esta funcion pedimos acceso al servidor
int PIPE_CONEXION(int conexion, int identificador)
{
    printf("Escribe Escribe tu ID Ejemplo: 1 :\n");
    scanf("%d", &usuario);
    printf("Escribe la contrasena de 5 carateres:\n");
    scanf("%s", contrasena);

    //envio de usuario
    mkfifo("/tmp/mi_fifo", 0666);

    fd = open("/tmp/mi_fifo", O_WRONLY);

    write(fd, &usuario, sizeof(int));

    //envio de contrasena

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
        write(fd, &identificador, sizeof(int));

        close(fd);

        printf("ID de esta sesion: %d\n", id);
    }
    else
    {
        respuesta = -1;
    }
    return respuesta;
}

//login del proverdor, verifica su contraseña y su id y si son incorrectas niega el acceso
int inicioProveedor()
{
    printf("Conectando con el servidor....\n");
    int con = PIPE_CONEXION(fd, 1); //1 para proveedor

    return con;
}

//login del cliente, verifica su contraseña y su id y si son incorrectas niega el acceso
int inicioCliente()
{
    printf("Conectando con el servidor....\n");
    int con = PIPE_CONEXION(fd, 2); //2 para cliente

    return con;
}

//Función de creacion de cuenta para cliente y proveedor
void crearCuenta()
{
    int id_nuevo;
    char nombre[20];
    printf("Ingresa tu nombre de no mas de 20 caracteres\n");
    scanf("%s", nombre);
    srand(time(NULL));
    id_nuevo = rand() % 1100;
}

//Acciones de Agregar artículo, Agregar existencia, Búsqueda del artículo
void opcionesProvedor()
{
    int opcionProveerdor;
    char descrip[10]="NULL";

    printf("+Selecciona 1 de las siguientes opciones:\n");
    printf("1: Busqueda de articulo\n");
    printf("2: Agregar articulo\n");
    printf("3: Agregar Existencia\n");
    printf("4: Salir\n");
    scanf("%d", &opcionProveerdor);

    switch (opcionProveerdor)
    {
    case 1:
        /* pedirle al provedor el nombre del articulo o id y enviar al servidor */
        swichopc = 1;
        IDproducto=0;
        

        printf("Ingresa el nombre del producto.txt a buscar:\n");
        scanf("%s", nombre_producto);

        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        write(fd, &swichopc, sizeof(int)); //opcion a ejecutar el server
        write(fd, &nombre_producto, sizeof(nombre_producto));
        write(fd, &IDproducto, sizeof(IDproducto));
        write(fd, &descrip, sizeof(descrip));

        close(fd);

        //Recibir la respuesta

        fd = open("/tmp/mi_fifo", O_RDONLY);
        read(fd, &rsp, sizeof(rsp));
        printf("%s", rsp);
        close(fd);

        inicioSesion();

        break;
    case 2:

        swichopc = 2;
        IDproducto = 0;

        printf("Ingresa el nombre del producto.txt:\n");
        scanf("%s", nombre_producto);
        printf("Ingresa el ID del producto de 3 digitos:\n");
        scanf("%d", &IDproducto);
        printf("Ingresa la existencia:\n");
        scanf("%s", Descripcion);

        //envio de Datos del producto
        mkfifo("/tmp/mi_fifo", 0666);

        fd = open("/tmp/mi_fifo", O_WRONLY);

        write(fd, &swichopc, sizeof(int)); //opcion a ejecutar el server
        write(fd, &nombre_producto, sizeof(nombre_producto));
        write(fd, &IDproducto, sizeof(IDproducto));
        write(fd, &Descripcion, sizeof(Descripcion));

        close(fd);

        printf("Producto Agregado Correctamente\n");
        inicioSesion();
        break;
    case 3:
        /*pedirle al provedor el nombre del articulo o id, el numero de unidades a agregar y enviar al servidor*/
        break;
    case 4:
        printf("Regresando a inicio...\n");
        inicioSesion();
        break;
    }
}

//Acciones de solicitar carrito, agregar carrito
void opcionesCliente()
{
    int opcionCliente;

    printf("+Selecciona 1 de las siguientes opciones:\n");
    printf("1: Solicitar Carrito\n");
    printf("2: Guaradr carrito\n");
    scanf("%d", &opcionCliente);
    switch (opcionCliente)
    {
    case 1:
        /* Solicitar un carrito, abrir la comunicacion y comenzar a comprar */
        break;
    case 2:
        /*guardas el caririto*/
        break;
    default:
        printf("No seleccionaste ninguna opcion del menu, regresando a inicio...\n");
        //inicioSesion();
        break;
    }
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