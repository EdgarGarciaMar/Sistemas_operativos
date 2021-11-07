#include <stdlib.h>
#include <stdlib.h>
#include <ncurses.h>



void holaMundo()
{
    initscr();             //inicio de ncurses
    printw("Hola linux!"); //hola mundo!
    refresh();
    getch();
    //Limpieza de pantalla
    erase();
    endwin();
}
void ventanas()
{
    initscr(); //inicio de ncurses
    int height, width, start_y, start_x;
    height = 25;           //alto de la ventana
    width = 70;            //ancho de la ventana
    start_y = start_x = 6; //inicio de la ventana
    WINDOW *win = newwin(height, width, start_y, start_x);
    refresh();

    box(win, 0, 0);
    wprintw(win, "Ejemplo de ventana"); //nombre de la ventana
    wrefresh(win);
    mvprintw(10, 10, "Ejemplo Salida por pantalla");
    getch();
    //Limpieza de pantalla
    erase();

    endwin();
}
void input()
{
    //Inicio
    initscr();
    noecho();
    cbreak();
    //obtención del tamaño de la pantalla
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    //Instrucciones
    printw("Introduce A, B, C para ejecutar una accion.");
    //ventana de entrada
    WINDOW *inputwin = newwin(3, xMax - 12, yMax - 5, 5);
    box(inputwin, 0, 0);
    refresh();
    wrefresh(inputwin);
    int c = wgetch(inputwin);

    if (c == 'A')
    {
        mvwprintw(inputwin, 1, 1, "Presionaste A!");
        wrefresh(inputwin);
    }
    else if (c == 'B')
    {
        mvwprintw(inputwin, 1, 1, "Presionaste B!");
        wrefresh(inputwin);
    }
    else if (c == 'C')
    {
        mvwprintw(inputwin, 1, 1, "Presionaste C!");
        wrefresh(inputwin);
    }
    else
    {
        mvwprintw(inputwin, 1, 1, "Presionaste una opcion que no esta registrada");
        wrefresh(inputwin);
    }

    getch();
    //Limpieza de pantalla
    erase();
    endwin();
}


void mouse()
{

}
void menu()
{
    int opc;
    printf("***P4 DISPOSITIVOS DE ENTRADA Y DE SALIDA***\n");
    printf("Programa que utiliza los dispositivos de entrada y\n");
    printf("salia en lenguaje C utilizando la biblioteca ncurses.\n");
    printf("Menu:\n");
    printf("1: Salir\n");
    printf("2: Hola mundo!\n");
    printf("3: Ventanas\n");
    printf("4: Entrada por teclado\n");
    printf("5: Entrada por raton\n");
    printf("Selecciona una opcion:\n");
    scanf("%d", &opc);

    switch (opc)
    {
    case 1:
        system("clear");
        exit(0);
        break;
    case 2:

        holaMundo();
        break;
    case 3:

        ventanas();
        break;
    case 4:

        input();
        break;
    case 5:
        mouse();
        break;
    default:
        printf("Sleecciona una opcion valida del menu:\n");
        menu();
        break;
    }
    system("clear");
    menu();
}

int main(int argc, char *argv[])
{
    menu();
    return 0;
}
