#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // para tolower

// estructura de un libro
typedef struct Libro {
    int ano;
    char* autor;
    char* titulo;
    char* descripcion;
    float precio;
    int codigo;
    struct Libro* sig; // puntero al siguiente libro
} Libro;

// cabeza de la lista enlazada
Libro* cabeza = NULL;

// funcion para copiar una cadena a memoria dinamica
char* copiar_cadena(const char* s) {
    if (s == NULL) return NULL;
    char* copia = (char*)malloc(strlen(s) + 1);
    if (copia) strcpy(copia, s);
    return copia;
}

// libera la memoria de un libro
void liberar_libro(Libro* l) {
    if (l) {
        free(l->autor);
        free(l->titulo);
        free(l->descripcion);
        free(l);
    }
}

// carga todos los libros desde el archivo binario
void cargar_libros() {
    FILE* f = fopen("libros.dat", "rb");
    if (!f) {
        printf("Archivo no existe. Empezando vacio.\n");
        return;
    }

    // limpiar la lista actual
    while (cabeza) {
        Libro* temp = cabeza;
        cabeza = cabeza->sig;
        liberar_libro(temp);
    }

    Libro tmp;
    while (fread(&tmp, sizeof(Libro), 1, f) == 1) {
        int len;

        // leer autor
        fread(&len, sizeof(int), 1, f);
        tmp.autor = (char*)malloc(len + 1);
        fread(tmp.autor, 1, len, f);
        tmp.autor[len] = '\0';

        // leer titulo
        fread(&len, sizeof(int), 1, f);
        tmp.titulo = (char*)malloc(len + 1);
        fread(tmp.titulo, 1, len, f);
        tmp.titulo[len] = '\0';

        // leer descripcion
        fread(&len, sizeof(int), 1, f);
        tmp.descripcion = (char*)malloc(len + 1);
        fread(tmp.descripcion, 1, len, f);
        tmp.descripcion[len] = '\0';

        // crear nodo nuevo y agregar al inicio
        Libro* nuevo = (Libro*)malloc(sizeof(Libro));
        *nuevo = tmp;
        nuevo->sig = cabeza;
        cabeza = nuevo;
    }

    fclose(f);
    printf("Libros cargados desde disco.\n");
}

// guarda todos los libros al archivo binario
void guardar_libros() {
    FILE* f = fopen("libros.dat", "wb");
    if (!f) {
        printf("Error al abrir archivo para guardar.\n");
        return;
    }

    Libro* actual = cabeza;
    while (actual) {
        // guardar datos fijos (int, float, punteros no se guardan directamente)
        Libro tmp = *actual;
        tmp.sig = NULL; // no guardar el puntero
        fwrite(&tmp, sizeof(Libro), 1, f);

        // guardar autor
        int len = strlen(actual->autor);
        fwrite(&len, sizeof(int), 1, f);
        fwrite(actual->autor, 1, len, f);

        // guardar titulo
        len = strlen(actual->titulo);
        fwrite(&len, sizeof(int), 1, f);
        fwrite(actual->titulo, 1, len, f);

        // guardar descripcion
        len = strlen(actual->descripcion);
        fwrite(&len, sizeof(int), 1, f);
        fwrite(actual->descripcion, 1, len, f);

        actual = actual->sig;
    }

    fclose(f);
    printf("Libros guardados en disco.\n");
}

