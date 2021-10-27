#include <stdio.h>
#include <stdlib.h>

//login del proverdor, verifica su contraseña y su id y si son incorrectas niega el acceso
int inicioProveedor()
{

    return -1;
}

//login del cliente, verifica su contraseña y su id y si son incorrectas niega el acceso
int inicioCliente()
{

    return -1;
}

//Función de creacion de cuenta para cliente y proveedor
void crearCuenta()
{
}

//Acciones de Agregar artículo, Agregar existencia, Búsqueda del artículo
void opcionesProvedor()
{
}

//Acciones de solicitar carrito, agregar carrito
void opcionesCliente()
{
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