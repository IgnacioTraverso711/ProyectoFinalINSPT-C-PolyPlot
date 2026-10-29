//xterm -geometry 140x40 -T $TITLE -e
// Lo de arriba lo tuve que pegar en la opciones del IDE porque no me daba el espacio en la consola para mostrar todo

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define leer_int(x) scanf(" %d", &(x))
#define leer_char(x) scanf(" %c", &(x))
#define leer_str(x) scanf("%s", (x))
#define filas(matriz) (sizeof(matriz) / sizeof(matriz[0]))
#define columnas(matriz) (sizeof(matriz[0]) / sizeof(matriz[0][0]))
#define tam_vector(arr) (sizeof(arr) / sizeof((arr)[0]))

#define ALTO_GRAFICO 51
#define ANCHO_GRAFICO 121
#define MAXIMO_RECORDS 5
typedef struct {
    int x;
    int y;
} punto;

typedef struct{
  char nombre[30];
  int cant_rondas;
} puntuacion_maxima;

int leerPuntuaciones(puntuacion_maxima lista[]) {
    FILE *archivo = fopen("puntuacion.txt", "r");
    int cantidad = 0;

    if (archivo != NULL) {
        while (cantidad < MAXIMO_RECORDS && fscanf(archivo, "%29s %d", lista[cantidad].nombre, &lista[cantidad].cant_rondas) == 2) {
            cantidad++;
        }
        fclose(archivo);
    }
    return cantidad;
}

// Guarda la lista completa en el archivo
void guardarPuntuaciones(puntuacion_maxima lista[], int cantidad) {
    FILE *archivo = fopen("puntuacion.txt", "w");

    if (archivo != NULL) {
        for (int i = 0; i < cantidad; i++) {
            fprintf(archivo, "%s %d\n", lista[i].nombre, lista[i].cant_rondas);
        }
        fclose(archivo);
    } else {
        printf("Error al guardar las puntuaciones.\n");
    }
}

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

int calcularPuntoFuncion(int terminos[], int grado, int puntoX) {
    int puntoY = 0;
    int potenciaX = 1; // esto es igual a x^0
    for (int i = 0; i <= grado; i++) {
        puntoY += terminos[i] * potenciaX;
        potenciaX *= puntoX;
    }
    return puntoY;
}

void generarPuntosObjetivo(punto objetivos[], int cantObjetivos) {
    int terminos[4] = {0};
    int grado = (rand() % 3) + 1; // Grado entre 1 y 3 basicamente
    // Coeficientes pequeños (-2 a 2) para evitar que se me vaya fuera de rango la Y
    for (int i = 0; i <= grado; i++) {
        terminos[i] = (rand() % 5) - 2;
    }
    if (terminos[0] == 0 && terminos[1] == 0 && terminos[2] == 0 && terminos[3] == 0) {
        terminos[0] = 1; // Evitar polinomio nulo
    }

    for (int i = 0; i < cantObjetivos; i++) {
        int repetido, xCandidata, yCalculada;
        do {
            repetido = 0;
            xCandidata = (rand() % 121) - 60; // X entre -60 y 60

            // Evitar generar dos objetivos en la misma columna X
            for (int j = 0; j < i; j++) {
                if (objetivos[j].x == xCandidata) {
                    repetido = 1;
                    break;
                }
            }

            if (!repetido) {
                yCalculada = calcularPuntoFuncion(terminos, grado, xCandidata);
                // Si la Y calculada está fuera de los límites, buscamos otra
                if (yCalculada < -25 || yCalculada > 25) {
                    repetido = 1;
                }
            }
        } while (repetido == 1);

        objetivos[i].x = xCandidata;
        objetivos[i].y = yCalculada;
    }
}

void ingresarFuncionUsuario(punto funcion[], int cantPuntos) {
    int grado;
    char confirmacion;

    do {
        int terminos[4] = {0};

        do {
            printf("\nIngrese el grado de la funcion (Maximo 3, Minimo 1): ");

            // Verificamos si fallo leerel entero
            if (leer_int(grado) != 1) {
                while (getchar() != '\n'); // hago esto para evitar que me rompa las pelotas a veces
                grado = -1;
            }

            if (grado > 3 || grado < 1) {
                printf("Entrada invalida. Por favor, ingrese solo un numero entre 1 y 3.\n");
            }
        } while (grado > 3 || grado < 1);

        for (int i = grado; i >= 0; i--) {
            int entradaValida;
            do {
                if (i == grado) {
                    printf("Ingrese el termino principal (x^%d): ", i);
                } else if (i == 1) {
                    printf("Ingrese el coeficiente lineal (x): ");
                } else if (i == 0) {
                    printf("Ingrese el termino independiente: ");
                } else {
                    printf("Ingrese el coeficiente de x^%d: ", i);
                }

                entradaValida = leer_int(terminos[i]);
                if (entradaValida != 1) {
                    while (getchar() != '\n'); // vuelvo a hacerf esto aca
                    printf("Por favor, ingrese un numero valido.\n");
                }
            } while (entradaValida != 1);
        }

        printf("\nSu funcion es: y = ");
        for (int i = grado; i >= 0; i--) {
            if (i == 0) {
                printf("%d", terminos[i]);
            } else if (i == 1) {
                printf("%dx ", terminos[i]);
            } else {
                printf("%dx^%d ", terminos[i], i);
            }

            if (i > 0 && terminos[i - 1] >= 0) {
                printf("+ ");
            }
        }

        printf("\n¿Confirmar esta funcion? (S/N): ");
        leer_char(confirmacion);

        if (confirmacion == 'S' || confirmacion == 's') {
            for (int i = 0; i < cantPuntos; i++) {
                int x = i - 60;
                funcion[i].x = x;
                funcion[i].y = calcularPuntoFuncion(terminos, grado, x);
            }
        }

    } while (confirmacion == 'N' || confirmacion == 'n');
}