// agrega un libro nuevo al inicio de la lista
void agregar_libro() {
    Libro* nuevo = (Libro*)malloc(sizeof(Libro));
    if (!nuevo) {
        printf("Error: no hay memoria.\n");
        return;
    }

    printf("Codigo: ");
    scanf("%d", &nuevo->codigo);
    printf("Ano: ");
    scanf("%d", &nuevo->ano);
    printf("Precio: ");
    scanf("%f", &nuevo->precio);

    // limpiar buffer de entrada
    while (getchar() != '\n');

    char buffer[200];

    printf("Autor: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    nuevo->autor = copiar_cadena(buffer);

    printf("Titulo: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    nuevo->titulo = copiar_cadena(buffer);

    printf("Descripcion: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    nuevo->descripcion = copiar_cadena(buffer);

    nuevo->sig = cabeza;
    cabeza = nuevo;

    printf("Libro agregado.\n");
}

// busca por titulo o autor (sin distinguir mayusculas)
void buscar_libro() {
    if (!cabeza) {
        printf("No hay libros.\n");
        return;
    }

    printf("Buscar por (1) Titulo o (2) Autor: ");
    int opc;
    scanf("%d", &opc);
    while (getchar() != '\n');

    char busqueda[100];
    printf("Texto a buscar: ");
    fgets(busqueda, sizeof(busqueda), stdin);
    busqueda[strcspn(busqueda, "\n")] = '\0';

    // convertir a minusculas
    for (int i = 0; busqueda[i]; i++)
        busqueda[i] = tolower(busqueda[i]);

    int encontrado = 0;
    Libro* actual = cabeza;

    while (actual) {
        char* texto = (opc == 1) ? actual->titulo : actual->autor;
        if (texto) {
            char temp[200];
            strcpy(temp, texto);
            for (int i = 0; temp[i]; i++)
                temp[i] = tolower(temp[i]);

            if (strstr(temp, busqueda)) {
                printf("\n--- LIBRO ENCONTRADO ---\n");
                printf("Codigo: %d\n", actual->codigo);
                printf("Titulo: %s\n", actual->titulo);
                printf("Autor: %s\n", actual->autor);
                printf("Ano: %d\n", actual->ano);
                printf("Precio: $%.2f\n", actual->precio);
                printf("Descripcion: %s\n", actual->descripcion);
                printf("-------------------------\n");
                encontrado = 1;
            }
        }
        actual = actual->sig;
    }

    if (!encontrado) {
        printf("No se encontro ningun libro.\n");
    }
}

// funcion nueva: buscar solo por autor
void buscar_por_autor() {
    if (!cabeza) {
        printf("No hay libros.\n");
        return;
    }

    char busqueda[100];
    printf("Ingrese el nombre del autor a buscar: ");
    fgets(busqueda, sizeof(busqueda), stdin);
    busqueda[strcspn(busqueda, "\n")] = '\0';

    for (int i = 0; busqueda[i]; i++)
        busqueda[i] = tolower(busqueda[i]);

    int encontrado = 0;
    Libro* actual = cabeza;

    while (actual) {
        if (actual->autor) {
            char temp[200];
            strcpy(temp, actual->autor);
            for (int i = 0; temp[i]; i++)
                temp[i] = tolower(temp[i]);

            if (strstr(temp, busqueda)) {
                printf("\n--- LIBRO ENCONTRADO ---\n");
                printf("Codigo: %d\n", actual->codigo);
                printf("Titulo: %s\n", actual->titulo);
                printf("Autor: %s\n", actual->autor);
                printf("Ano: %d\n", actual->ano);
                printf("Precio: $%.2f\n", actual->precio);
                printf("Descripcion: %s\n", actual->descripcion);
                printf("-------------------------\n");
                encontrado = 1;
            }
        }
        actual = actual->sig;
    }

    if (!encontrado) {
        printf("No se encontraron libros de ese autor.\n");
    }
}

// funcion nueva: ordenar la lista por precio usando Bubble Sort
void ordenar_por_precio() {
    if (!cabeza || !cabeza->sig) {
        printf("No hay suficientes libros para ordenar.\n");
        return;
    }

    int swapped;
    Libro* lptr = NULL;

    do {
        swapped = 0;
        Libro* ptr1 = cabeza;

        while (ptr1->sig != lptr) {
            if (ptr1->precio > ptr1->sig->precio) {
                // intercambiar todos los campos (menos el puntero sig)
                int temp_int;
                float temp_float;
                char* temp_str;

                temp_int = ptr1->codigo;
                ptr1->codigo = ptr1->sig->codigo;
                ptr1->sig->codigo = temp_int;

                temp_int = ptr1->ano;
                ptr1->ano = ptr1->sig->ano;
                ptr1->sig->ano = temp_int;

                temp_float = ptr1->precio;
                ptr1->precio = ptr1->sig->precio;
                ptr1->sig->precio = temp_float;

                temp_str = ptr1->autor;
                ptr1->autor = ptr1->sig->autor;
                ptr1->sig->autor = temp_str;

                temp_str = ptr1->titulo;
                ptr1->titulo = ptr1->sig->titulo;
                ptr1->sig->titulo = temp_str;

                temp_str = ptr1->descripcion;
                ptr1->descripcion = ptr1->sig->descripcion;
                ptr1->sig->descripcion = temp_str;

                swapped = 1;
            }
            ptr1 = ptr1->sig;
        }
        lptr = ptr1;
    } while (swapped);

    printf("Lista ordenada por precio (ascendente).\n");
        printf("\n=== LISTA DE LIBROS ===\n");
    Libro* actual = cabeza;
    while (actual) {
        printf("Cod: %d | %s - %s ($%.2f)\n",
            actual->codigo, actual->titulo, actual->autor, actual->precio);
        actual = actual->sig;
    }
    printf("\n");
}

// muestra todos los libros
void listar_libros() {
    if (!cabeza) {
        printf("No hay libros.\n");
        return;
    }

    printf("\n=== LISTA DE LIBROS ===\n");
    Libro* actual = cabeza;
    while (actual) {
        printf("Cod: %d | %s - %s ($%.2f)\n",
            actual->codigo, actual->titulo, actual->autor, actual->precio);
        actual = actual->sig;
    }
    printf("\n");
}

// libera toda la memoria al salir
void liberar_todo() {
    while (cabeza) {
        Libro* temp = cabeza;
        cabeza = cabeza->sig;
        liberar_libro(temp);
    }
}

// funcion principal
int main() {
    cargar_libros();

    int opc;
    do {
        printf("\n=== SISTEMA DE LIBROS ===\n");
        printf("1. Agregar libro\n");
        printf("2. Buscar por titulo o autor\n");
        printf("3. Listar todos los libros\n");
        printf("4. Ordenar por precio\n");
        printf("5. Guardar y salir\n");
        printf("Opcion: ");
        scanf("%d", &opc);

        // limpiar buffer de entrada
        while (getchar() != '\n');

        switch (opc) {
            case 1: agregar_libro(); break;
            case 2: buscar_libro(); break;
            case 3: listar_libros(); break;
            case 4: ordenar_por_precio(); break;
            case 5:
                guardar_libros();
                opc = 5; // se usa para salir del bucle
                break;
            default:
                if (opc != 5) printf("Opcion invalida.\n");
        }
    } while (opc != 5);

    liberar_todo();
    printf("Adios.\n");
    return 0;
}