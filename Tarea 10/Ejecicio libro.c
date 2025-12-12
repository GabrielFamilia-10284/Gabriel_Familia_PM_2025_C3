#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura de un libro
typedef struct Libro {
    int ano;
    char* autor;       
    char* titulo;      
    char* descripcion; 
    float precio;
    int codigo;
    struct Libro* sig; // puntero al siguiente (lista enlazada)
} Libro;

// Cabecera de la lista (puntero al primer libro)
Libro* cabeza = NULL;

// Funcion para copiar una cadena a memoria dinamica
char* copiar_cadena(const char* s) {
    if (s == NULL) return NULL;
    char* copia = (char*)malloc(strlen(s) + 1);
    if (copia) strcpy(copia, s);
    return copia;
}

// Libera un libro de la memoria
void liberar_libro(Libro* l) {
    if (l) {
        free(l->autor);
        free(l->titulo);
        free(l->descripcion);
        free(l);
    }
}

// Carga todos los libros desde el archivo binario
void cargar_libros() {
    FILE* f = fopen("libros.dat", "rb");
    if (!f) {
        printf("Archivo no existe. Empezando vacio.\n");
        return;
    }

    // Limpiar lista actual
    while (cabeza) {
        Libro* temp = cabeza;
        cabeza = cabeza->sig;
        liberar_libro(temp);
    }

    Libro tmp;
    while (fread(&tmp, sizeof(Libro), 1, f) == 1) {
        // Leer cadenas (tamanos guardados en el archivo como enteros)
        int len;
        fread(&len, sizeof(int), 1, f);
        tmp.autor = (char*)malloc(len + 1);
        fread(tmp.autor, 1, len, f);
        tmp.autor[len] = '\0';

        fread(&len, sizeof(int), 1, f);
        tmp.titulo = (char*)malloc(len + 1);
        fread(tmp.titulo, 1, len, f);
        tmp.titulo[len] = '\0';

        fread(&len, sizeof(int), 1, f);
        tmp.descripcion = (char*)malloc(len + 1);
        fread(tmp.descripcion, 1, len, f);
        tmp.descripcion[len] = '\0';

        // Insertar al inicio de la lista
        Libro* nuevo = (Libro*)malloc(sizeof(Libro));
        *nuevo = tmp;
        nuevo->sig = cabeza;
        cabeza = nuevo;
    }

    fclose(f);
    printf("Libros cargados desde disco.\n");
}

// Guarda todos los libros al archivo binario
void guardar_libros() {
    FILE* f = fopen("libros.dat", "wb");
    if (!f) {
        printf("Error al abrir archivo para guardar.\n");
        return;
    }

    Libro* actual = cabeza;
    while (actual) {
        // Guardar los datos fijos (int, float)
        fwrite(actual, sizeof(Libro), 1, f);

        // Guardar autor (longitud + cadena)
        int len = strlen(actual->autor);
        fwrite(&len, sizeof(int), 1, f);
        fwrite(actual->autor, 1, len, f);

        // Guardar titulo
        len = strlen(actual->titulo);
        fwrite(&len, sizeof(int), 1, f);
        fwrite(actual->titulo, 1, len, f);

        // Guardar descripcion
        len = strlen(actual->descripcion);
        fwrite(&len, sizeof(int), 1, f);
        fwrite(actual->descripcion, 1, len, f);

        actual = actual->sig;
    }

    fclose(f);
    printf("Libros guardados en disco.\n");
}

// Agrega un libro nuevo (al inicio)
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

    // Limpiar buffer antes de leer cadenas
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

    // Enlazar a la lista (al inicio)
    nuevo->sig = cabeza;
    cabeza = nuevo;

    printf("Libro agregado.\n");
}

// Busca por titulo o autor (ignora mayusculas/minúsculas)
void buscar_libro() {
    if (!cabeza) {
        printf("No hay libros.\n");
        return;
    }

    printf("Buscar por (1) Titulo o (2) Autor: ");
    int opc;
    scanf("%d", &opc);

    while (getchar() != '\n'); // limpiar

    char busqueda[100];
    printf("Texto a buscar: ");
    fgets(busqueda, sizeof(busqueda), stdin);
    busqueda[strcspn(busqueda, "\n")] = '\0';

    // Convertir busqueda a minusculas para comparar
    for (int i = 0; busqueda[i]; i++)
        busqueda[i] = tolower(busqueda[i]);

    int encontrado = 0;
    Libro* actual = cabeza;

    while (actual) {
        char* texto = (opc == 1) ? actual->titulo : actual->autor;
        if (texto) {
            // Hacer copia temporal en minusculas
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

// Muestra todos los libros
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

// Libera toda la memoria al salir
void liberar_todo() {
    while (cabeza) {
        Libro* temp = cabeza;
        cabeza = cabeza->sig;
        liberar_libro(temp);
    }
}

// Menu principal
int main() {
    cargar_libros(); // carga al inicio

    int opc;
    do {
        printf("\n=== SISTEMA DE LIBROS ===\n");
        printf("1. Agregar libro\n");
        printf("2. Buscar libro\n");
        printf("3. Listar todos\n");
        printf("4. Guardar y salir\n");
        printf("Opcion: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: agregar_libro(); break;
            case 2: buscar_libro(); break;
            case 3: listar_libros(); break;
            case 4: guardar_libros(); break;
            default: if (opc != 4) printf("Opcion invalida.\n");
        }
    } while (opc != 4);

    liberar_todo();
    printf("Adios.\n");
    return 0;
}