int contarImpactos(punto funcion[], punto objetivos[], int cantObjetivos) {
    int impactos = 0;

    for (int i = 0; i < cantObjetivos; i++) {
        int ox = objetivos[i].x;
        int oy = objetivos[i].y;

        // En el arreglo, el índice 0 corresponde a X=-60.
        // Entonces, el índice de cualquier X es (X + 60).
        int indiceFuncion = ox + 60;

        if (indiceFuncion >= 0 && indiceFuncion < ANCHO_GRAFICO) {
            // Si la Y del usuario en esa X es igual a la Y del objetivo, es un impacto
            if (funcion[indiceFuncion].y == oy) {
                impactos++;
            }
        }
    }

    return impactos;
}

int main() {
    srand(time(NULL));

    punto funcionUsuario[ANCHO_GRAFICO];
    punto objetivos[50];

    int vidas = 5;
    int nivel = 1;
    int cantObjetivos = 1;

    printf("\n=============================================================\n");
    printf("        ¡Golpea los puntos escribiendo una funcion!!!!         \n");
    printf("===============================================================\n");

    while (vidas > 0) {
        printf("\n--- NIVEL %d | VIDAS: %d | OBJETIVOS: %d ---\n", nivel, vidas, cantObjetivos);

        generarPuntosObjetivo(objetivos, cantObjetivos);
        mostrarGraficoPuntos(funcionUsuario, 0, objetivos, cantObjetivos);

        ingresarFuncionUsuario(funcionUsuario, ANCHO_GRAFICO);

        mostrarGraficoPuntos(funcionUsuario, ANCHO_GRAFICO, objetivos, cantObjetivos);

        int impactos = contarImpactos(funcionUsuario, objetivos, cantObjetivos);
        printf("\n¡Impactaste %d de %d objetivos!\n", impactos, cantObjetivos);

        if (impactos == cantObjetivos) {
            printf("¡BIEN! le pegaste a todos.\n", nivel);
            nivel++;
            cantObjetivos++;
        } else {
            vidas--;
            printf("¡No le pegaste a todos! Perdes una vida. Te quedan %d vidas.\n", vidas);
        }

        if (vidas > 0) {
            printf("\nPresiona ENTER para continuar...");
            while (getchar() != '\n');
            getchar();
        }
    }

    // ... fin del bucle de vidas ...

    printf("\n========================================\n");
    printf("              ¡PERDISTE!               \n");
    printf("      Llegaste hasta el nivel %d        \n", nivel);
    printf("========================================\n");

    puntuacion_maxima records[MAXIMO_RECORDS];
    int cant_records = leerPuntuaciones(records);

    int posicion_insercion = -1;
    for (int i = 0; i < cant_records; i++) {
        if (nivel > records[i].cant_rondas) {
            posicion_insercion = i;
            break;
        }
    }

    // Si no superó a nadie pero hay espacio libre en la lista, entra al final
    if (posicion_insercion == -1 && cant_records < MAXIMO_RECORDS) {
        posicion_insercion = cant_records;
    }

    // Insertar el nuevo récord si clasificó
    if (posicion_insercion != -1) {
        printf("\n¡Felicidades! Entraste al Top %d de mejores jugadores.\n", MAXIMO_RECORDS);

        char nuevoNombre[30];
        printf("Ingresa tu nombre (sin espacios): ");
        leer_str(nuevoNombre);
        while (getchar() != '\n');

        // Muevo hacia abajo los otros registros
        int fin;
        if (cant_records < MAXIMO_RECORDS) {
            fin = cant_records;
        } else {
            fin = MAXIMO_RECORDS - 1;
        }
        for (int i = fin; i > posicion_insercion; i--) {
            records[i] = records[i - 1];
        }

        // Guardo el registro
        strcpy(records[posicion_insercion].nombre, nuevoNombre);
        records[posicion_insercion].cant_rondas = nivel;

        if (cant_records < MAXIMO_RECORDS) {
            cant_records++;
        }

        guardarPuntuaciones(records, cant_records);
    }

    printf("\n--- TABLA DE POSICIONES ---\n");
    if (cant_records == 0) {
        printf("Todavia no hay registros.\n");
    } else {
        for (int i = 0; i < cant_records; i++) {
            printf("%d. %s - %d niveles\n", i + 1, records[i].nombre, records[i].cant_rondas);
        }
    }

    return 0;
}
