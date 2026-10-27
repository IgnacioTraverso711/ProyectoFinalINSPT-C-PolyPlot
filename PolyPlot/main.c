//xterm -geometry 140x40 -T $TITLE -e
// Lo de arriba lo tuve que pegar en la opciones del IDE porque no me daba el espacio en la consola para mostrar todo

#include <stdio.h>
#include <stdlib.h>

#define leer_int(x) scanf(" %d", &(x))
#define leer_char(x) scanf(" %c", &(x))
#define leer_str(x) scanf("%s", (x))
#define filas(matriz) (sizeof(matriz) / sizeof(matriz[0]))
#define columnas(matriz) (sizeof(matriz[0]) / sizeof(matriz[0][0]))
#define tam_vector(arr) (sizeof(arr) / sizeof((arr)[0]))

#define ALTO_GRAFICO 51
#define ANCHO_GRAFICO 121

typedef struct {
    int x;
    int y;
} punto;

void mostrarGraficoPuntos(punto funcion[], int cantPuntosFuncion, punto objetivos[], int cantObjetivos) {
    char grafico[ALTO_GRAFICO][ANCHO_GRAFICO];
    int f, c;

    //armo el grafico en la matriz
    for (f = 0; f < ALTO_GRAFICO; f++) {
        int y = 25 - f;
        for (c = 0; c < ANCHO_GRAFICO; c++) {
            int x = c - 60;

            if (x == 0 && y == 0) {
                grafico[f][c] = '+';
            } else if (y == 0) {
                grafico[f][c] = '-';
            } else if (x == 0) {
                grafico[f][c] = '|';
            } else {
                grafico[f][c] = '.';
            }
        }
    }

    // dibujo la funcion del usuario en la matriz
    for (int i = 0; i < cantPuntosFuncion; i++) {
        int px = funcion[i].x;
        int py = funcion[i].y;

        if (px >= -60 && px <= 60 && py >= -25 && py <= 25) {
            grafico[25 - py][px + 60] = 'O';
        }
    }

    // recorro el vector de objetivos y cambio los valores en el grafico correspondientemente, si detecta colision con la funcion del usuario tambien lo agrega
    for (int i = 0; i < cantObjetivos; i++) {
        int ox = objetivos[i].x;
        int oy = objetivos[i].y;

        if (ox >= -60 && ox <= 60 && oy >= -25 && oy <= 25) {
            int col = ox + 60;
            int fila = 25 - oy;

            if (grafico[fila][col] == 'O') {
                grafico[fila][col] = 'X';
            } else {
                grafico[fila][col] = '*';
            }
        }
    }

    // imprimir matriz
    printf("\n* = Objetivo  ///  O = Tu Funcion  ///  X = ¡GOLPE!\n");
    printf("+ = Centro    ///  | = (Y=0)       ///  - = (X=0)\n");
    printf(". = Vacio\n\n");

    for (f = 0; f < ALTO_GRAFICO; f++) {
        int y = 25 - f;

        // %3d lo use para alinear los numeros del eje y, eso hace que siempre ocupen 3 espacio, de esa manera no importa la cantida de caracteres del eje y no se va a romper mostrar la amtriz
        printf("%3d | ", y);

        for (c = 0; c < ANCHO_GRAFICO; c++) {
            printf("%c", grafico[f][c]);
        }
        printf("\n");
    }
    //Guia de abajo
    printf("      -------------------------------------------------------------------------------------------------------------------------\n");
    printf("      -60       -50       -40       -30       -20       -10       0         10        20        30        40        50        60\n\n");
}

int main() {
    punto funcionUsuario[121];
    punto objetivos[50];

    mostrarGraficoPuntos(funcionUsuario, 0, objetivos, 0);

    return 0;
}
