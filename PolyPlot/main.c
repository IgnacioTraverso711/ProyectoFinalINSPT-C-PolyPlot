#include <stdio.h>
#include <stdlib.h>
#define leer_int(x) scanf(" %d", &(x))
#define leer_char(x)  scanf(" %c", &(x))
#define leer_str(x)   scanf("%s", (x))
#define filas(matriz) (sizeof(matriz) / sizeof(matriz[0]))
#define columnas(matriz) (sizeof(matriz[0]) / sizeof(matriz[0][0]))
#define tam_vector(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct {
    int x;
    int y;
} punto;

int main()
{
    punto primer_punto = {7,8};
    printf("x: %d \n", primer_punto.x);
    printf("y: %d \n", primer_punto.y);
}




